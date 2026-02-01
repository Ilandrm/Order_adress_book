#pragma once
#include "modele.h"

class OrderBookTree {
private:
    Order* root;
    Order* best_order;
    bool is_min_tree;

    void rotateLeft(Order* node);
    void rotateRight(Order* node);
    void fixInsert(Order* node);
    void fixDelete(Order* node, Order* parent);
    void transplant(Order* u, Order* v);
    Order* minimum(Order* node);

public:
    OrderBookTree(bool isMinTree = true);
    void insert(Order* newOrder);
    void remove(Order* order);
    Order* getBest();
    bool isEmpty() const { return root == nullptr; }
};
