#include "BlockTimer.h"
#include <iostream>
#include <iomanip>

namespace EngineCore
{
    struct comma_separator : std::numpunct<char>
    {
        char do_thousands_sep() const override { return ','; }
        std::string do_grouping() const override { return "\3"; } // Groups of 3 digits
    };

    BlockTimer::BlockTimer(std::string name) : timerName(std::move(name))
    {
    }

    void BlockTimer::startBlock(const std::string& name)
    {
        auto now = clock::now();
        finalizeActiveBlock(now);

        if (blocks.find(name) == blocks.end())
        {
            blocks[name] = BlockData{};
            blockOrder.push_back(name);
        }

        activeBlock = name;
        startTime = now;
    }

    void BlockTimer::endFrame()
    {
        auto now = clock::now();
        finalizeActiveBlock(now);

        for (auto& [name, data] : blocks)
            data.totalUs += data.currentFrameUs;
        activeBlock = "";
        frameCount++;
    }

    void BlockTimer::printStats()
    {
        std::locale previousLocale = std::cout.getloc();
        std::cout.imbue(std::locale(previousLocale, new comma_separator));
        std::cout << "\n--- Profiler: " << timerName << " (" << frameCount << " frames) ---\n";

        size_t width = 0;
        for (const auto& name : blockOrder)
            if (name.length() > width)
                width = name.length();
        width += 5;

        for (const auto& name : blockOrder)
        {
            const auto& data = blocks[name];
            uint64_t average = frameCount > 0 ? data.totalUs / frameCount : 0;

            std::cout << std::left << std::setw(width) << ("[" + name + "] : ")
                << std::right << std::setw(8) << average << " us\n";
        }
    }

    void BlockTimer::finalizeActiveBlock(TimePoint endTime)
    {
        if (!activeBlock.empty())
        {
            auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
            blocks[activeBlock].currentFrameUs = static_cast<uint64_t>(elapsed.count());
        }
    }
}
