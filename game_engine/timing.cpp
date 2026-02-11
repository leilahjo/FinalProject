//
// Created by greg8 on 1/21/2026.
//

#include "timing.h"

#include <chrono>
#include <iostream>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#else
#include <time.h>
#endif

#ifdef __APPLE__
#include <pthread.h>
#include <mach/thread_policy.h>
#include <mach/mach.h>
#endif

namespace GameEngine
{
    using Timestamp = std::chrono::time_point<std::chrono::steady_clock>;

    void set_current_thread_affinity(int core_id);
    void set_max_thread_priority();
    void game_sleep(int millis);
    void enable_high_precision_sleep();
    Timestamp get_oldest_frame_start_time();

    Timestamp startTime;
    Timestamp frameStart;
    Timestamp lastFrameStart;

    constexpr long long targetFps = 60;
    constexpr long long targetFrameDurationNs = 1'000'000'000 / targetFps;
    constexpr long long targetFrameDurationUs = targetFrameDurationNs / 1'000;

    FrameData frameData = {0, 0};

    Timestamp frameStartHistory[targetFps];
    int frameStartHistoryIndex = 0;

    void initializeTiming()
    {
        using clock = std::chrono::steady_clock;

        enable_high_precision_sleep();
        // set_current_thread_affinity(0); // Generally not a good idea.
        set_max_thread_priority();

        startTime = clock::now();
        frameStart = startTime;
        lastFrameStart = frameStart - std::chrono::nanoseconds(targetFrameDurationNs);
    }

    FrameData frameSync()
    {
        using clock = std::chrono::steady_clock;
        auto now = clock::now();
        frameData.workDurationUs = std::chrono::duration_cast<std::chrono::microseconds>(now - frameStart).count();

        long long frameDurationUs = std::chrono::duration_cast<std::chrono::microseconds>(frameStart - lastFrameStart).
            count();
        lastFrameStart = frameStart;

        frameData.jitterUs = frameDurationUs - targetFrameDurationUs;
        frameData.frame++;

        std::chrono::duration<double> duration = frameStart - frameStartHistory[frameData.frame % targetFps];
        frameData.fps = targetFps / duration.count();
        frameStartHistory[frameData.frame % targetFps] = frameStart;

        now = clock::now();
        long long elapsedFrameNs = std::chrono::duration_cast<std::chrono::nanoseconds>(now - frameStart).count();
        long long remainingNs = targetFrameDurationNs - elapsedFrameNs;

        while (remainingNs > 3'000'000) // 3ms
        {
            game_sleep(1);
            now = clock::now();
            elapsedFrameNs = std::chrono::duration_cast<std::chrono::nanoseconds>(now - frameStart).count();
            remainingNs = targetFrameDurationNs - elapsedFrameNs;
        }

        while (true)
        {
            now = clock::now();
            elapsedFrameNs = std::chrono::duration_cast<std::chrono::nanoseconds>(now - frameStart).count();
            if (elapsedFrameNs >= targetFrameDurationNs)
                break;
        }

        frameStart = now;
        return frameData;
    }

    void stopTiming()
    {
        using clock = std::chrono::steady_clock;

        auto endTime = clock::now();
        long long actualTotalNs = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
        long long expectedTotalNs = frameData.frame * targetFrameDurationNs;
        double driftRate = (static_cast<double>(actualTotalNs) / expectedTotalNs);

        std::cout << "Drift rate: " << driftRate << "\n";
    }

    Timestamp get_oldest_frame_start_time()
    {
        int oldestIndex = frameStartHistoryIndex + 1;
        if (oldestIndex >= targetFps)
            oldestIndex = 0;
        return frameStartHistory[oldestIndex];
    }


    //Consider using timer constructs, for example SetWaitableTimer on Windows or mach_wait_until on Mac.
    void game_sleep(int millis)
    {
#ifdef _WIN32
        Sleep(millis);
#else
        std::this_thread::sleep_for(std::chrono::milliseconds(millis));
#endif
    }

    void set_current_thread_affinity(int core_id)
    {
#ifdef _WIN32
        SetThreadAffinityMask(GetCurrentThread(), 1ull << core_id);
#elif __APPLE__
        thread_affinity_policy_data_t policy = {core_id + 1}; // group ID must be non-zero
        thread_policy_set(
            mach_thread_self(),
            THREAD_AFFINITY_POLICY,
            (thread_policy_t) & policy,
            1);
#endif
    }

    void set_max_thread_priority()
    {
#ifdef _WIN32
        SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
#elif __APPLE__
        pthread_set_qos_class_self_np(QOS_CLASS_USER_INTERACTIVE, 0);
#endif
    }

    void enable_high_precision_sleep()
    {
#ifdef _WIN32
        timeBeginPeriod(1);
#endif
        // mac already has a high precision sleep.
    }
}
