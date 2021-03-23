#include "tcp_sender.hh"

#include "tcp_config.hh"

#include <random>

// #include <iostream>

// Dummy implementation of a TCP sender

// For Lab 3, please replace with a real implementation that passes the
// automated checks run by `make check_lab3`.

using namespace std;

//! \param[in] capacity the capacity of the outgoing byte stream
//! \param[in] retx_timeout the initial amount of time to wait before retransmitting the oldest outstanding segment
//! \param[in] fixed_isn the Initial Sequence Number to use, if set (otherwise uses a random ISN)
TCPSender::TCPSender(const size_t capacity, const uint16_t retx_timeout, const std::optional<WrappingInt32> fixed_isn)
    : _isn(fixed_isn.value_or(WrappingInt32{random_device()()})), _initial_retransmission_timeout{retx_timeout}, _stream(capacity), _current_retransmission_timeout(retx_timeout) {}

uint64_t TCPSender::bytes_in_flight() const
{
    return _bytes_in_flight;
}

void TCPSender::fill_window()
{
    // For the first time you should send a SYN segment.
    if (!_syn)
    {
        _syn = true;
        TCPSegment seg;
        seg.header().syn = true;
        send_segment(seg);
        return;
    }

    if (!_segments_outgoing.empty() && _segments_outgoing.front().header().syn)
    {
        return;
    }
    if (_fin)
    {
        return;
    }
    // if not eof just have not write now, just do noting
    if (!_stream.buffer_size() && !_stream.eof())
    {
        return;
    }

    while (_receiver_window_free_space)
    {
        TCPSegment seg;
        size_t send_max = std::min(TCPConfig::MAX_PAYLOAD_SIZE, _receiver_window_free_space);
        size_t rest_stream_size = _stream.buffer_size();
        if (rest_stream_size < send_max)
        {
            seg.payload() = _stream.read(rest_stream_size);
            if (_stream.eof())

            {
                seg.header().fin = true;
                _fin = true;
            }
            send_segment(seg);
            return;
        }
        else
        {
            seg.payload() = _stream.read(send_max);
            send_segment(seg);
        }
    }
}

void TCPSender::send_segment(TCPSegment &seg)
{
    // Set the sequence number(not absolute seqno).
    seg.header().seqno = wrap(_next_seqno, _isn);
    _next_seqno += seg.length_in_sequence_space();
    // Add the outstanding bytes.
    _bytes_in_flight += seg.length_in_sequence_space();
    // Keep tracking the send one.
    _segments_outgoing.push(seg);
    // cout << "send: " << string(seg.payload().str()) << endl;
    _segments_out.push(seg);
    // reduce the receiver free-window space
    _receiver_window_free_space -= seg.length_in_sequence_space();
    // Open the timer if it hasn't open.
    if (!_timer_is_running)
    {
        _timer_is_running = true;
        _time = 0;
    }
}

//! \param ackno The remote receiver's ackno (acknowledgment number)
//! \param window_size The remote receiver's advertised window size
//! \returns `false` if the ackno appears invalid (acknowledges something the TCPSender hasn't sent yet)
bool TCPSender::ack_received(const WrappingInt32 ackno, const uint16_t window_size)
{
    uint64_t absolute_ackno = unwrap(ackno, _isn, _next_seqno);
    if (!ackno_valid(absolute_ackno))
    {
        // cout << "invalid ackno" << endl;
        return false;
    }

    while (!_segments_outgoing.empty())
    {
        const auto &seg = _segments_outgoing.front();
        uint64_t first_non_acked = unwrap(seg.header().seqno, _isn, _next_seqno);
        if (absolute_ackno >= first_non_acked + seg.length_in_sequence_space())
        {
            _bytes_in_flight -= seg.length_in_sequence_space();
            _segments_outgoing.pop();

            _time = 0;
            _current_retransmission_timeout = _initial_retransmission_timeout;
            _consecutive_retransmissions = 0;
        }
        else
        {
            break;
        }
    }

    _receiver_window_size = window_size;
    _receiver_window_free_space = window_size - _bytes_in_flight;

    if (_segments_outgoing.empty())
    {
        _timer_is_running = false;
    }

    fill_window();
    return true;
}

//! \param[in] ms_since_last_tick the number of milliseconds since the last call to this method
void TCPSender::tick(const size_t ms_since_last_tick)
{
    if (_timer_is_running)
    {
        _time += ms_since_last_tick;
        if (_time >= _current_retransmission_timeout)
        {
            // You don't need worry that the receiver's window free space,
            // for this kind segent you have send before, and you did cut
            // the window size down when you first send it.
            _segments_out.push(_segments_outgoing.front());
            if (_receiver_window_size || _segments_outgoing.front().header().syn)
            {
                ++_consecutive_retransmissions;
                _current_retransmission_timeout *= 2;
            }
            _time = 0;
        }
    }
}

unsigned int TCPSender::consecutive_retransmissions() const
{
    return _consecutive_retransmissions;
}

void TCPSender::send_empty_segment()
{
    TCPSegment empty_segment;
    empty_segment.header().seqno = wrap(_next_seqno, _isn);
    // we don't need to track this empty segment
    _segments_out.push(empty_segment);
}

bool TCPSender::ackno_valid(uint64_t abs_ackno) const
{
    // If ackno less than the left edge it's not perfect
    // but also not a fault.
    if (abs_ackno <= _next_seqno)
    {
        return true;
    }
    return false;
}