#include <chrono>
#include <iostream>

int main()
{
    using clock = std::chrono::steady_clock;

    auto start = clock::now();

    // Consider: how long does the loop itself take?
    for (int i = 0; i < 1'000'000; i++)
    {
        clock::now();
    }

    auto end = clock::now();

    std::cout << "Duration (ns): " << (end - start) / 1'000'000 << std::endl;

    return 0;
}
