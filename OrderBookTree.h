#pragma once
#include "modele.h"

class OrderBookTree {
private:
    Order* root;
    void rotateLeft(Order*& node);
    void rotateRight(Order*& node);
    void fixInsert(Order*& node);
    void fixDelete(Order*& node);

public:
    OrderBookTree() : root(nullptr) {}

    void insert(Order* newOrder);

    void remove(Order* order);

    Order* getBest();

    bool isEmpty() const { return root == nullptr; }
};