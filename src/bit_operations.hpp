#pragma once

#include <cstddef>

inline void set_uchar_bit(unsigned char* val, std::size_t index) {
    const std::size_t byte_index = index / 8;
    const std::size_t bit_position = index % 8;

    val[byte_index] |= (1 << bit_position);
}

inline void clear_uchar_bit(unsigned char* val, std::size_t index) {
    const std::size_t byte_index = index / 8;
    const std::size_t bit_position = index % 8;

    val[byte_index] &= ~(1 << bit_position);
}

inline bool get_uchar_bit(unsigned char* val, std::size_t index) {
    const std::size_t byte_index = index / 8;
    const std::size_t bit_position = index % 8;

    return (val[byte_index] >> bit_position) & 1;
}