//
// Created by greg8 on 2/3/2026.
//

#ifndef ENGINE_CORE_ARCHETYPE_H
#define ENGINE_CORE_ARCHETYPE_H

#include <vector>

#include "AnimationData.h"
#include "GameColor.h"
#include "ECSTypes.h"

namespace EngineCore
{
    struct Archetype
    {
        enum Components : ComponentMask
        {
            COMP_POSITION  = 1 << 0,
            COMP_VELOCITY  = 1 << 1,
            COMP_SIZE      = 1 << 2,
            COMP_COLOR     = 1 << 3,
            COMP_STATE     = 1 << 4,
            COMP_COLLIDER  = 1 << 5,
            COMP_ANIMATION = 1 << 6,
            COMP_SPRITE    = 1 << 7,
            COMP_TYPE      = 1 << 8,
            COMP_BEHAVIOR  = 1 << 9,
            COMP_HP        = 1 << 10,
            COMP_ROTATION  = 1 << 11,
            COMP_PARTICLE  = 1 << 12,
        };

        // Fixed per archetype.
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
        std::vector<GameColor> color;

        // State Component
        std::vector<State> state;

        // Collider Component
        std::vector<ColliderShape> colliderShape;
        std::vector<ColliderLayerId> colliderLayerId;

        // Animation Component (SoAoS!)
        std::vector<AnimationData> animationData;

        // Sprite Component
        std::vector<SpriteId> spriteId;

        // Type Component
        std::vector<EntityTypeId> entityType;

        // Behavior Component
        std::vector<BehaviorId> behaviorId;

        // Hitpoints Component
        std::vector<float> maxHp;
        std::vector<float> currentHp;

        // Rotation Component (degrees)
        std::vector<float> rotation;

        // Particle Component
        std::vector<float> particleLifetime;    // seconds remaining
        std::vector<float> particleMaxLifetime; // total lifetime

        EntityIndex createEntity(EntityId entityId,
                                 float x, float y,
                                 float vx, float vy,
                                 float width, float height,
                                 GameColor color,
                                 State state,
                                 ColliderShape collisionShape, ColliderLayerId collisionLayerId,
                                 AnimationData animationData,
                                 SpriteId spriteId,
                                 EntityTypeId typeId,
                                 BehaviorId behaviorId,
                                 float maxHp = 0.0f, float currentHp = 0.0f,
                                 float rotation = 0.0f,
                                 float particleLifetime = 0.0f, float particleMaxLifetime = 0.0f);

        // Returns the Entity Id of the moved data, if any data was moved.
        EntityId removeEntity(EntityIndex entityIndex);

        bool hasPosition()   { return componentMask & COMP_POSITION; }
        bool hasVelocity()   { return componentMask & COMP_VELOCITY; }
        bool hasSize()       { return componentMask & COMP_SIZE; }
        bool hasColor()      { return componentMask & COMP_COLOR; }
        bool hasState()      { return componentMask & COMP_STATE; }
        bool hasCollider()   { return componentMask & COMP_COLLIDER; }
        bool hasAnimation()  { return componentMask & COMP_ANIMATION; }
        bool hasSprite()     { return componentMask & COMP_SPRITE; }
        bool hasEntityType() { return componentMask & COMP_TYPE; }
        bool hasBehavior()   { return componentMask & COMP_BEHAVIOR; }
        bool hasHp()         { return componentMask & COMP_HP; }
        bool hasRotation()   { return componentMask & COMP_ROTATION; }
        bool hasParticle()   { return componentMask & COMP_PARTICLE; }

        size_t getEntityCount() const { return n; }

    private:
        size_t n;
    };
}

#endif //ENGINE_CORE_ARCHETYPE_H
