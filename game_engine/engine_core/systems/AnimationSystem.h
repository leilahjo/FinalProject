//
// Created by greg8 on 11/19/2025.
//

#ifndef ENGINE_CORE_ANIMATIONMANAGER_H
#define ENGINE_CORE_ANIMATIONMANAGER_H

#include <cstdint>
#include <vector>

#include "../ecs/ECSTypes.h"

namespace EngineCore
{
    struct World;

    enum AnimationPlaybackMode : uint8_t
    {
        LOOP,
        STOP_AT_BEGINNING,
        STOP_AT_END,
    };

    struct Animation
    {
        AnimationPlaybackMode playbackMode;
        float defaultDurationS;

        const uint16_t startFrameIndex;
        const uint16_t frameCount;
        uint16_t getCurrentFrameIndex(float progress);
    };

    class AnimationSystem
    {
        std::vector<Animation> animations;

    public:
        AnimationId createAnimation(AnimationPlaybackMode,
                                    float defaultDurationS,
                                    uint16_t startFrameIndex = 0,
                                    uint16_t frameCount = 0);
        static void startAnimation(World& world, EntityId entityId, AnimationId animationId, float speed = 1);
        Animation* getAnimation(AnimationId animationId);

        void update(World& world);
    };
}

#endif //ENGINE_CORE_ANIMATIONMANAGER_H
