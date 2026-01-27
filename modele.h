#include <cstdint>

enum class Side : uint8_t {
    BUY,
    SELL
};

struct Order {
    uint64_t id;
    Side side;
    int32_t price;
    uint32_t quantity;
    uint64_t timestamp;

    Order* left;
    Order* right;
    Order* parent;
    bool is_red;

    Order() : id(0), side(Side::BUY), price(0), quantity(0), timestamp(0),
              left(nullptr), right(nullptr), parent(nullptr), is_red(false) {}
};