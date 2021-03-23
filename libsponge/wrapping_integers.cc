#include "wrapping_integers.hh"

// Dummy implementation of a 32-bit wrapping integer

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

using namespace std;

//! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
//! \param n The input absolute 64-bit sequence number
//! \param isn The initial sequence number
WrappingInt32 wrap(uint64_t n, WrappingInt32 isn)
{
    return isn + n;
}

//! Transform a WrappingInt32 into an "absolute" 64-bit sequence number (zero-indexed)
//! \param n The relative sequence number
//! \param isn The initial sequence number
//! \param checkpoint A recent absolute 64-bit sequence number
//! \returns the 64-bit sequence number that wraps to `n` and is closest to `checkpoint`
//!
//! \note Each of the two streams of the TCP connection has its own ISN. One stream
//! runs from the local TCPSender to the remote TCPReceiver and has one ISN,
//! and the other stream runs from the remote TCPSender to the local TCPReceiver and
//! has a different ISN.

// #include <iostream>
// using std::cout;
// using std::endl;

uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint)
{
    uint64_t absolute_no = 0;
    if (n.raw_value() >= isn.raw_value())
    {
        absolute_no = n.raw_value() - isn.raw_value();
    }
    else
    {
        absolute_no = 1 + (uint64_t)UINT32_MAX - isn.raw_value() + n.raw_value();
        // minus first to avoid overflow
    }

    uint64_t curr_diff = UINT64_MAX, old_diff = UINT64_MAX;
    uint64_t old_absolute_no = absolute_no;
    while (1)
    {
        uint64_t temp;
        if (checkpoint >= absolute_no)
        {
            temp = checkpoint - absolute_no;
        }
        else
        {
            temp = absolute_no - checkpoint;
        }

        old_diff = curr_diff;
        curr_diff = temp;

        if (curr_diff > old_diff)
        {
            break;
        }

        if (absolute_no + UINT32_MAX < absolute_no)
        {
            break;
        }
        old_absolute_no = absolute_no;
        absolute_no += (uint64_t)UINT32_MAX + 1;
    }
    // Using these for testing.
    // cout << "a" << endl;
    // cout << old_absolute_no << endl;
    return old_absolute_no;
}
