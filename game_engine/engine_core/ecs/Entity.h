//
// Created by greg8 on 1/27/2026.
//

#ifndef ENGINE_CORE_ENTITY_H
#define ENGINE_CORE_ENTITY_H


#include "AnimationData.h"
#include "GameColor.h"
#include "ECSTypes.h"
#include "Archetype.h"

namespace EngineCore
{
    // Represents an entity in the game world. The struct itself is a lightweight view
    // into the arrays of an archetype.
    struct Entity
    {
        EntityLocation entityLocation;

        float& x() { return entityLocation.archetype->x[entityLocation.entityIndex]; }
        float& y() { return entityLocation.archetype->y[entityLocation.entityIndex]; }
        float& vx() { return entityLocation.archetype->vx[entityLocation.entityIndex]; }
        float& vy() { return entityLocation.archetype->vy[entityLocation.entityIndex]; }
        float& width() { return entityLocation.archetype->width[entityLocation.entityIndex]; }
        float& height() { return entityLocation.archetype->height[entityLocation.entityIndex]; }
        GameColor& color() { return entityLocation.archetype->color[entityLocation.entityIndex]; }
        State& state() { return entityLocation.archetype->state[entityLocation.entityIndex]; }
        EntityId& id() { return entityLocation.archetype->entityId[entityLocation.entityIndex]; }
        EntityTypeId& entityTypeId() { return entityLocation.archetype->entityType[entityLocation.entityIndex]; }
        ColliderShape& colliderShape() { return entityLocation.archetype->colliderShape[entityLocation.entityIndex]; }
        ColliderLayerId& colliderLayerId() { return entityLocation.archetype->colliderLayerId[entityLocation.entityIndex]; }
        AnimationData& animation() { return entityLocation.archetype->animationData[entityLocation.entityIndex]; }
        SpriteId& spriteId() { return entityLocation.archetype->spriteId[entityLocation.entityIndex]; }
        BehaviorId& behaviorId() { return entityLocation.archetype->behaviorId[entityLocation.entityIndex]; }

        float right() { return x() + width() / 2; }
        float left() { return x() - width() / 2; }
        float top() { return y() + height() / 2; }
        float bottom() { return y() - height() / 2; }

        bool hasAnimation() { return entityLocation.archetype->hasAnimation(); }
        bool hasSprite() { return entityLocation.archetype->hasSprite(); }
        bool hasColor() { return entityLocation.archetype->hasColor(); }
        bool hasEntityType() { return entityLocation.archetype->hasEntityType(); }
        bool hasBehavior() { return entityLocation.archetype->hasBehavior(); }


        bool operator==(const Entity& entity) const = default;

    };

    inline State operator|(State lhs, State rhs)
    {
        return static_cast<State>(
            static_cast<StateMask>(lhs) | static_cast<StateMask>(rhs)
        );
    }

    inline State& operator|=(State& lhs, State rhs)
    {
        lhs = lhs | rhs;
        return lhs;
    }
}

#endif //ENGINE_CORE_ENTITY_H
