#pragma once

#include "engine_core/EngineCore.h"
#include "rendering/Renderer.h"

namespace Game
{
    using namespace EngineCore;
    using namespace Rendering;

    // Populated once in onStart. Call spawn* to create entities via EntitySpec
    // designated initializers instead of 15-parameter createEntity call sites.
    struct EntityFactory
    {
        // Archetypes — set by Game::onStart
        Archetype* playerArch   = nullptr;
        Archetype* chaserArch   = nullptr;
        Archetype* tankArch     = nullptr;
        Archetype* projArch     = nullptr;
        Archetype* pickupArch   = nullptr;
        Archetype* particleArch = nullptr;

        // Shared assets — set by Game::onStart
        SpriteId    playerSprite;
        AnimationId playerRunAnim;

        // Entity type IDs — set by Game before calling spawn*
        EntityTypeId typePlayer    = 0;
        EntityTypeId typeChaser    = 0;
        EntityTypeId typeTank      = 0;
        EntityTypeId typeProjectile= 0;
        EntityTypeId typePickup    = 0;
        EntityTypeId typeParticle  = 0;

        ColliderLayerId layerPlayer  = 0;
        ColliderLayerId layerEnemy   = 0;
        ColliderLayerId layerProj    = 0;
        ColliderLayerId layerPickup  = 0;

        BehaviorId chaserBehaviorId = INVALID_BEHAVIOR_ID;

        EntityId spawnPlayer(World& world, float x, float y);
        EntityId spawnChaser(World& world, float x, float y);
        EntityId spawnTank  (World& world, float x, float y);
        EntityId spawnProj  (World& world, float x, float y, float dx, float dy);
        EntityId spawnPickup(World& world, float x, float y);
        void     spawnBurst (World& world, float x, float y, GameColor color, int n = 8);
    };
}
