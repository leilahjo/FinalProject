#include <iostream>
#include <chrono>

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

    std::cout << "time1: " << (end1 - start1) / static_cast<double>(ct) << std::endl;
}
