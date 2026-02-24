//
// Created by greg8 on 2/3/2026.
//

#include "Archetype.h"
#include "ECSTypes.h"

namespace GameEngine
{
    Archetype::Archetype(ComponentMask mask) : componentMask(mask), n(0)
    {
    }

    EntityIndex Archetype::createEntity(
        EntityId entityId,
        float x, float y,
        float vx, float vy,
        float width, float height,
        Color color,
        Entity::State state,
        ColliderShape collisionShape, ColliderLayerId collisionLayerId,
        AnimationData animationData)
    {
        this->entityId.push_back(entityId);

        if (componentMask & COMP_POSITION)
        {
            this->x.push_back(x);
            this->y.push_back(y);
        }

        if (componentMask & COMP_VELOCITY)
        {
            this->vx.push_back(vx);
            this->vy.push_back(vy);
        }

        if (componentMask & COMP_SIZE)
        {
            this->width.push_back(width);
            this->height.push_back(height);
        }

        if (componentMask & COMP_COLOR)
        {
            this->color.push_back(color);
        }

        if (componentMask & COMP_STATE)
        {
            this->state.push_back(state);
        }

        if (componentMask & COMP_COLLIDER)
        {
            this->colliderShape.push_back(collisionShape);
            this->colliderLayerId.push_back(collisionLayerId);
        }

        if (componentMask & COMP_ANIMATION)
        {
            this->animationData.push_back(animationData);
        }

        return n++;
    }

    EntityId Archetype::removeEntity(EntityIndex entityIndex)
    {
        if (entityIndex >= n)
            return INVALID_ENTITY_ID;
        EntityIndex lastIndex = n - 1;

        EntityId movedEntityId = (entityIndex == lastIndex)
                                     ? INVALID_ENTITY_ID
                                     : entityId[lastIndex];

        entityId[entityIndex] = entityId[lastIndex];
        entityId.pop_back();

        if (componentMask & COMP_POSITION)
        {
            x[entityIndex] = x[lastIndex];
            x.pop_back();
            y[entityIndex] = y[lastIndex];
            y.pop_back();
        }

        if (componentMask & COMP_VELOCITY)
        {
            vx[entityIndex] = vx[lastIndex];
            vx.pop_back();
            vy[entityIndex] = vy[lastIndex];
            vy.pop_back();
        }

        if (componentMask & COMP_SIZE)
        {
            width[entityIndex] = width[lastIndex];
            width.pop_back();
            height[entityIndex] = height[lastIndex];
            height.pop_back();
        }

        if (componentMask & COMP_COLOR)
        {
            color[entityIndex] = color[lastIndex];
            color.pop_back();
        }

        if (componentMask & COMP_STATE)
        {
            state[entityIndex] = state[lastIndex];
            state.pop_back();
        }

        if (componentMask & COMP_COLLIDER)
        {
            colliderShape[entityIndex] = colliderShape[lastIndex];
            colliderShape.pop_back();
            colliderLayerId[entityIndex] = colliderLayerId[lastIndex];
            colliderLayerId.pop_back();
        }

        if (componentMask & COMP_ANIMATION)
        {
            animationData[entityIndex] = animationData[lastIndex];
            animationData.pop_back();
        }

        n--;
        return movedEntityId;
    }
}
