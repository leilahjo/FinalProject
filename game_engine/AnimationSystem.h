//
// Created by greg8 on 11/19/2025.
//

#ifndef GAME_ENGINE_ANIMATIONMANAGER_H
#define GAME_ENGINE_ANIMATIONMANAGER_H
#include <cstdint>
#include <vector>

#include "Sprite.h"
#include "ECSTypes.h"
#include "World.h"

namespace GameEngine
{
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
        AnimationId createAnimation(AnimationPlaybackMode, float defaultDurationS, uint16_t startFrameIndex = 0, uint16_t frameCount = 0);
        static void startAnimation(World& world, EntityId entityId, AnimationId animationId, float speed = 1);
        Animation* getAnimation(AnimationId animationId);

        void update(World& world, float dt);
    };
}

#endif //GAME_ENGINE_ANIMATIONMANAGER_H
