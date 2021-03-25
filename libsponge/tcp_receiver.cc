#include "tcp_receiver.hh"

// Dummy implementation of a TCP receiver

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

using namespace std;

bool TCPReceiver::segment_received(const TCPSegment &seg)
{
    const TCPHeader &header = seg.header();
    // It's a SYN, but if _syn has been set to true, you can't set it again.
    // And if you haven't set it, just set it to true.
    if (header.syn)
    {
        if (_syn)
            return false;
        _syn = true;
        _isn = header.seqno.raw_value();
    }

    if (_syn)
    {
        bool eof = false;
        if (header.fin)
        {
            if (_fin)
                return false;

            _fin = true;
            eof = true;
        }

        uint64_t seqno = unwrap(header.seqno, WrappingInt32(_isn), _check_point);
        // Convert the absolute sequence number to the stream number
        if (header.syn)
        {
            seqno = 1;
        }

        // The seqno minus 1 is the index of byte stream.
        if (seqno - 1 >= _reassembler.unacceptable_byte())
        {
            _seqno_out_of_window = true;
            return false;
        }

        if (seqno - 1 + seg.length_in_sequence_space() <= _reassembler.first_unreceived_byte())
        {
            return false;
        }

        _reassembler.push_substring(string(seg.payload().str()), seqno - 1, eof);
        _check_point = seqno;
        return true;
    }

    // _syn == false
    return false;
}

optional<WrappingInt32> TCPReceiver::ackno() const
{
    uint64_t offset = 1;
    if (_fin && _reassembler.unassembled_bytes() == 0)
    {
        offset = 2;
    }

    if (_syn == true)
    {
        return wrap(_reassembler.first_unreceived_byte() + offset, WrappingInt32(_isn));
    }
    return {};
}

size_t TCPReceiver::window_size() const
{
    // size_t here(64-bit system) is uint64_t
    return _capacity - _reassembler.stream_out().buffer_size();
}
