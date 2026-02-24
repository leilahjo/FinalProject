//
// Created by greg8 on 1/27/2026.
//

#include "Entity.h"
#include "Archetype.h"

namespace GameEngine
{
    float& Entity::x() { return entityLocation.archetype->x[entityLocation.entityIndex]; }
    float& Entity::y() { return entityLocation.archetype->y[entityLocation.entityIndex]; }
    float& Entity::vx() { return entityLocation.archetype->vx[entityLocation.entityIndex]; }
    float& Entity::vy() { return entityLocation.archetype->vy[entityLocation.entityIndex]; }
    float& Entity::width() { return entityLocation.archetype->width[entityLocation.entityIndex]; }
    float& Entity::height() { return entityLocation.archetype->height[entityLocation.entityIndex]; }
    Color& Entity::color() { return entityLocation.archetype->color[entityLocation.entityIndex]; }
    Entity::State& Entity::state() { return entityLocation.archetype->state[entityLocation.entityIndex]; }
    EntityId& Entity::id() { return entityLocation.archetype->entityId[entityLocation.entityIndex]; }
    ColliderShape& Entity::colliderShape() { return entityLocation.archetype->colliderShape[entityLocation.entityIndex]; }
    ColliderLayerId& Entity::colliderLayerId() { return entityLocation.archetype->colliderLayerId[entityLocation.entityIndex]; }
    AnimationData& Entity::animation() { return entityLocation.archetype->animationData[entityLocation.entityIndex]; }

    float Entity::right() { return x() + width() / 2; }
    float Entity::left() { return x() - width() / 2; }
    float Entity::top() { return y() + height() / 2; }
    float Entity::bottom() { return y() - height() / 2; }

    bool Entity::hasAnimation() { return entityLocation.archetype->hasAnimation(); }

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
