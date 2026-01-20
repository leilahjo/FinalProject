#include <iostream>
#include <chrono>
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

void set_current_thread_affinity(int core_id);
void set_max_thread_priority();
void game_sleep(int millis);
void enable_high_precision_sleep();
void doWork(long long durationNs);

int main()
{
    using clock = std::chrono::steady_clock;

    enable_high_precision_sleep();
    // set_current_thread_affinity(0); // Generally not a good idea.
    set_max_thread_priority();
    constexpr long long targetFps = 60;
    constexpr long long targetFrameDurationNs = 1'000'000'000 / targetFps;
    constexpr int totalFrames = targetFps * 3;

    auto startTime = clock::now();
    auto lastFrameStart = startTime;
    int frame = 0;

    while (frame < totalFrames)
    {
        auto frameStart = clock::now();

        doWork(2'000'000); // Simulate some work.

        long long frameDurationNs = std::chrono::duration_cast<std::chrono::nanoseconds>(frameStart - lastFrameStart).count();
        long long jitterNs = frameDurationNs - targetFrameDurationNs;
        lastFrameStart = frameStart;

        std::cout << "Frame " << frame++ << " | jitter: " << jitterNs << "ns\n";

        auto now = clock::now();
        long long elapsedFrameNs = std::chrono::duration_cast<std::chrono::nanoseconds>(now - frameStart).count();
        long long remainingNs = targetFrameDurationNs - elapsedFrameNs;

        while (remainingNs > 3'000'000) // 3ms
        {
            game_sleep(1);
            auto sleepStarTime = now;
            now = clock::now();
            auto sleepDurationNs = std::chrono::duration_cast<std::chrono::nanoseconds>(now - sleepStarTime).count();
            // std::cout << "sleepDurationNs = " << sleepDurationNs << "ns\n";
            elapsedFrameNs = std::chrono::duration_cast<std::chrono::nanoseconds>(now - frameStart).count();
            remainingNs = targetFrameDurationNs - elapsedFrameNs;
        }

        while (true)
        {
            now = clock::now();
            long long waitNs = std::chrono::duration_cast<std::chrono::nanoseconds>(now - frameStart).count();
            if (waitNs >= targetFrameDurationNs)
                break;
        }
    }

    auto endTime = clock::now();
    long long actualTotalNs = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime).count();
    long long expectedTotalNs = totalFrames * targetFrameDurationNs;
    double driftRate = (static_cast<double>(actualTotalNs) / expectedTotalNs);

    std::cout << "Drift rate: " << driftRate << "\n";
    std::cout << "Press Enter to exit...";
    std::cin.get();
    return 0;
}

void doWork(long long durationNs)
{
    using clock = std::chrono::steady_clock;

    auto startTime = clock::now();
    while (true)
    {
        auto now = clock::now();
        long long waitNs = std::chrono::duration_cast<std::chrono::nanoseconds>(now - startTime).count();
        if (waitNs >= durationNs)
            break;
    }
}

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
        (thread_policy_t)&policy,
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