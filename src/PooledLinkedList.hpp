#pragma once

#include <cstddef>
#include <cassert>

#include <iostream>

template <typename T>
struct PoolNode {
    PoolNode<T>* previous;
    PoolNode<T>* next;
    T data;

    PoolNode<T>(T value) : previous(nullptr), next(nullptr), data(value) {}
};

template<typename T>
struct PooledLinkedList {
    std::size_t size;
    PoolNode<T>* front;
    PoolNode<T>* back;

    PooledLinkedList<T>() : size(0), front(nullptr), back(nullptr) {}

    ~PooledLinkedList<T>() {
        clear();
    }

    void clear() {
        size = 0;

        while (front != nullptr) {
            PoolNode<T>* temp = front;
            front = front->next;
            delete temp;
        }
    }

    void push_front(T data) {
        ++size;

        PoolNode<T>* new_node = new PoolNode<T>(data);
        if (front == nullptr) {
            front = new_node;
            back = new_node;
        } else {
            new_node->next = front;
            front->previous = new_node;
            front = new_node;
        }
    }

    void push_back(T data) {
        ++size;

        PoolNode<T>* new_node = new PoolNode<T>(data);
        if (back == nullptr) {
            front = new_node;
            back = new_node;
        } else {
            back->next = new_node;
            new_node->previous = back;
            back = new_node;
        }
    }

    void remove_front() {
        if (front == nullptr) return;

        --size;
        PoolNode<T>* temp = front;
        front = front->next;
        front->previous = nullptr;
        delete temp;
    }

    void remove_back() {
        if (back == nullptr) return;

        --size;
        PoolNode<T>* temp = back;
        back = back->previous;
        back->next = nullptr;

        delete temp;
    }

    T pop_front() {
        std::cout << "front is null: " << (front == nullptr) << std::endl;
        assert(front != nullptr);

        if (size == 1) {
            T temp = front->data;
            delete front;
            front = nullptr;
            return temp;
        }

        --size;
        PoolNode<T>* temp = front;
        front = front->next;
        front->previous = nullptr;

        T data = front->data;
        delete temp;

        return data;
    }

    T pop_back() {
        assert(back != nullptr);

        --size;
        PoolNode<T>* temp = back;
        back = back->previous;
        back->next = nullptr;

        T data = back->data;
        delete temp;

        return data;
    }
};
