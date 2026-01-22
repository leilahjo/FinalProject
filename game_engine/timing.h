//
// Created by greg8 on 1/22/2026.
//

#ifndef GAME_ENGINE_TIMING_H
#define GAME_ENGINE_TIMING_H
#include <cstdint>

struct FrameData
{
    uint64_t frame;

    // Diagnostic
    uint64_t jitterUs;
    float fps;
    uint64_t workDurationUs;
};

void initializeTiming();

FrameData frameSync();

void stopTiming();

#endif //GAME_ENGINE_TIMING_H