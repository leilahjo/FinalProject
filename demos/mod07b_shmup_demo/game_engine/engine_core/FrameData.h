//
// Created by greg8 on 2/21/2026.
//

#ifndef ENGINE_CORE_FRAMEDATA_H
#define ENGINE_CORE_FRAMEDATA_H

#include <cstdint>

namespace EngineCore
{
    struct FrameData
    {
        uint64_t frame;

        // Diagnostic
        int64_t jitterUs;
        float fps;
        uint64_t workDurationUs;
    };
}

#endif //ENGINE_CORE_FRAMEDATA_H
