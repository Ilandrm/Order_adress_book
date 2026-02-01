#include "OrderPool.h"

Order* OrderPool::acquire()
{
    if (free_count > 0) {
        Order* o = free_list[--free_count];
        *o = Order();
        return o;
    }

    if (next_free >= CAP) return nullptr;

    Order* o = &storage[next_free++];
    *o = Order();
    return o;
}
// dest trop chaud sah 
void OrderPool::release(Order* o)
{
    if (!o) return;
    if (free_count < CAP) free_list[free_count++] = o;
}
