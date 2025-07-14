#pragma once

#include <cstddef>

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
};
