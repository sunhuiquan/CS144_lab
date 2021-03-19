#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity), _unacceptable(capacity) {}

void StreamReassembler::handle_right_edge(std::pair<uint64_t, std::string> &new_seg)
{
    uint64_t right_end = new_seg.first + new_seg.second.size();
    if (right_end > _unacceptable)
    {
        new_seg.second = new_seg.second.substr(0, _unacceptable - new_seg.first);
    }
}

void StreamReassembler::handle_left_edge(std::pair<uint64_t, std::string> &new_seg)
{
    if (new_seg.first < _unreceived)
    {
        new_seg.first = _unreceived;
        new_seg.second = new_seg.second.substr(_unreceived - new_seg.first);
    }
    _output.write(new_seg.second);
    _unreceived = new_seg.first + new_seg.second.size();

    for (const auto &it : _reassemble_cache)
    {
        if (it.first <= _unreceived)
        {
            if (it.first + it.second.size() <= _unreceived)
            {
                _reassemble_cache.erase(it);
                continue;
            }

            pair<uint64_t, string> temp(it);
            _reassemble_cache.erase(it);
            if (temp.first < _unreceived)
            {
                temp.first = _unacceptable;
                temp.second = temp.second.substr(_unreceived - temp.first);
            }

            _output.write(temp.second);
            _unacceptable = temp.first + temp.second.size();
        }
    }
}

void StreamReassembler::handle_middle(std::pair<uint64_t, std::string> &new_seg)
{
}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof)
{
    pair<uint64_t, string> new_seg(index, data);
    handle_right_edge(new_seg);
    if (new_seg.first <= _unreceived)
    {
        handle_left_edge(new_seg);
    }
    else
    {
        handle_middle(new_seg);
    }
}

size_t StreamReassembler::unassembled_bytes() const
{
    size_t unassembled_bytes = 0;
    for (const auto &it : _reassemble_cache)
    {
        unassembled_bytes += it.second.size();
    }
    return unassembled_bytes;
}

bool StreamReassembler::empty() const
{
    return unassembled_bytes() == 0;
}
