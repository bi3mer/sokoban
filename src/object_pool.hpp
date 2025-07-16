#pragma once

#include "log.hpp"
#include <vector>

template<typename T>
struct ObjectPool {
    std::vector<T*> pool;

    ~ObjectPool<T>() {
        for (T* t : pool) {
            delete t;
        }
    }

    void alloc_more(std::size_t size) {
        for (std::size_t i = 0; i < size; ++i) {
            pool.push_back(new T());
        }
    }

    T* get_object() {
        if (pool.empty()) {
            Log::debug("pool :: alloc");
            return new T();
        }

        Log::debug("pool :: get");
        T* object = pool.back();
        pool.pop_back();

        return object;
    }

    void return_object(T* object) {
        pool.push_back(object);
    }
};