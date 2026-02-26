//
// Created by greg8 on 11/19/2025.
//

#include "AnimationSystem.h"
#include "World.h"

namespace GameEngine
{
    uint16_t Animation::getCurrentFrameIndex(float progress)
    {
        auto frameOffset = static_cast<uint16_t>(progress * frameCount);
        if (frameOffset > frameCount - 1)
            frameOffset = frameCount - 1;
        return startFrameIndex + frameOffset;
    }

    AnimationId AnimationSystem::createAnimation(
        AnimationPlaybackMode playbackMode, float defaultDurationS, uint16_t startFrameIndex, uint16_t frameCount)
    {
        animations.push_back(Animation{playbackMode, defaultDurationS, startFrameIndex, frameCount});
        return animations.size() - 1;
    }

    void AnimationSystem::startAnimation(World& world, EntityId entityId, AnimationId animationId,
                                         float speed)
    {
        auto optEntity = world.findEntity(entityId);
        if (!optEntity)
            return;
        auto& entity = optEntity.value();
        if (!entity.hasAnimation())
            return;

        auto& animation = entity.animation();
        animation.animationId = animationId;
        animation.speed = speed;
        animation.progress = 0;
        animation.state = AnimationData::PLAYING;
    }

    Animation* AnimationSystem::getAnimation(AnimationId animationId)
    {
        return &animations[animationId];
    }

    void AnimationSystem::update(World& world, float frameDt)
    {
        world.forEach(Archetype::COMP_ANIMATION, [&](Entity entity)
        {
            auto& entityAnimation = entity.animation();
            if (entityAnimation.state == AnimationData::PAUSED)
                return;

            Animation& animationDefinition = animations[entity.animation().animationId];
            entityAnimation.progress +=
                entityAnimation.speed * frameDt / animationDefinition.defaultDurationS;

            if (entityAnimation.progress >= 1.0f)
            {
                switch (animationDefinition.playbackMode)
                {
                case STOP_AT_BEGINNING:
                    entityAnimation.progress = 0;
                    entityAnimation.state = AnimationData::State::PAUSED;
                    break;
                case STOP_AT_END:
                    entityAnimation.progress = 1;
                    entityAnimation.state = AnimationData::State::PAUSED;
                    break;
                case LOOP:
                    entityAnimation.progress =
                        entityAnimation.progress - static_cast<int>(entityAnimation.progress);
                    break;
                }
            }
        });
    }
}
