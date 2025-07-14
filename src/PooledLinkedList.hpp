#pragma once

#include <cstddef>
#include <iostream>
template <typename T>
struct PooledNode {
    PooledNode* previous;
    PooledNode* next;

    T data;

    PooledNode(T value) : data(value), next(nullptr) {}
 };

 template <typename T>
 struct PooledLinkedList {
    std::size_t size;
    PooledNode<T>* tail;

    PooledLinkedList<T>() : tail(nullptr), size(0) {}

    ~PooledLinkedList<T>() {
        clear();
    }

    void clear() {
        size = 0;
        while (tail != nullptr) {
            PooledNode<T>* temp = tail->previous;
            delete tail;
            tail = temp;
        }
    }

    void push(T value) {
        ++size;
        PooledNode<T>* new_node = new PooledNode<T>(value);

        if (tail == nullptr) {
            tail = new_node;
        } else {
            tail->next = new_node;
            new_node->previous = tail;
            tail = new_node;
        }
    }

    void remove_tail() {
        if (tail == nullptr) {
            std::cout << "head was null" << std::endl;
            return;
        }

        --size;
        PooledNode<T>* old_tail = tail;
        tail = tail->previous;
        delete old_tail;
    }
 };
