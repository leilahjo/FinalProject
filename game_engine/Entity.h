//
// Created by greg8 on 1/27/2026.
//

#ifndef GAME_ENGINE_ENTITY_H
#define GAME_ENGINE_ENTITY_H

#include <cstdint>
#include "ECSTypes.h"
#include "raylib.h"

namespace GameEngine
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

        Color& color();

        State& state();

        ColliderLayerId& colliderLayerId();
        ColliderShape& colliderShape();
        
        float right();
        float left();
        float top();
        float bottom();
        
        bool operator==(const Entity& entity) const = default;
    };

    Entity::State operator|(Entity::State lhs, Entity::State rhs);
    Entity::State& operator|=(Entity::State& lhs, Entity::State rhs);
}

#endif //GAME_ENGINE_ENTITY_H
