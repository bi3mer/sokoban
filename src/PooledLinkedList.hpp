#pragma once

#include "object_pool.hpp"
#include <cstddef>
#include <cassert>

template <typename T>
struct PoolNode {
    PoolNode<T>* previous;
    PoolNode<T>* next;
    T data;

    PoolNode<T>() : previous(nullptr), next(nullptr) {}
    PoolNode<T>(const T value) : previous(nullptr), next(nullptr), data(value) {}
};

template<typename T>
struct PooledLinkedList {
    std::size_t size;
    PoolNode<T>* front;
    PoolNode<T>* back;
    ObjectPool<PoolNode<T>> pool;

    PooledLinkedList<T>() : size(0), front(nullptr), back(nullptr) {}
    PooledLinkedList<T>(std::size_t pool_size) : size(0), front(nullptr), back(nullptr), pool(pool_size) {}

    ~PooledLinkedList<T>() {
        clear();
    }

    void clear() {
        size = 0;

        while (front != nullptr) {
            PoolNode<T>* object = front;
            pool.return_object(object);

            front = front->next;
            object->previous = nullptr;
            object->next = nullptr;
        }

        front = nullptr;
        back = nullptr;
    }

    void push_front(const T data) {
        ++size;

        PoolNode<T>* new_node = pool.get_object();
        new_node->data = data;

        if (front == nullptr) {
            front = new_node;
            back = new_node;

            new_node->next = nullptr;
            new_node->previous = nullptr;
        } else {
            new_node->next = front;
            front->previous = new_node;
            front = new_node;
        }
    }

    void push_back(const T data) {
        ++size;

        PoolNode<T>* new_node = pool.get_object();
        new_node->data = data;

        if (back == nullptr) {
            front = new_node;
            back = new_node;

            new_node->next = nullptr;
            new_node->previous = nullptr;
        } else {
            back->next = new_node;
            new_node->previous = back;
            back = new_node;
        }
    }

    void remove_front() {
        if (front == nullptr) return;

        --size;
        PoolNode<T>* object = front;
        front = front->next;
        front->previous = nullptr;

        pool.return_object(object);
    }

    void remove_back() {
        if (back == nullptr) return;

        --size;
        PoolNode<T>* object = back;
        back = back->previous;
        back->next = nullptr;

        pool.return_object(object);
    }

    T pop_front() {
        assert(front != nullptr);

        T data = front->data;
        --size;

        if (front == back) {
            pool.return_object(front);

            front = nullptr;
            back = nullptr;
        } else {
            PoolNode<T>* object = front;
            front = front->next;
            front->previous = nullptr;

            pool.return_object(object);
        }

        return data;
    }

    T pop_back() {
        assert(back != nullptr);

        T data = back->data;
        --size;

        if (front == back) {
            pool.return_object(back);

            front = nullptr;
            back = nullptr;
        } else {
            PoolNode<T>* object = back;
            back = back->previous;
            back->next = nullptr;

            pool.return_object(object);
        }

        return data;
    }
};
