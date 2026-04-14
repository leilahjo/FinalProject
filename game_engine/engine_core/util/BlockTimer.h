#ifndef GAME_ENGINE_BLOCKTIMER_H
#define GAME_ENGINE_BLOCKTIMER_H

#include <string>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <cstdint>

namespace EngineCore
{
    class BlockTimer
    {
    public:
        struct BlockData
        {
            uint64_t currentFrameUs = 0;
            uint64_t totalUs = 0;
        };

        explicit BlockTimer(std::string name);

        void startBlock(const std::string& name);
        void endFrame();
        void printStats();

    private:
        using clock = std::chrono::steady_clock;
        using TimePoint = std::chrono::time_point<clock>;

        uint32_t frameCount = 0;
        std::string timerName;
        std::unordered_map<std::string, BlockData> blocks;
        std::vector<std::string> blockOrder;
        std::string activeBlock;
        TimePoint startTime;

        void finalizeActiveBlock(TimePoint endTime);
    };
}

#endif //GAME_ENGINE_BLOCKTIMER_H
