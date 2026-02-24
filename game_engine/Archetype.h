//
// Created by greg8 on 2/3/2026.
//

#ifndef GAME_ENGINE_ARCHETYPE_H
#define GAME_ENGINE_ARCHETYPE_H

#include <vector>
#include <cstdint>

#include "ECSTypes.h"
#include "Entity.h"
#include "raylib.h"

namespace GameEngine
{
    struct Archetype
    {
        enum Components : ComponentMask
        {
            COMP_POSITION = 1 << 0,
            COMP_VELOCITY = 1 << 1,
            COMP_SIZE = 1 << 2,
            COMP_COLOR = 1 << 3,
            COMP_STATE = 1 << 4,
            COMP_COLLIDER = 1 << 5,
            COMP_ANIMATION = 1 << 6
        };

        //Fixed per archetype.
        const ComponentMask componentMask;

        Archetype(ComponentMask mask);

        std::vector<EntityId> entityId;

        // Position Component
        std::vector<float> x, y;

        // Velocity Component
        std::vector<float> vx, vy;

        // Size Component
        std::vector<float> width, height;

        // Color Component
        std::vector<Color> color;

        // State Component
        std::vector<Entity::State> state;

        // Collider Component
        std::vector<ColliderShape> colliderShape;
        std::vector<ColliderLayerId> colliderLayerId;

        // Animation Component (SoAoS!)
        std::vector<AnimationData> animationData;

        EntityIndex createEntity(EntityId entityId,
                                 float x, float y,
                                 float vx, float vy,
                                 float width, float height,
                                 Color color,
                                 Entity::State state,
                                 ColliderShape collisionShape, ColliderLayerId collisionLayerId,
                                 AnimationData animationData);
        // Returns the Entity Id of the moved data, if any data was moved.
        EntityId removeEntity(EntityIndex entityIndex);

        bool hasPosition() { return componentMask & COMP_POSITION; }
        bool hasVelocity() { return componentMask & COMP_VELOCITY; }
        bool hasSize() { return componentMask & COMP_SIZE; }
        bool hasColor() { return componentMask & COMP_COLOR; }
        bool hasState() { return componentMask & COMP_STATE; }
        bool hasCollider() { return componentMask & COMP_COLLIDER; }
        bool hasAnimation() { return componentMask & COMP_ANIMATION; }

        size_t getEntityCount() const { return n; }

    private:
        size_t n;
    };
}

#endif //GAME_ENGINE_KINEMATICS_H
