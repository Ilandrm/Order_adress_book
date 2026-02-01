#pragma once
#include <array>
#include <cstddef>
#include "modele.h"

class OrderPool {
private:
    static constexpr std::size_t CAP = 1000000;
    std::array<Order, CAP> storage;
    std::array<Order*, CAP> free_list;
    std::size_t next_free;
    std::size_t free_count;
public:
    OrderPool() : next_free(0), free_count(0) {}
    Order* acquire();
    void release(Order* o);
};
