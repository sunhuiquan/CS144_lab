#include "tcp_connection.hh"

#include <iostream>

// Dummy implementation of a TCP connection

// For Lab 4, please replace with a real implementation that passes the
// automated checks run by `make check`.

using namespace std;

size_t TCPConnection::remaining_outbound_capacity() const
{
    return _sender.stream_in().remaining_capacity();
}

size_t TCPConnection::bytes_in_flight() const
{
    return _sender.bytes_in_flight();
}

size_t TCPConnection::unassembled_bytes() const
{
    return _receiver.unassembled_bytes();
}

size_t TCPConnection::time_since_last_segment_received() const
{
    return _time_since_last_segment_received;
}

void TCPConnection::segment_received(const TCPSegment &seg)
{
    if (!active())
    {
        return;
    }
    _time_since_last_segment_received = 0;

    // STATE: LISTEN => SYN_RECV
    // receive SYN / send SYN + ACK
    if (!_receiver.ackno().has_value() && _sender.next_seqno_absolute() == 0)
    {
        if (!seg.header().syn)
        {
            return;
        }
        _receiver.segment_received(seg);
        connect();
        return;
    }

    // STATE: SYN_SENT => ESTABLISED
    // receive SYN + ACK / send ACK
    if (!_receiver.ackno().has_value() && _sender.next_seqno_absolute() > 0 && _sender.next_seqno_absolute() == _sender.bytes_in_flight())
    {
        if (seg.payload().size())
        {
            return;
        }
        if (!seg.header().ack)
        {
            if (seg.header().syn)
            {
                // simultaneous open
                _receiver.segment_received(seg);
                // to easy the process not use this function
                // _sender.send_syn_ack_segment();
                _sender.send_empty_segment();
            }
            return;
        }
        if (seg.header().rst)
        {
            // now we haven't build the connection with the server,
            // so don't care about the server, don't need to inform it
            _receiver.stream_out().set_error();
            _sender.stream_in().set_error();
            _active = false;
            return;
        }
    }

    _receiver.segment_received(seg);
    // _sender.ack_received(seg.header().ackno, seg.header().win);

    //after establising and receive a bad ack, you should send empty(the exists one)
    if (!_sender.ack_received(seg.header().ackno, seg.header().win))
    {
        if (_sender.next_seqno_absolute() > 0 && seg.header().ack)
        {
            if (_receiver.ackno().has_value() && _sender.segments_out().empty() && !seg.length_in_sequence_space())
            {
                _sender.send_empty_segment();
            }
        }
    }

    // empty segment will add ACK in send_segments function
    if (_sender.segments_out().empty() && seg.length_in_sequence_space())
    {
        _sender.send_empty_segment();
    }

    if (seg.header().rst)
    {
        _sender.send_empty_segment();
        // to make sure the sender point still have segments to send,
        // and we'll use this segment to tell the server rst have been set
        unclean_shutdown();
        return;
    }

    send_segments();
}

bool TCPConnection::active() const
{
    return _active;
}

size_t TCPConnection::write(const string &data)
{
    // to avoid the waste of invoke other functions
    if (!data.size())
    {
        return 0;
    }
    size_t write_size = _sender.stream_in().write(data);
    // after write new data in sender's byte stream, just send it as soon as possible
    _sender.fill_window();
    send_segments();
    return write_size;
}

//! \param[in] ms_since_last_tick number of milliseconds since the last call to this method
void TCPConnection::tick(const size_t ms_since_last_tick)
{
    if (!_active)
    {
        return;
    }
    _time_since_last_segment_received += ms_since_last_tick;
    _sender.tick(ms_since_last_tick);
    if (_sender.consecutive_retransmissions() > TCPConfig::MAX_RETX_ATTEMPTS)
    {
        unclean_shutdown();
    }
    // _sender.tick() may invoke the fill_window() function
    send_segments();
}

void TCPConnection::end_input_stream()
{
    _sender.stream_in().end_input();
    // end_input() may cause eof, and fill_window() then _sender
    // may set _fin flag to true and send a empty segment with this flag
    _sender.fill_window();
    send_segments();
}

void TCPConnection::connect()
{
    _sender.fill_window();
    send_segments();
}

TCPConnection::~TCPConnection()
{
    try
    {
        if (active())
        {
            cerr << "Warning: Unclean shutdown of TCPConnection\n";

            // when decoustuct runs, force to end
            _sender.send_empty_segment();
            unclean_shutdown();
        }
    }
    catch (const exception &e)
    {
        std::cerr << "Exception destructing TCP FSM: " << e.what() << std::endl;
    }
}

void TCPConnection::send_segments()
{
    // cout << "a" << endl;
    TCPSegment seg;
    while (!_sender.segments_out().empty())
    {
        seg = _sender.segments_out().front();
        _sender.segments_out().pop();
        if (_receiver.ackno().has_value())
        {
            seg.header().ack = true;
            seg.header().ackno = _receiver.ackno().value();
            seg.header().win = _receiver.window_size();
        }
        _segments_out.push(seg);
    }
    // test whether the connection should close
    clean_shutdown();
}

void TCPConnection::unclean_shutdown()
{
    _receiver.stream_out().set_error();
    _sender.stream_in().set_error();
    _active = false;
    TCPSegment seg = _sender.segments_out().front();
    _sender.segments_out().pop();

    seg.header().rst = true;
    if (_receiver.ackno().has_value())
    {
        seg.header().ack = true;
        seg.header().win = _receiver.window_size();
        seg.header().ackno = _receiver.ackno().value();
    }
    _segments_out.push(seg);
}

void TCPConnection::clean_shutdown()
{
    // receiver input_ended is enough for application may never get the data off
    // the sender must eof to ensure all the data have been sent
    if (_receiver.stream_out().input_ended())
    {
        if (!_sender.stream_in().eof())
        {
            _linger_after_streams_finish = false;
        }
        else if (_sender.bytes_in_flight() == 0)
        {
            if (!_linger_after_streams_finish || time_since_last_segment_received() >= 10 * _cfg.rt_timeout)
            {
                _active = false;
            }
        }
    }
}