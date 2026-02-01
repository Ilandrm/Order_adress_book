#ifndef MODELE_H
#define MODELE_H
#include <cstdint>

enum class Side : std::uint8_t { BUY, SELL };
struct Order {
    std::uint64_t id;
    Side side;
    std::int32_t price;
    std::uint32_t quantity;
    std::uint64_t timestamp;

    Order* left;
    Order* right;
    Order* parent;
    bool is_red;
    Order()
        : id(0), side(Side::BUY), price(0), quantity(0), timestamp(0),
          left(nullptr), right(nullptr), parent(nullptr), is_red(false) {}
};
#endif
