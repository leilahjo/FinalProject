//
// Created by greg8 on 1/27/2026.
//

#include "Entity.h"
#include "Archetype.h"

namespace GameEngine
{
    float& Entity::x() { return archetype->x[entityIndex]; }
    float& Entity::y() { return archetype->y[entityIndex]; }
    float& Entity::vx() { return archetype->vx[entityIndex]; }
    float& Entity::vy() { return archetype->vy[entityIndex]; }
    float& Entity::width() { return archetype->width[entityIndex]; }
    float& Entity::height() { return archetype->height[entityIndex]; }
    Color& Entity::color() { return archetype->color[entityIndex]; }
    Entity::State& Entity::state() { return archetype->state[entityIndex]; }

    Entity::State operator|(Entity::State lhs, Entity::State rhs)
    {
        return static_cast<Entity::State>(
            static_cast<Entity::StateMask>(lhs) | static_cast<Entity::StateMask>(rhs)
        );
    }

    Entity::State& operator|=(Entity::State& lhs, Entity::State rhs)
    {
        lhs = lhs | rhs;
        return lhs;
    }
}
