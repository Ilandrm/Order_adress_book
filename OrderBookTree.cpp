#include "OrderBookTree.h"

OrderBookTree::OrderBookTree()
{
}

void OrderBookTree::rotateLeft(Order*& node)
{
    Order* node_second = node->right;

    node->right = node_second->left;
    if (node_second->left != nullptr) {
        node_second->left->parent = node;
    }

    node_second->parent = node->parent;
    if (node->parent == nullptr)
    {
        root = node_second;
    } else if (node == node->parent->left) {
        node->parent->left = node_second;
    } else
    {
        node->parent->right = node_second;
    }
    node_second->left = node;
    node->parent = node_second;
}

void OrderBookTree::rotateRight(Order*& node)
{
    Order* node_second = node->left;

    node->left = node_second->right;
    if (node_second->right != nullptr) {
        node_second->right->parent = node;
    }

    node_second->parent = node->parent;
    if (node->parent == nullptr)
    {
        root = node_second;
    } else if (node == node->parent->right) {
        node->parent->right = node_second;
    } else
    {
        node->parent->left = node_second;
    }
    node_second->right = node;
    node->parent = node_second;
}

void OrderBookTree::insert(Order* newOrder)
{
    Order* parent_order = nullptr;
    Order* current_order = root
    while (current_order != nullptr)
    {
        parent_order = current_order;
        if (newOrder->price < current_order->price)
        {
            current_order = current_order->left;
        } else if (current_order->price < newOrder->price)
        {
            current_order = current_order->right;
        } else {
            current_order = current_order->right;
        }
    }
    newOrder->parent = parent_order;
    newOrder->left = nullptr;
    newOrder->right = nullptr;
    newOrder->is_red = true;
    fixOrder(newOrder);
    if (parent_order == nullptr) {
        root = newOrder;
    } else if (newOrder->price < parent_order->price) {
        parent_order->left = newOrder;
    } else if (newOrder->price > parent_order->price) {
        parent_order->right = newOrder;
    } else {
        parent_order->right = newOrder;
    }
}
void OrderBookTree::fixInsert(Order* order) {
    while (order->parent != nullptr && order->parent->is_red) {
        if (order->parent == order->parent->parent->left) {
            Order* oncle = order->parent->parent->right;

            if (oncle != nullptr && oncle->is_red) {
                order->parent->is_red = false;
                oncle->is_red = false;
                order->parent->parent->is_red = true;
                order = order->parent->parent;
            } else {
                if (order == order->parent->right) {
                    order = order->parent;
                    rotateLeft(order);
                }
                order->parent->is_red = false;
                order->parent->parent->is_red = true;
                rotateRight(order->parent->parent);
            }
        } else {
            Order* oncle = order->parent->parent->left;

            if (oncle != nullptr && oncle->is_red) {
                order->parent->is_red = false;
                oncle->is_red = false;
                order->parent->parent->is_red = true;
                order = order->parent->parent;
            } else {
                if (order == order->parent->left) {
                    order = order->parent;
                    rotateRight(order);
                }
                order->parent->is_red = false;
                order->parent->parent->is_red = true;
                rotateLeft(order->parent->parent);
            }
        }
    }

    root->is_red = false;
}
