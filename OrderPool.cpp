#include "OrderPool.h"

OrderPool::OrderPool() {
    for (size_t i = 0; i < ORDER_POOL_SIZE; ++i) {
        free_indices_.push(i);
    }
}

Order* OrderPool::acquire() {
    if (free_indices_.empty()) {
        return nullptr;
    }
    size_t index = free_indices_.top();
    free_indices_.pop();
    return &pool_[index];
}

void OrderPool::release(Order* order) {
    size_t index = order - pool_.data();
    free_indices_.push(index);
}