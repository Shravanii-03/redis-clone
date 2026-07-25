#include <chrono>
#include <iostream>

#include "DataStore.h"

int main()
{
    DataStore store;

    const int OPERATIONS = 100000;

    //---------------- SET Benchmark ----------------

    auto start =
        std::chrono::high_resolution_clock::now();

    for(int i = 0; i < OPERATIONS; i++)
    {
        store.set(
            "key" + std::to_string(i),
            "value");
    }

    auto end =
        std::chrono::high_resolution_clock::now();

    auto setTime =
        std::chrono::duration_cast<
            std::chrono::milliseconds>(
                end - start);

    //---------------- GET Benchmark ----------------

    start =
        std::chrono::high_resolution_clock::now();

    for(int i = 0; i < OPERATIONS; i++)
    {
        store.get(
            "key" + std::to_string(i));
    }

    end =
        std::chrono::high_resolution_clock::now();

    auto getTime =
        std::chrono::duration_cast<
            std::chrono::milliseconds>(
                end - start);

    //---------------- Results ----------------

    std::cout << "\n=============================\n";
    std::cout << "MiniRedis Benchmark\n";
    std::cout << "=============================\n\n";

    std::cout << "SET Operations : "
              << OPERATIONS
              << std::endl;

    std::cout << "SET Time       : "
              << setTime.count()
              << " ms\n\n";

    std::cout << "GET Operations : "
              << OPERATIONS
              << std::endl;

    std::cout << "GET Time       : "
              << getTime.count()
              << " ms\n\n";

    double totalSeconds =
        (setTime.count() + getTime.count()) / 1000.0;

    double throughput =
        (OPERATIONS * 2) / totalSeconds;

    std::cout << "Throughput     : "
              << throughput
              << " ops/sec\n";

    std::cout << "=============================\n";
}