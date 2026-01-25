#include <chrono>
#include <iostream>

// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.
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
    // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.
}