#include <cassert>
#include <cstddef>
#include <cstdlib>

template <typename T, std::size_t size>
struct RingBuffer {
    static_assert(size > 0, "RingBuffer size must be greater than 0.");

    std::size_t back;
    std::size_t front;
    std::size_t count;
    T list[size];

    RingBuffer() : back(0), front(0), count(0) {}

    inline void clear() noexcept {
        back = 0;
        front = 0;
        count = 0;
    }

    inline bool empty() const noexcept {
        return count == 0;
    }

    inline std::size_t capacity() const noexcept {
        return size;
    }

    T operator[](std::size_t i) noexcept {
        assert(i < count);
        return list[(front + i) % size];
    }

    const T& operator[](std::size_t i) const noexcept {
        assert(i < count);
        return list[(front + i) % size];
    }

    // @TODO: implement this
    [[noreturn]] void push_front(const T& data) {
        std::abort();
    }

    T get_back() noexcept {
        return list[(back - 1) % size];
    }

    const T& get_back() const noexcept {
        return list[(back - 1) % size];
    }

    void push_back(const T& data) noexcept {
        list[back] = data;
        back = (back + 1) % size;

        if (count < size) {
            ++count;
        } else {
            front = (front + 1) % size;
        }
    }

    T pop_back() noexcept {
        assert(!empty());
        back = (back - 1) % size;
        --count;

        return list[back];
    }

    T pop_front() noexcept {
        assert(!empty());

        const std::size_t index = front;
        front = (front + 1) % size;
        --count;

        return list[index];
    }
};