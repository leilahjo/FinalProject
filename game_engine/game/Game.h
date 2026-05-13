#pragma once

#include "engine_runtime/EngineRuntime.h"
#include "EntityFactory.h"
#include "WaveManager.h"

namespace Game
{
    using namespace EngineRuntime;

    struct ArenaGame final : IGame
    {
        Runtime runtime;

        // -- Archetypes --
        Archetype* playerArch   = nullptr;
        Archetype* chaserArch   = nullptr;
        Archetype* tankArch     = nullptr;
        Archetype* projArch     = nullptr;
        Archetype* pickupArch   = nullptr;
        Archetype* particleArch = nullptr;
        Archetype* wallArch     = nullptr;

        EntityId playerId;

        // -- Assets --
        SpriteId    playerSprite;
        AnimationId playerRunAnim;
        SoundId     shootSfx;
        SoundId     hitSfx;
        SoundId     dieSfx;
        MusicId     bgMusic;

        // -- Game state --
        enum State { PLAYING, DEAD } state = PLAYING;
        float survivalTime = 0;
        int   kills        = 0;
        float fireCooldown = 0;
        float hitCooldown  = 0; // invincibility frames after taking damage

        // -- Debug stats (text output, updated each frame) --
        size_t lastCollisionCount = 0;
        int    debugLogTimer      = 0; // frame counter for periodic console logs

        EntityFactory factory;
        WaveManager   waves;

        // IGame
        void onStart(World& world, Renderer& renderer) override;
        void onUpdateBegin(World& world) override;
        void onUpdatePostKinematics(World& world) override;
        void onUpdatePostCollisionDetection(World& world, Collision[], size_t) override;
        void onUpdatePostCollisionResolution(World& world, Collision[], size_t) override;

        int run();

        enum EntityType : EntityTypeId
        {
            TYPE_NONE = 0,
            TYPE_PLAYER,
            TYPE_CHASER,
            TYPE_TANK,
            TYPE_PROJECTILE,
            TYPE_PICKUP,
            TYPE_PARTICLE,
            TYPE_WALL,
        };

        enum Layer : ColliderLayerId
        {
            LAYER_NONE = 0,
            LAYER_PLAYER,
            LAYER_ENEMY,
            LAYER_PROJ,
            LAYER_PICKUP,
        };

    private:
        void handleInput(World& world);
        void updateEnemyAI(World& world);
        void clampToArena(World& world);
        void printDebugLog(World& world);
        void spawnWalls(World& world);
        void restartGame(World& world);
    };
}
