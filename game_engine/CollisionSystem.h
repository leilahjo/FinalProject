//
// Created by greg8 on 2/17/2026.
//

#ifndef GAME_ENGINE_COLLISIONSYSTEM_H
#define GAME_ENGINE_COLLISIONSYSTEM_H
#include <vector>

#include "ECSTypes.h"

namespace GameEngine
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

        std::vector<Collision> detect(World& world);

    private:
        //Consider reducing the memory footprint by using bits rather than bytes.
        bool permittedLayerCollisions[MAX_COLLISION_LAYER_COUNT][MAX_COLLISION_LAYER_COUNT] = {{}};
    };
}

#endif //GAME_ENGINE_COLLISIONSYSTEM_H
