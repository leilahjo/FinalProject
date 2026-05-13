//
// Created by greg8 on 2/17/2026.
//

#ifndef ENGINE_CORE_COLLISIONSYSTEM_H
#define ENGINE_CORE_COLLISIONSYSTEM_H

#include <vector>
#include "../ecs/ECSTypes.h"
#include "../ecs/Entity.h"
#include "engine_core/memory_management/ArenaAllocator.h"

namespace EngineCore
{
    struct World;

    struct Collision
    {
        EntityId a;
        EntityId b;
    };

    struct CollisionSystem
    {
        static constexpr size_t MAX_COLLISION_LAYER_COUNT = 64;

        void enableCollisions(ColliderLayerId a, ColliderLayerId b, bool enabled);

        Collision* detect(World& world, size_t& collisionCount, ArenaAllocator& allocator);
        void resolve(World& world, Collision collisions[], size_t collisionCount);

    private:
        static void separate(Entity& a, Entity& b);
        bool permittedLayerCollisions[MAX_COLLISION_LAYER_COUNT][MAX_COLLISION_LAYER_COUNT] = {{}};
        std::vector<Entity> colliderEntities[MAX_COLLISION_LAYER_COUNT];
    };
}

#endif //ENGINE_CORE_COLLISIONSYSTEM_H
