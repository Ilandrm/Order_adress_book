#include "OrderBookTree.h"

OrderBookTree::OrderBookTree(bool isMinTree)
{
    root = nullptr;
    best_order = nullptr;
    is_min_tree = isMinTree;
}

void OrderBookTree::rotateLeft(Order* node)
{
    Order* y = node->right;
    node->right = y->left;
    if (y->left != nullptr) y->left->parent = node;

    y->parent = node->parent;

    if (node->parent == nullptr) root = y;
    else if (node == node->parent->left) node->parent->left = y;
    else node->parent->right = y;

    y->left = node;
    node->parent = y;
}

void OrderBookTree::rotateRight(Order* node)
{
    Order* y = node->left;
    node->left = y->right;
    if (y->right != nullptr) y->right->parent = node;

    y->parent = node->parent;

    if (node->parent == nullptr) root = y;
    else if (node == node->parent->right) node->parent->right = y;
    else node->parent->left = y;

    y->right = node;
    node->parent = y;
}

void OrderBookTree::fixInsert(Order* node)
{
    while (node->parent != nullptr && node->parent->is_red) {
        Order* p = node->parent;
        Order* g = p->parent;
        if (g == nullptr) break;

        if (p == g->left) {
            Order* u = g->right;

            if (u != nullptr && u->is_red) {
                p->is_red = false;
                u->is_red = false;
                g->is_red = true;
                node = g;
            } else {
                if (node == p->right) {
                    node = p;
                    rotateLeft(node);
                    p = node->parent;
                    g = p ? p->parent : nullptr;
                    if (!g) break;
                }
                p->is_red = false;
                g->is_red = true;
                rotateRight(g);
            }
        } else {
            Order* u = g->left;

            if (u != nullptr && u->is_red) {
                p->is_red = false;
                u->is_red = false;
                g->is_red = true;
                node = g;
            } else {
                if (node == p->left) {
                    node = p;
                    rotateRight(node);
                    p = node->parent;
                    g = p ? p->parent : nullptr;
                    if (!g) break;
                }
                p->is_red = false;
                g->is_red = true;
                rotateLeft(g);
            }
        }
    }

    if (root != nullptr) root->is_red = false;
}

void OrderBookTree::transplant(Order* u, Order* v)
{
    if (u->parent == nullptr) root = v;
    else if (u == u->parent->left) u->parent->left = v;
    else u->parent->right = v;

    if (v != nullptr) v->parent = u->parent;
}

Order* OrderBookTree::minimum(Order* node)
{
    while (node != nullptr && node->left != nullptr) node = node->left;
    return node;
}

void OrderBookTree::insert(Order* newOrder)
{
    Order* parent = nullptr;
    Order* cur = root;

    while (cur != nullptr) {
        parent = cur;
        if (newOrder->price < cur->price) cur = cur->left;
        else cur = cur->right;
    }

    newOrder->parent = parent;
    newOrder->left = nullptr;
    newOrder->right = nullptr;
    newOrder->is_red = true;

    if (parent == nullptr) root = newOrder;
    else if (newOrder->price < parent->price) parent->left = newOrder;
    else parent->right = newOrder;

    fixInsert(newOrder);

    if (best_order == nullptr) best_order = newOrder;
    else {
        if (is_min_tree) {
            if (newOrder->price < best_order->price) best_order = newOrder;
        } else {
            if (newOrder->price > best_order->price) best_order = newOrder;
        }
    }
}

void OrderBookTree::fixDelete(Order* x, Order* parent)
{
    while (x != root && (x == nullptr || x->is_red == false)) {

        if (parent == nullptr) break;

        if (x == parent->left) {
            Order* w = parent->right;

            if (w != nullptr && w->is_red) {
                w->is_red = false;
                parent->is_red = true;
                rotateLeft(parent);
                w = parent->right;
            }

            bool wLeftBlack  = (w == nullptr || w->left == nullptr || w->left->is_red == false);
            bool wRightBlack = (w == nullptr || w->right == nullptr || w->right->is_red == false);

            if (wLeftBlack && wRightBlack) {
                if (w != nullptr) w->is_red = true;
                x = parent;
                parent = x->parent;
            } else {
                if (w != nullptr && (w->right == nullptr || w->right->is_red == false)) {
                    if (w->left != nullptr) w->left->is_red = false;
                    w->is_red = true;
                    rotateRight(w);
                    w = parent->right;
                }

                if (w != nullptr) {
                    w->is_red = parent->is_red;
                    if (w->right != nullptr) w->right->is_red = false;
                }
                parent->is_red = false;
                rotateLeft(parent);
                x = root;
            }
        } else {
            Order* w = parent->left;

            if (w != nullptr && w->is_red) {
                w->is_red = false;
                parent->is_red = true;
                rotateRight(parent);
                w = parent->left;
            }

            bool wRightBlack = (w == nullptr || w->right == nullptr || w->right->is_red == false);
            bool wLeftBlack  = (w == nullptr || w->left == nullptr || w->left->is_red == false);

            if (wRightBlack && wLeftBlack) {
                if (w != nullptr) w->is_red = true;
                x = parent;
                parent = x->parent;
            } else {
                if (w != nullptr && (w->left == nullptr || w->left->is_red == false)) {
                    if (w->right != nullptr) w->right->is_red = false;
                    w->is_red = true;
                    rotateLeft(w);
                    w = parent->left;
                }

                if (w != nullptr) {
                    w->is_red = parent->is_red;
                    if (w->left != nullptr) w->left->is_red = false;
                }
                parent->is_red = false;
                rotateRight(parent);
                x = root;
            }
        }
    }

    if (x != nullptr) x->is_red = false;
}

void OrderBookTree::remove(Order* order)
{
    if (order == nullptr) return;
    if (order == best_order) best_order = nullptr;

    Order* y = order;
    bool yOriginalRed = y->is_red;

    Order* x = nullptr;
    Order* xParent = nullptr;

    if (order->left == nullptr) {
        x = order->right;
        xParent = order->parent;
        transplant(order, order->right);
    } else if (order->right == nullptr) {
        x = order->left;
        xParent = order->parent;
        transplant(order, order->left);
    } else {
        y = minimum(order->right);
        yOriginalRed = y->is_red;
        x = y->right;

        if (y->parent == order) {
            xParent = y;
        } else {
            xParent = y->parent;
            transplant(y, y->right);
            y->right = order->right;
            if (y->right) y->right->parent = y;
        }

        transplant(order, y);
        y->left = order->left;
        if (y->left) y->left->parent = y;
        y->is_red = order->is_red;
    }

    if (yOriginalRed == false) {
        fixDelete(x, xParent);
    }

    // recalcul best_order simple
    if (root == nullptr) {
        best_order = nullptr;
    } else if (best_order == nullptr) {
        best_order = root;
        if (is_min_tree) {
            while (best_order->left) best_order = best_order->left;
        } else {
            while (best_order->right) best_order = best_order->right;
        }
    }
}

Order* OrderBookTree::getBest()
{
    return best_order;
}
