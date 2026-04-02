//
// Created by greg8 on 3/10/2026.
//

#ifndef GAME_ENGINE_PREFAB_H
#define GAME_ENGINE_PREFAB_H

#include "engine_core/EngineCore.h"

namespace Scripting
{
    using namespace EngineCore;

    struct Prefab
    {
        Archetype* archetype = nullptr;

        SpriteId spriteId = INVALID_SPRITE_ID;
        float width = 1;
        float height = 1;
        GameColor color = white;
        Entity::State state = Entity::STATE_DEFAULT;
        ColliderShape colliderShape = ColliderShape::RECT;
        ColliderLayerId colliderLayerId = DEFAULT_COLLISION_LAYER_ID;
        AnimationData animationData = {};
        EntityTypeId entityTypeId = -1;
        BehaviorId behaviorId = INVALID_BEHAVIOR_ID;
    };
}

#endif //GAME_ENGINE_PREFAB_H
