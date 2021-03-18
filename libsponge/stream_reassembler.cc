#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

// template <typename... Targs>
// void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _output(capacity), _capacity(capacity) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof)
{
    if (eof == true)
    {
        _eof = true;
    }

    auto a = _slices.insert(pair<uint64_t, std::string>(index, data));
    auto b = _slices.lower_bound(pair<uint64_t, std::string>(index, data));
    if (b != _slices.end() && b->first != index)
    {
        _slices.erase(a.first);
    }
    else
    {
        _unassembled_bytes += a.first->second.size();
    }

    auto it = a.first;
    while (b != _slices.end())
    {
        if (it->first + it->second.size() == b->first)
        {
            auto aa = _slices.insert(pair<uint64_t, std::string>(it->first, it->second + b->second));
            _slices.erase(it);
            _slices.erase(b);
            b = _slices.lower_bound(*(aa.first));
            it = aa.first;
        }
        else
        {
            break;
        }
    }

    b = _slices.upper_bound(*it);
    while (1)
    {
        if (b->first + b->second.size() == it->first)
        {
            auto aa = _slices.insert(pair<uint64_t, std::string>(b->first, b->second + it->second));
            _slices.erase(b);
            _slices.erase(it);
            b = _slices.upper_bound(*(aa.first));
            it = aa.first;
        }
        else
        {
            break;
        }
        if (b == _slices.begin())
        {
            break;
        }
    }

    if (_eof == true)
    {
        if (_slices.size() == 1)
        {
            _output.write(_slices.begin()->second);
            _unassembled_bytes -= _slices.begin()->second.size();
            _slices.clear();
        }
    }
}

size_t StreamReassembler::unassembled_bytes() const
{
    return _unassembled_bytes;
}

bool StreamReassembler::empty() const
{
    if (unassembled_bytes() == 0)
    {
        return true;
    }
    return false;
}
