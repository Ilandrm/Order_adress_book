#include "OrderBookTree.h"

OrderBookTree::OrderBookTree()
{
    root = nullptr
    best_order = nullptr;
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
    Order* current_order = root;
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
    if (best_order == nullptr) {
        best_order = newOrder;
    } else {
        if (is_min_tree) {
            if (newOrder->price < best_order->price) best_order = newOrder;
        } else {
            if (newOrder->price > best_order->price) best_order = newOrder;
        }
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
void OrderBookTree::transplant(Order* u, Order* v) {
    if (u->parent == nullptr) {
        root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    if (v != nullptr) {
        v->parent = u->parent;
    }
}
Order* OrderBookTree::minimum(Order* node) {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}
void OrderBookTree::remove(Order* order) {
    if (order == best_order) {
        best_order = nullptr;
    }
    Order* y = order;
    bool y_original_color = y->is_red;
    Order* x = nullptr;

    if (order->left == nullptr) {
        x = order->right;
        transplant(order, order->right);
    } else if (order->right == nullptr) {
        x = order->left;
        transplant(order, order->left);
    } else {
        y = minimum(order->right);
        y_original_color = y->is_red;
        x = y->right;
        if (y->parent == order) {
            if (x != nullptr) {
                x->parent = y;
            }
        } else {
            transplant(y, y->right);
            y->right = order->right;
            y->right->parent = y;
        }
        transplant(order, y);
        y->left = order->left;
        y->left->parent = y;
        y->is_red = order->is_red;
    }

    if (!y_original_color) {
        fixDelete(x);
    }

    if (best_order == nullptr && root != nullptr) {
        best_order = root;
        if (is_min_tree) {
            while (best_order->left != nullptr) best_order = best_order->left;
        } else {
            while (best_order->right != nullptr) best_order = best_order->right;
        }
    }
}
void OrderBookTree::fixDelete(Order*& node)
{
    while (order != root && (order == nullptr || order->is_red == false)) {
        if (order == order->parent->left) {
            Order* w = order->parent->right;
            if (w != nullptr) {
                if (w->is_red) {
                    w->is_red = false;
                    order->parent->is_red = true;
                    rotateLeft(order->parent);
                    w = order->parent->right;
                }

                if ((w->left == nullptr || w->left->is_red == false) &&
                    (w->right == nullptr || w->right->is_red == false)) {
                    w->is_red = true;
                    order = order->parent;
                }
                else if (w->right == nullptr || w->right->is_red == false) {
                    if (w->left != nullptr) w->left->is_red = false;
                    w->is_red = true;
                    rotateRight(w);
                    w = order->parent->right;
                }
                else {
                    w->is_red = order->parent->is_red;
                    order->parent->is_red = false;
                    if (w->right != nullptr) w->right->is_red = false;
                    rotateLeft(order->parent);
                    order = root;
                }
            }
        } else {
            Order* w = order->parent->left;
            if (w != nullptr) {
                if (w->is_red) {
                    w->is_red = false;
                    order->parent->is_red = true;
                    rotateRight(order->parent);
                    w = order->parent->left;
                }

                if ((w->right == nullptr || w->right->is_red == false) &&
                    (w->left == nullptr || w->left->is_red == false)) {
                    w->is_red = true;
                    order = order->parent;
                }
                else if (w->left == nullptr || w->left->is_red == false) {
                    if (w->right != nullptr) w->right->is_red = false;
                    w->is_red = true;
                    rotateLeft(w);
                    w = order->parent->left;
                }
                else {
                    w->is_red = order->parent->is_red;
                    order->parent->is_red = false;
                    if (w->left != nullptr) w->left->is_red = false;
                    rotateRight(order->parent);
                    order = root;
                }
            }
        }
    }
    if (order != nullptr) {
        order->is_red = false;
    }
}
Order* OrderBookTree::getBest() {
    return best_order;
}

