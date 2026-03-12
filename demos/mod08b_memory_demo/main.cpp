#include <iostream>
#include <chrono>

#include "ArenaAllocator.h"

int main()
{
    using clock = std::chrono::steady_clock;

    static constexpr auto ct = 1'000'000;
    int** temp = new int*[ct];
    auto start1 = clock::now();
    for (int i = 0; i < ct; i++)
        temp[i] = new int;
    for (int i = 0; i < ct; i++)
        delete temp[i];
    auto end1 = clock::now();
    delete[] temp;

    EngineCore::ArenaAllocator arenaAllocator = EngineCore::ArenaAllocator(100 * 1024 * 1024);
    auto start2 = clock::now();
    for (int i = 0; i < ct; i++)
        arenaAllocator.allocate<int>(1);
    arenaAllocator.reset();
    auto end2 = clock::now();

    std::cout << "time1: " << (end1 - start1) / static_cast<double>(ct) << std::endl;
    std::cout << "time2: " << (end2 - start2) / static_cast<double>(ct) << std::endl;
}
