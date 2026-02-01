#include "MatchingEngine.h"
#include <algorithm>

MatchingEngine::MatchingEngine()
    : buy_tree_(false), sell_tree_(true) {}

bool MatchingEngine::crosses(const Order* incoming, const Order* bestOpp) const
{
    if (incoming->side == Side::BUY) return incoming->price >= bestOpp->price;
    return incoming->price <= bestOpp->price;
}

uint32_t MatchingEngine::processOrder(const Order& incoming)
{
    Order* inc = pool_.acquire();
    if (!inc) return 0;

    *inc = incoming;
    inc->left = nullptr;
    inc->right = nullptr;
    inc->parent = nullptr;
    inc->is_red = false;

    uint32_t executed = 0;

    OrderBookTree* opp_tree = (inc->side == Side::BUY) ? &sell_tree_ : &buy_tree_;
    OrderBookTree* own_tree = (inc->side == Side::BUY) ? &buy_tree_ : &sell_tree_;

    while (inc->quantity > 0 && !opp_tree->isEmpty()) {
        Order* best = opp_tree->getBest();
        if (!best) break;
        if (!crosses(inc, best)) break;

        uint32_t tradeQty = (std::min)(inc->quantity, best->quantity);
        inc->quantity -= tradeQty;
        best->quantity -= tradeQty;
        executed += tradeQty;

        if (best->quantity == 0) {
            index_.erase(best->id);
            opp_tree->remove(best);
            pool_.release(best);
        }
    }

    if (inc->quantity > 0) {
        own_tree->insert(inc);
        index_[inc->id] = inc;
        return executed;
    }

    pool_.release(inc);
    return executed;
}

bool MatchingEngine::cancelOrder(uint64_t orderId)
{
    auto it = index_.find(orderId);
    if (it == index_.end()) return false;

    Order* ord = it->second;
    index_.erase(it);

    if (ord->side == Side::BUY) buy_tree_.remove(ord);
    else sell_tree_.remove(ord);

    pool_.release(ord);
    return true;
}
