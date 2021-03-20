#include "stream_reassembler.hh"

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : _unacceptable(capacity), _output(capacity), _capacity(capacity) {}

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
        if (new_seg.first + new_seg.second.size() <= _unreceived)
        {
            return;
        }
        new_seg.second = new_seg.second.substr(_unreceived - new_seg.first);
        new_seg.first = _unreceived;
    }
    _output.write(new_seg.second);
    _unreceived = new_seg.first + new_seg.second.size();
    _unacceptable = _unreceived + _capacity;

    for (auto it = _reassemble_cache.begin(); it != _reassemble_cache.end();)
    {
        if (it->first <= _unreceived)
        {
            if (it->first + it->second.size() <= _unreceived)
            {
                it = _reassemble_cache.erase(it);
                continue;
            }

            pair<uint64_t, string> temp(*it);
            it = _reassemble_cache.erase(it);
            if (temp.first < _unreceived)
            {
                temp.first = _unacceptable;
                temp.second = temp.second.substr(_unreceived - temp.first);
            }

            _output.write(temp.second);
            _unreceived = temp.first + temp.second.size();
            _unacceptable = _unreceived + _capacity;
            break;
        }
        else
        {
            ++it;
        }
    }
}

void StreamReassembler::handle_middle(std::pair<uint64_t, std::string> &new_seg)
{
    for (auto it = _reassemble_cache.begin(); it != _reassemble_cache.end();)
    {
        if (it->first == new_seg.first)
        {
            if (it->second.size() >= new_seg.second.size())
            {
                return;
            }
            else
            {
                it = _reassemble_cache.erase(it);
            }
        }
        else
        {
            ++it;
        }
    }

    pair<uint64_t, std::string> left_merge;
    bool left_can_merge = false;
    pair<uint64_t, std::string> right_merge;
    bool right_can_merge = false;

    auto right = _reassemble_cache.upper_bound(new_seg);
    for (; right != _reassemble_cache.end(); right = _reassemble_cache.upper_bound(*right))
    {
        if (right->first <= new_seg.first + new_seg.second.size())
        {
            if (right->first + right->second.size() <= new_seg.first + new_seg.second.size())
            {
                _reassemble_cache.erase(*right);
            }
            else
            {
                right_merge = *right;
                right_can_merge = true;
                _reassemble_cache.erase(*right);
            }
        }
        else
        {
            break;
        }
    }

    auto left = _reassemble_cache.lower_bound(new_seg);
    for (; left != _reassemble_cache.end(); left = _reassemble_cache.lower_bound(*left))
    {
        if (left->first + left->second.size() < new_seg.first)
        {
            break;
        }
        else if (left->first + left->second.size() >= new_seg.first + new_seg.second.size())
        {
            return;
        }
        else
        {
            left_merge = *left;
            left_can_merge = true;
            _reassemble_cache.erase(*left);
        }
    }

    if (left_can_merge == true)
    {
        new_seg.first = left_merge.first;
        new_seg.second = left_merge.second + new_seg.second.substr(left_merge.first + left_merge.second.size());
    }
    if (right_can_merge == true)
    {
        new_seg.second = new_seg.second + right_merge.second.substr(new_seg.first + new_seg.second.size());
    }
    _reassemble_cache.insert(new_seg);
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

    if (eof == true)
    {
        _eof = true;
    }

    if (_eof == true && empty())
    {
        _output.end_input();
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
