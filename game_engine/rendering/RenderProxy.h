//
// Created by greg8 on 3/12/2026.
//

#ifndef GAME_ENGINE_RENDERPROXY_H
#define GAME_ENGINE_RENDERPROXY_H

#include "engine_core/EngineCore.h"

namespace Rendering
{
    using namespace EngineCore;

    struct RenderProxy
    {
        // Position and size in world units
        float x, y;
        float width, height;

        // Color (frameless entities)
        bool      hasColor;
        GameColor color;

        // Sprite / animation
        bool     hasSprite;
        SpriteId spriteId;
        bool     hasAnimationFrameIndex;
        uint16_t animationFrameIndex;

        // Type info for layer sorting
        bool         hasType;
        EntityTypeId typeId;

        // Rotation in degrees (used by DrawTexturePro / DrawRectanglePro)
        bool  hasRotation;
        float rotation;

        // Velocity — used by debug rendering to draw direction arrows
        bool  hasVelocity;
        float vx, vy;

        // Collider info — used by debug rendering to draw bounding shapes
        bool          hasCollider;
        ColliderShape colliderShape;

        // HP — used by debug rendering to draw health bars
        bool  hasHp;
        float currentHp;
        float maxHp;

        // Particle — tint alpha is already baked into color; no extra data needed

        size_t index;       // original insertion order (tie-breaker in sort)
        uint8_t renderLayer;
    };
}

#endif //GAME_ENGINE_RENDERPROXY_H
