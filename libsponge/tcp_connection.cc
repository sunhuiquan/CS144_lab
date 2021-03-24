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
    if (!_receiver.ackno().has_value() && _sender.next_seqno_absolute() > 0)
    {
        if (seg.payload().size() != 0)
        {
            return;
        }
        if (!seg.header().ack)
        {
            if (seg.header().syn)
            {
                // simultaneous open
                _receiver.segment_received(seg);
                _sender.send_syn_ack_segment();
            }
            return;
        }
        if (seg.header().rst)
        {
            _receiver.stream_out().set_error();
            _sender.stream_in().set_error();
            _active = false;
            return;
        }
    }

    // SYN_SENT => ESTABLISED send ACK and usual data transmission's ACK
    // and SYN_RECV rece ACK without SYN are both fine
    _receiver.segment_received(seg);
    _sender.ack_received(seg.header().ackno, seg.header().win);
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
    if (_sender.consecutive_retransmissions() >= TCPConfig::MAX_RETX_ATTEMPTS)
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

            // Your code here: need to send a RST segment to the peer
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
}

void TCPConnection::unclean_shutdown()
{
}

void TCPConnection::clean_shutdown()
{
}