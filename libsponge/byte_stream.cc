#include "byte_stream.hh"

#include <algorithm>
#include <iterator>
#include <stdexcept>

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&.../* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity)
{
    buffer_size_ = 0;
    buffer_capacity_ = capacity;
    end_write_ = true;
    total_size_write_ = 0;
    total_size_read_ = 0;
    _error = false;
}

size_t ByteStream::write(const string &data)
{
    /* Can be terminate by end_write(). */
    end_write_ = false;
    /* Input-end can write container char by char. */
    size_t char_num = 0;
    for (const auto &it : data)
    {
        while (remaining_capacity())
        {
            container_.push_back(it);
            char_num++;
        }
    }
    total_size_write_ += char_num;
    return char_num;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const
{
    std::string output_str;
    for (int i = 0; i < len; i++)
    {
        output_str.push_back(container_.at(i));
    }
    return output_str;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len)
{
    for (int i = 0; i < len; i++)
    {
        container_.pop_front();
    }
    total_size_read_ += len;
}

void ByteStream::end_input()
{
    end_write_ = true;
}

bool ByteStream::input_ended() const
{
    if (end_write_ == true)
    {
        return true;
    }
    return false;
}

size_t ByteStream::buffer_size() const
{
    return buffer_size_;
}

bool ByteStream::buffer_empty() const
{
    if (buffer_size() == 0)
    {
        return true;
    }
    return false;
}

bool ByteStream::eof() const
{
    return false;
}

size_t ByteStream::bytes_written() const
{
    return total_size_write_;
}

size_t ByteStream::bytes_read() const
{
    return total_size_read_;
}

size_t ByteStream::remaining_capacity() const
{
    return buffer_capacity_ - buffer_size();
}
