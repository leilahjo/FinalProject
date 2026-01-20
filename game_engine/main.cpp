

#include <chrono>
#include <iostream>
#include <windows.h>

int main()
{
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
    //SetThreadAffinityMask(GetCurrentThread(), 1ull << 4);
    using clock = std::chrono::steady_clock;

    timeBeginPeriod(1);

    std::cout.imbue(std::locale(""));

    constexpr long long frameRate = 60;
    constexpr long long totalFrames = 240;
    constexpr long long targetFrameDurationNs = 1'000'000'000 / frameRate;
    auto lastFrameStart = clock::now();

    int frame = 0;
    while (frame < totalFrames) {
        auto frameStart = clock::now();

        //166666700
        //166666666 = 34
        //166666600
        long long frameDurationNs = std::chrono::duration_cast<std::chrono::nanoseconds>(frameStart - lastFrameStart).count();
        long long jitterNs = frameDurationNs - targetFrameDurationNs;
        std::cout << "Frame " << frame++ << " | jitter: " << jitterNs << "ns\n";


        auto now = clock::now();
        long long elapsedFrameNs =
            std::chrono::duration_cast<std::chrono::nanoseconds>(now - frameStart).count();
        long long remainingNs = targetFrameDurationNs - elapsedFrameNs;

        while ( remainingNs > 3'000'000) {
            Sleep(1 );
            auto sleepStarTime = now;
            now = clock::now();
            auto sleepDurationNs = std::chrono::duration_cast<std::chrono::nanoseconds>(now - sleepStarTime).count();
            //std::cout << "sleepDurationNs = " << sleepDurationNs << "ns\n";
            elapsedFrameNs = std::chrono::duration_cast<std::chrono::nanoseconds>(now - frameStart).count();
            remainingNs = targetFrameDurationNs - elapsedFrameNs;
        }

        while (true) {
            auto now = clock::now();
            long long waitNs = std::chrono::duration_cast<std::chrono::nanoseconds>(now - frameStart).count();
            if (waitNs >= targetFrameDurationNs)
                break;
        }

        lastFrameStart = frameStart;
    }

    return 0;
}
