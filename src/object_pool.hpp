#pragma once
#include <cstddef>
#include <vector>

#include "log.hpp"

template<typename T>
struct ObjectPool {
    std::vector<T*> pool;

    ObjectPool() {};

    ObjectPool(std::size_t initial_pool_size)  {
       allocate_objects(initial_pool_size);
    }

    ~ObjectPool() {
        for (T* t : pool) {
            delete t;
        }
    }

    void allocate_objects(std::size_t size) {
        for (std::size_t i = 0; i < size; ++i) {
            pool.push_back(new T());
        }
    }

    T* get_object() {
        if (pool.empty()) {
            return new T();
        }

        T* object = pool.back();
        pool.pop_back();

        return object;
    }

    void return_object(T* object) {
        pool.push_back(object);
    }
};