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

// #include <iostream>

size_t ByteStream::write(const string &data)
{
    // std::cout << data << std::endl;
    size_t len = data.size();
    if (len > remaining_capacity())
    {
        len = remaining_capacity();
    }
    _total_size_write += len;
    for (size_t i = 0; i < len; ++i)
    {
        _container.push_back(data[i]);
    }
    return len;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const
{
    size_t sz = len;
    if (sz > buffer_size())
    {
        sz = buffer_size();
    }
    // string s;
    // for (size_t i = 0; i < sz; ++i)
    // {
    //     s.push_back(_container.at(i));
    // }
    // return 0;
    return string().assign(_container.cbegin(), _container.cbegin() + sz);
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
    return _container.size();
}

bool ByteStream::buffer_empty() const
{
    return buffer_size() == 0;
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
