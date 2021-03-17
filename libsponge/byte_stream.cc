#include "byte_stream.hh"

#include <algorithm>
#include <iterator>
#include <stdexcept>

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

// template <typename... Targs>
// void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) : _buffer_capacity(capacity) {}

size_t ByteStream::write(const string &data)
{
    /* Can be terminate by end_write(). */
    _end_write = false;
    /* Input-end can write container char by char. */
    size_t char_num = 0;
    for (const auto &it : data)
    {
        if (remaining_capacity())
        {
            _container.push_back(it);
            char_num++;
        }
    }
    _buffer_size += char_num;
    _total_size_write += char_num;
    return char_num;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const
{
    size_t sz = len;
    if (sz > buffer_size())
    {
        sz = buffer_size();
    }
    return string().assign(_container.cbegin(), _container.cend() + sz);
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len)
{
    size_t sz = len;
    if (sz > buffer_size())
    {
        sz = buffer_size();
    }
    for (size_t i = 0; i < sz; ++i)
    {
        _container.pop_front();
    }
    _total_size_read += sz;
    _buffer_size -= sz;
}

void ByteStream::end_input()
{
    _end_write = true;
}

bool ByteStream::input_ended() const
{
    return _end_write;
}

size_t ByteStream::buffer_size() const
{
    return _buffer_size;
}

bool ByteStream::buffer_empty() const
{
    return _buffer_size == 0;
}

bool ByteStream::eof() const
{
    return (buffer_empty() && input_ended());
}

size_t ByteStream::bytes_written() const
{
    return _total_size_write;
}

size_t ByteStream::bytes_read() const
{
    return _total_size_read;
}

size_t ByteStream::remaining_capacity() const
{
    return _buffer_capacity - buffer_size();
}
