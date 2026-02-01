#include <vector>
#include <random>
#include <cstdint>
#include "modele.h"

std::vector<Order> generateOrders(std::size_t n = 1'000'000)
{
    std::vector<Order> orders;
    orders.reserve(n);

    std::mt19937 rng(123456);

    std::uniform_int_distribution<int> sideDist(0, 1);
    std::uniform_int_distribution<int32_t> priceDist(9000, 11000);
    std::uniform_int_distribution<uint32_t> qtyDist(1, 500);

    std::uint64_t ts = 1;

    for (std::size_t i = 0; i < n; ++i) {
        Order o;
        o.id = static_cast<std::uint64_t>(i + 1);
        o.side = sideDist(rng) == 0 ? Side::BUY : Side::SELL;
        o.price = priceDist(rng);
        o.quantity = qtyDist(rng);
        o.timestamp = ts++;
        o.left = o.right = o.parent = nullptr;
        o.is_red = false;

        orders.push_back(o);
    }

    return orders;
}
