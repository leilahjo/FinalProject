//
// Created by greg8 on 1/22/2026.
//

#ifndef ENGINE_RUNTIME_TIMING_H
#define ENGINE_RUNTIME_TIMING_H

#include "engine_core/EngineCore.h"

namespace EngineRuntime
{
    using namespace EngineCore;

    void initializeTiming(uint64_t targetFps);

    FrameData frameSync(bool pace);

    void stopTiming();
}

#endif //ENGINE_RUNTIME_TIMING_H
