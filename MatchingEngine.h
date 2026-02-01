#pragma once

#include <cstdint>
#include <unordered_map>
#include "modele.h"
#include "OrderBookTree.h"
#include "OrderPool.h"

class MatchingEngine {
private:
    OrderPool pool_;
    OrderBookTree buy_tree_;
    OrderBookTree sell_tree_;
    std::unordered_map<uint64_t, Order*> index_;

    bool crosses(const Order* incoming, const Order* bestOpp) const;

public:
    MatchingEngine();

    uint32_t processOrder(const Order& incoming);
    bool cancelOrder(uint64_t orderId);
};
