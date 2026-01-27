#pragma once
#include "modele.h"

class OrderBookTree {
private:
    Order* root;
    Order* best_order;
    void rotateLeft(Order*& node);
    void rotateRight(Order*& node);
    void fixInsert(Order*& node);
    void fixDelete(Order*& node);
    void transplant(Order* u, Order* v)
    void minimum(Order* node)
public:
    OrderBookTree() : root(nullptr) {}

    void insert(Order* newOrder);

    void remove(Order* order);

    Order* getBest();

    bool isEmpty() const { return root == nullptr; }
};