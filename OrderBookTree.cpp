#include "OrderBookTree.h"

OrderBookTree::OrderBookTree()
{
}

void OrderBoolTree::rotateLeft(Order*& node)
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

void OrderBoolTree::rotateRight(Order*& node)
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
