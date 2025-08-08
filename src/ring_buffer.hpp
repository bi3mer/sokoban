#include <cassert>
#include <cstddef>

template <typename T, std::size_t size>
struct RingBuffer {
    std::size_t index = 0;
    std::size_t last_valid_index = 0;
    T ring_buffer[size];

    void clear() {
        index = 0;
        last_valid_index = 0;
    }

    T operator[](std::size_t i){
        assert(i < size);
        return ring_buffer[i];
    }

    void push(const T data) {
        ring_buffer[index] = data;
        index = (index + 1) % size;

        if (index + 1 >= size) {
            index = 0;
            last_valid_index = index + 1;
        }
    }

    T pop() {
        const std::size_t i = index;
        index = (index - 1) % size;

        return ring_buffer[i];
    }
};