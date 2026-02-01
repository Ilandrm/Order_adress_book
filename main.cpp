#include "MatchingEngine.h"
#include <iostream>
#include <vector>
#include <random>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <cmath>

static std::vector<Order> generateOrders(std::size_t n)
{
    std::vector<Order> orders;
    orders.reserve(n);

    std::mt19937 rng(123456);
    std::uniform_int_distribution<int> sideDist(0, 1);
    std::uniform_int_distribution<std::int32_t> priceDist(9000, 11000);
    std::uniform_int_distribution<std::uint32_t> qtyDist(1, 500);

    std::uint64_t ts = 1;

    for (std::size_t i = 0; i < n; ++i) {
        Order o;
        o.id = (std::uint64_t)(i + 1);
        o.side = (sideDist(rng) == 0) ? Side::BUY : Side::SELL;
        o.price = priceDist(rng);
        o.quantity = qtyDist(rng);
        o.timestamp = ts++;
        orders.push_back(o);
    }

    return orders;
}

int main()
{
    const std::size_t N = 1000000;

    auto engine = new MatchingEngine();

    auto orders = generateOrders(N);

    std::vector<double> latencies_us;
    latencies_us.reserve(N);

    std::uint64_t totalExecuted = 0;

    auto t0 = std::chrono::high_resolution_clock::now();

    for (std::size_t i = 0; i < N; ++i) {
        const Order& o = orders[i];

        auto start = std::chrono::high_resolution_clock::now();
        totalExecuted += engine->processOrder(o);
        auto end = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double, std::micro> diff = end - start;
        latencies_us.push_back(diff.count()); // microsecondes
    }

    auto t1 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> sec = t1 - t0;

    std::sort(latencies_us.begin(), latencies_us.end());

    double p99_us = 0.0;
    if (!latencies_us.empty()) {
        const size_t n = latencies_us.size();
        double rank = 0.99 * static_cast<double>(n);
        size_t idx = static_cast<size_t>(std::ceil(rank));
        if (idx == 0) idx = 1;
        idx = idx - 1;
        if (idx >= n) idx = n - 1;
        p99_us = latencies_us[idx];
    }

    double avg_us = 0.0;
    if (!latencies_us.empty()) {
        double sum = std::accumulate(latencies_us.begin(), latencies_us.end(), 0.0);
        avg_us = sum / static_cast<double>(latencies_us.size());
    }

    std::cout << "Nb ordre mesurés: " << latencies_us.size() << std::endl;
    std::cout << "Total quantié executé: " << totalExecuted << std::endl;
    std::cout << "Temps total (sans warm-up): " << sec.count() << " s" << std::endl;
    std::cout << "Nb ordre par sec: " << (double)latencies_us.size() / sec.count() << " orders/s" << std::endl;
    std::cout << "avg latency: " << avg_us << " us" << std::endl;
    std::cout << "p99 latency: " << p99_us << " us" << std::endl;

    delete engine;
    return 0;
}