//
// Created by greg8 on 11/19/2025.
//

#ifndef ENGINE_CORE_ANIMATION_H
#define ENGINE_CORE_ANIMATION_H

#include <cstdint>

#include "ECSTypes.h"

namespace EngineCore
{
    struct AnimationData
    {
        enum State : uint8_t
        {
            PLAYING, PAUSED
        };

        AnimationId animationId = INVALID_ANIMATION_ID;
        float progress = 0.0f; // [0, 1].
        float speed = 1.0f;
        State state = PAUSED;
    };
}

#endif //ENGINE_CORE_ANIMATION_H
