//
// Created by greg8 on 1/27/2026.
//

#ifndef ENGINE_CORE_ENTITY_H
#define ENGINE_CORE_ENTITY_H

#include <cstdint>

#include "AnimationData.h"
#include "GameColor.h"
#include "ECSTypes.h"

namespace EngineCore
{
    struct Archetype;

    // Represents an entity in the game world. The struct itself is a lightweight view
    // into the arrays of an archetype.
    struct Entity
    {
        using StateMask = std::uint32_t;

        enum State : StateMask
        {
            STATE_DEFAULT = 0,
            STATE_DESTROYED = 1 << 0,
        };

        EntityLocation entityLocation;

        EntityId& id();

        // Position
        float& x();
        float& y();

        // Velocity
        float& vx();
        float& vy();

        // Size
        float& width();
        float& height();

        GameColor& color();

        State& state();

        ColliderShape& colliderShape();
        ColliderLayerId& colliderLayerId();

        AnimationData& animation();

        SpriteId& spriteId();

        EntityTypeId& entityTypeId();

        float left();
        float right();
        float top();
        float bottom();

        bool operator==(const Entity& entity) const = default;

        bool hasAnimation();
        bool hasSprite();
        bool hasColor();
        bool hasEntityType();
    };

    Entity::State operator|(Entity::State lhs, Entity::State rhs);
    Entity::State& operator|=(Entity::State& lhs, Entity::State rhs);
}

#endif //ENGINE_CORE_ENTITY_H
