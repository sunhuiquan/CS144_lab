#include "tcp_connection.hh"

#include <iostream>

// Dummy implementation of a TCP connection

// For Lab 4, please replace with a real implementation that passes the
// automated checks run by `make check`.

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

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
    if (seg.header().rst)
    {
        //! todo
        return;
    }

    if (seg.header().ack == true)
    {
        _sender.ack_received(seg.header().seqno, seg.header().win);
    }
    else if (_receiver.segment_received(seg) == true)
    {
        // we don't need to track the ack segment
        TCPSegment ack_seg;
        ack_seg.header().ack = true;
        ack_seg.header().seqno = wrap(_receiver.stream_rassembler().unreceived_byte() + 1, _receiver.get_isn());
        _segments_out.push(ack_seg);
    }
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
    return write_size;
}

//! \param[in] ms_since_last_tick number of milliseconds since the last call to this method
void TCPConnection::tick(const size_t ms_since_last_tick)
{
    // ???
    if (!_active)
    {
        return;
    }
    _time_since_last_segment_received += ms_since_last_tick;

    _sender.tick(ms_since_last_tick);
    if (_sender.consecutive_retransmissions() >= TCPConfig::MAX_RETX_ATTEMPTS)
    {
        // to do:
        // RST
    }
    // ???
}

void TCPConnection::end_input_stream()
{
    _sender.stream_in().end_input();
    // end_input() may cause eof, and fill_window() then _sender
    // may set _fin flag to true and send a empty segment with this flag
    _sender.fill_window();
}

void TCPConnection::connect()
{
    _sender.fill_window();
}

TCPConnection::~TCPConnection()
{
    try
    {
        if (active())
        {
            cerr << "Warning: Unclean shutdown of TCPConnection\n";

            // Your code here: need to send a RST segment to the peer
        }
    }
    catch (const exception &e)
    {
        std::cerr << "Exception destructing TCP FSM: " << e.what() << std::endl;
    }
}
