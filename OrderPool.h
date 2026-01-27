#include <array>
#include <stack>
#include "modele.h"

class OrderPool {
private:
    static constexpr size_t ORDER_POOL_SIZE = 1'000'000;
    std::array<Order, ORDER_POOL_SIZE> pool_;
    std::stack<size_t> free_indices_;

public:
    OrderPool();
    Order* acquire();
    void release(Order* order);
};