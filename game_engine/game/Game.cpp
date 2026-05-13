#include "Game.h"

#include <cmath>
#include <iostream>
#include <filesystem>

#include "raylib.h"

namespace Game
{
    static constexpr float ARENA_W  = 3.7f;   // half-width (so full arena is 7.4 units)
    static constexpr float ARENA_H  = 2.7f;   // half-height
    static constexpr float CAM_LAG  = 0.12f;  // how much the camera drifts toward player
    static constexpr float FIRE_CD  = 0.18f;  // seconds between shots
    static constexpr float HIT_CD   = 0.8f;   // invincibility after being hit
    static constexpr float PICKUP_HEAL = 30.0f;

    int ArenaGame::run()
    {
        return runtime.run(*this, 1200, 900, "Arena Survival");
    }

    void ArenaGame::onStart(World& world, Renderer& renderer)
    {
        world.viewportScale = 7.0f; // shows a bit more than the arena for parallax room
        world.backgroundColor = {15, 15, 25, 255};

        // Parallax background layers — dark dot grids at different depths
        renderer.addParallaxLayer({30, 30, 60, 255},  0.15f, 0.6f, 0.6f);
        renderer.addParallaxLayer({20, 50, 80, 190},  0.35f, 0.35f, 0.35f);

        // Archetypes
        playerArch = world.createArchetype(
            Archetype::COMP_POSITION | Archetype::COMP_VELOCITY |
            Archetype::COMP_SIZE     | Archetype::COMP_COLOR    |
            Archetype::COMP_STATE    | Archetype::COMP_COLLIDER |
            Archetype::COMP_ANIMATION| Archetype::COMP_SPRITE   |
            Archetype::COMP_TYPE     | Archetype::COMP_HP       |
            Archetype::COMP_ROTATION);

        chaserArch = world.createArchetype(
            Archetype::COMP_POSITION | Archetype::COMP_VELOCITY |
            Archetype::COMP_SIZE     | Archetype::COMP_COLOR    |
            Archetype::COMP_STATE    | Archetype::COMP_COLLIDER |
            Archetype::COMP_TYPE     | Archetype::COMP_HP       |
            Archetype::COMP_BEHAVIOR);

        tankArch = world.createArchetype(
            Archetype::COMP_POSITION | Archetype::COMP_VELOCITY |
            Archetype::COMP_SIZE     | Archetype::COMP_COLOR    |
            Archetype::COMP_STATE    | Archetype::COMP_COLLIDER |
            Archetype::COMP_TYPE     | Archetype::COMP_HP);

        projArch = world.createArchetype(
            Archetype::COMP_POSITION | Archetype::COMP_VELOCITY |
            Archetype::COMP_SIZE     | Archetype::COMP_COLOR    |
            Archetype::COMP_STATE    | Archetype::COMP_COLLIDER |
            Archetype::COMP_TYPE     | Archetype::COMP_PARTICLE |
            Archetype::COMP_ROTATION);

        pickupArch = world.createArchetype(
            Archetype::COMP_POSITION |
            Archetype::COMP_SIZE     | Archetype::COMP_COLOR    |
            Archetype::COMP_STATE    | Archetype::COMP_COLLIDER |
            Archetype::COMP_TYPE     | Archetype::COMP_PARTICLE);

        particleArch = world.createArchetype(
            Archetype::COMP_POSITION | Archetype::COMP_VELOCITY |
            Archetype::COMP_SIZE     | Archetype::COMP_COLOR    |
            Archetype::COMP_STATE    | Archetype::COMP_TYPE     |
            Archetype::COMP_PARTICLE);

        wallArch = world.createArchetype(
            Archetype::COMP_POSITION | Archetype::COMP_SIZE |
            Archetype::COMP_COLOR    | Archetype::COMP_STATE);

        playerSprite   = renderer.spriteManager.loadSprite("assets/player_run.png", 6, 4, 5);
        playerRunAnim  = world.animationSystem.createAnimation(LOOP, 1.0f, 0, 6);

        shootSfx = runtime.audioManager.loadAudioAsset("assets/doot.wav", 6, Audio::DROP);
        hitSfx   = runtime.audioManager.loadAudioAsset("assets/hit.wav",  4, Audio::DROP);
        dieSfx   = runtime.audioManager.loadAudioAsset("assets/die.wav",  4, Audio::REPLACE);

        // Music — load if file exists, gracefully skip otherwise
        auto musicPath = std::filesystem::path(PROJECT_ROOT) / "assets" / "music.ogg";
        if (std::filesystem::exists(musicPath))
        {
            bgMusic = runtime.audioManager.loadMusic(musicPath.string());
            runtime.audioManager.playMusic(bgMusic, 2.0f);
        }
        else
        {
            bgMusic = INVALID_MUSIC_ID;
            std::cout << "[Audio] No music.ogg found in assets/ — skipping BGM\n";
        }

        // Render layers (lower = drawn first/behind)
        renderer.typeToRenderLayer[TYPE_WALL]       = 1;
        renderer.typeToRenderLayer[TYPE_PICKUP]     = 2;
        renderer.typeToRenderLayer[TYPE_PARTICLE]   = 3;
        renderer.typeToRenderLayer[TYPE_CHASER]     = 4;
        renderer.typeToRenderLayer[TYPE_TANK]       = 4;
        renderer.typeToRenderLayer[TYPE_PROJECTILE] = 5;
        renderer.typeToRenderLayer[TYPE_PLAYER]     = 6;

        world.collisionSystem.enableCollisions(LAYER_PLAYER, LAYER_ENEMY,   true);
        world.collisionSystem.enableCollisions(LAYER_PLAYER, LAYER_PICKUP,  true);
        world.collisionSystem.enableCollisions(LAYER_PROJ,   LAYER_ENEMY,   true);

        factory.playerArch   = playerArch;
        factory.chaserArch   = chaserArch;
        factory.tankArch     = tankArch;
        factory.projArch     = projArch;
        factory.pickupArch   = pickupArch;
        factory.particleArch = particleArch;
        factory.playerSprite = playerSprite;
        factory.playerRunAnim= playerRunAnim;
        factory.typePlayer    = TYPE_PLAYER;
        factory.typeChaser    = TYPE_CHASER;
        factory.typeTank      = TYPE_TANK;
        factory.typeProjectile= TYPE_PROJECTILE;
        factory.typePickup    = TYPE_PICKUP;
        factory.typeParticle  = TYPE_PARTICLE;
        factory.layerPlayer   = LAYER_PLAYER;
        factory.layerEnemy    = LAYER_ENEMY;
        factory.layerProj     = LAYER_PROJ;
        factory.layerPickup   = LAYER_PICKUP;

        auto faunaPath = (std::filesystem::path(PROJECT_ROOT) / "assets" / "fauna.lua").string();
        factory.chaserBehaviorId = runtime.behaviorManager.loadBehavior(faunaPath);

        spawnWalls(world);

        playerId = factory.spawnPlayer(world, 0, 0);
        AnimationSystem::startAnimation(world, playerId, playerRunAnim);
        runtime.behaviorManager.setGlobalEntityId("PLAYER_ID", playerId);

        std::cout << "[Arena] Starting arena survival demo\n";
        std::cout << "[Arena] WASD/arrows = move, Mouse = aim, LMB = shoot, F1 = debug\n";
    }

    void ArenaGame::onUpdateBegin(World& world)
    {
        auto& input = runtime.inputManager;

        if (state == DEAD)
        {
            if (input.keyRPressed)
                restartGame(world);
            return;
        }

        survivalTime += world.frameDt;
        fireCooldown  = std::max(0.0f, fireCooldown - world.frameDt);
        hitCooldown   = std::max(0.0f, hitCooldown  - world.frameDt);

        handleInput(world);
        updateEnemyAI(world);

        if (waves.update(world.frameDt, world, factory))
            std::cout << "[Wave " << waves.wave << "] Started!\n";

        // Periodic entity-count debug log (every 180 frames ~3 sec)
        debugLogTimer++;
        if (debugLogTimer >= 180)
        {
            debugLogTimer = 0;
            printDebugLog(world);
        }
    }

    void ArenaGame::handleInput(World& world)
    {
        auto player = world.findEntity(playerId);
        if (!player) return;

        auto& input = runtime.inputManager;

        float mx = 0, my = 0;
        if (input.keyLeft)  mx -= 1;
        if (input.keyRight) mx += 1;
        if (input.keyUp)    my += 1;
        if (input.keyDown)  my -= 1;

        float len = std::sqrt(mx * mx + my * my);
        if (len > 0.01f) { mx /= len; my /= len; }

        float speed = 2.5f;
        player->vx() = mx * speed;
        player->vy() = my * speed;

        float dx = input.mouseWorldX - player->x();
        float dy = input.mouseWorldY - player->y();
        if (std::abs(dx) > 0.001f || std::abs(dy) > 0.001f)
            player->rotation() = std::atan2(-dy, dx) * (180.0f / 3.14159265f);

        if (input.keySpace && fireCooldown <= 0.0f)
        {
            float d = std::sqrt(dx * dx + dy * dy);
            if (d > 0.001f)
            {
                factory.spawnProj(world, player->x(), player->y(), dx / d, dy / d);
                runtime.audioManager.playOneshot(shootSfx);
                fireCooldown = FIRE_CD;
            }
        }
    }

    void ArenaGame::updateEnemyAI(World& world)
    {
        auto playerOpt = world.findEntity(playerId);
        if (!playerOpt) return;
        float px = playerOpt->x(), py = playerOpt->y();

        world.forEach(Archetype::COMP_POSITION | Archetype::COMP_VELOCITY | Archetype::COMP_TYPE,
            [&](Entity e)
            {
                if (!e.hasEntityType()) return;
                if (e.entityTypeId() != TYPE_CHASER && e.entityTypeId() != TYPE_TANK) return;
                if (e.hasBehavior()) return; // Lua script handles movement for this entity

                float dx = px - e.x(), dy = py - e.y();
                float d  = std::sqrt(dx * dx + dy * dy);
                if (d < 0.001f) return;

                float spd = (e.entityTypeId() == TYPE_TANK) ? 0.35f : 0.65f;
                e.vx() = (dx / d) * spd;
                e.vy() = (dy / d) * spd;

                float hw = e.width() * 0.5f, hh = e.height() * 0.5f;
                if (e.x() - hw <= -ARENA_W + 0.02f && e.vx() < 0) e.vx() = 0;
                if (e.x() + hw >=  ARENA_W - 0.02f && e.vx() > 0) e.vx() = 0;
                if (e.y() - hh <= -ARENA_H + 0.02f && e.vy() < 0) e.vy() = 0;
                if (e.y() + hh >=  ARENA_H - 0.02f && e.vy() > 0) e.vy() = 0;
            });
    }

    void ArenaGame::onUpdatePostKinematics(World& world)
    {
        clampToArena(world);
    }

    void ArenaGame::clampToArena(World& world)
    {
        world.forEach(Archetype::COMP_POSITION | Archetype::COMP_SIZE,
            [](Entity e)
            {
                float hw = e.width()  * 0.5f;
                float hh = e.height() * 0.5f;

                if (e.x() - hw < -ARENA_W) e.x() = -ARENA_W + hw;
                if (e.x() + hw >  ARENA_W) e.x() =  ARENA_W - hw;
                if (e.y() - hh < -ARENA_H) e.y() = -ARENA_H + hh;
                if (e.y() + hh >  ARENA_H) e.y() =  ARENA_H - hh;
            });
    }

    void ArenaGame::onUpdatePostCollisionDetection(World& world, Collision* collisions, size_t count)
    {
        lastCollisionCount = count;

        for (size_t i = 0; i < count; i++)
        {
            auto eA = world.findEntity(collisions[i].a);
            auto eB = world.findEntity(collisions[i].b);
            if (!eA || !eB) continue;

            auto& a = *eA;
            auto& b = *eB;
            if (!a.hasEntityType() || !b.hasEntityType()) continue;

            EntityTypeId ta = a.entityTypeId(), tb = b.entityTypeId();

            auto handleProjHit = [&](Entity& proj, Entity& enemy)
            {
                if (proj.entityTypeId()  != TYPE_PROJECTILE) return;
                if (enemy.entityTypeId() != TYPE_CHASER && enemy.entityTypeId() != TYPE_TANK) return;

                proj.state() |= STATE_DESTROYED;

                float dmg = 20.0f;
                if (enemy.hasHp())
                {
                    enemy.currentHp() -= dmg;
                    if (enemy.currentHp() <= 0)
                    {
                        kills++;
                        waves.notifyEnemyKilled();
                        factory.spawnBurst(world, enemy.x(), enemy.y(),
                                           enemy.color(), 10);

                        // ~40% chance to drop a pickup
                        if (randomFloat(0, 1) < 0.4f)
                            factory.spawnPickup(world, enemy.x(), enemy.y());

                        std::cout << "[Kill] enemy down | total kills: " << kills
                                  << " | wave enemies left: " << waves.enemiesLeft << "\n";
                    }
                    else
                    {
                        factory.spawnBurst(world, enemy.x(), enemy.y(),
                                           {255, 180, 60, 255}, 4);
                    }
                }
            };

            auto handlePlayerHit = [&](Entity& player, Entity& enemy)
            {
                if (player.entityTypeId() != TYPE_PLAYER) return;
                if (enemy.entityTypeId()  != TYPE_CHASER && enemy.entityTypeId() != TYPE_TANK) return;
                if (hitCooldown > 0) return;

                float dmg = (enemy.entityTypeId() == TYPE_TANK) ? 25.0f : 15.0f;
                if (player.hasHp())
                {
                    player.currentHp() -= dmg;
                    hitCooldown = HIT_CD;
                    factory.spawnBurst(world, player.x(), player.y(),
                                       {255, 80, 80, 255}, 6);
                    runtime.audioManager.playOneshot(hitSfx);

                    std::cout << "[Hit] player HP: " << player.currentHp()
                              << " / " << player.maxHp() << "\n";
                }
            };

            auto handlePickup = [&](Entity& player, Entity& pickup)
            {
                if (player.entityTypeId() != TYPE_PLAYER) return;
                if (pickup.entityTypeId() != TYPE_PICKUP) return;

                pickup.state() |= STATE_DESTROYED;
                if (player.hasHp())
                {
                    player.currentHp() = std::min(player.currentHp() + PICKUP_HEAL, player.maxHp());
                    factory.spawnBurst(world, pickup.x(), pickup.y(),
                                       {80, 255, 120, 255}, 6);
                }
            };

            handleProjHit(a, b);
            handleProjHit(b, a);
            handlePlayerHit(a, b);
            handlePlayerHit(b, a);
            handlePickup(a, b);
            handlePickup(b, a);
        }
    }

    void ArenaGame::onUpdatePostCollisionResolution(World& world, Collision*, size_t)
    {
        auto player = world.findEntity(playerId);

        if (player)
        {
            // Subtle camera drift — gives parallax something to work with
            world.viewportX = player->x() * CAM_LAG;
            world.viewportY = player->y() * CAM_LAG;

            if (player->hasHp() && player->currentHp() <= 0 && state == PLAYING)
            {
                state = DEAD;
                factory.spawnBurst(world, player->x(), player->y(),
                                   {255, 100, 100, 255}, 16);
                runtime.audioManager.playOneshot(dieSfx);
                player->state() |= STATE_DESTROYED;

                std::cout << "\n=== GAME OVER ===\n";
                std::cout << "Survived: " << static_cast<int>(survivalTime) << " seconds\n";
                std::cout << "Wave reached: " << waves.wave << "\n";
                std::cout << "Total kills: " << kills << "\n";
                std::cout << "=================\n";
            }
        }
        else if (state == PLAYING)
        {
            // Player entity was cleaned up already — mark dead
            state = DEAD;
        }

        // Write HUD snapshot for the renderer. Done last so HP reflects this frame's damage.
        world.hud.wave          = waves.wave;
        world.hud.kills         = kills;
        world.hud.survivalTime  = survivalTime;
        world.hud.isDead        = (state == DEAD);

        auto playerForHud = world.findEntity(playerId);
        if (playerForHud && playerForHud->hasHp())
        {
            world.hud.playerHpCurrent = playerForHud->currentHp();
            world.hud.playerHpMax     = playerForHud->maxHp();
        }
        else if (state == DEAD)
        {
            world.hud.playerHpCurrent = 0.0f;
        }
    }

    void ArenaGame::spawnWalls(World& world)
    {
        float bw = ARENA_W * 2 + 0.1f, bh = ARENA_H * 2 + 0.1f;
        constexpr GameColor wallColor = {80, 80, 160, 255};

        world.createEntity(wallArch, EntitySpec{ .x =  0,             .y =  ARENA_H + 0.03f, .width = bw,   .height = 0.06f, .color = wallColor });
        world.createEntity(wallArch, EntitySpec{ .x =  0,             .y = -ARENA_H - 0.03f, .width = bw,   .height = 0.06f, .color = wallColor });
        world.createEntity(wallArch, EntitySpec{ .x =  ARENA_W + 0.03f, .y = 0,             .width = 0.06f, .height = bh,   .color = wallColor });
        world.createEntity(wallArch, EntitySpec{ .x = -ARENA_W - 0.03f, .y = 0,             .width = 0.06f, .height = bh,   .color = wallColor });
    }

    void ArenaGame::restartGame(World& world)
    {
        // Mark every entity with a state component for destruction, then flush.
        world.forEach(Archetype::COMP_STATE, [](Entity e)
        {
            e.state() |= STATE_DESTROYED;
        });
        world.cleanup();

        // Reset game state.
        state        = PLAYING;
        survivalTime = 0.0f;
        kills        = 0;
        fireCooldown = 0.0f;
        hitCooldown  = 0.0f;
        lastCollisionCount = 0;
        debugLogTimer      = 0;
        waves = WaveManager{};

        // Flush HUD immediately so the game-over overlay disappears this frame.
        world.hud = HudData{};

        // Re-create arena geometry and player.
        spawnWalls(world);
        playerId = factory.spawnPlayer(world, 0, 0);
        AnimationSystem::startAnimation(world, playerId, playerRunAnim);
        runtime.behaviorManager.setGlobalEntityId("PLAYER_ID", playerId);

        std::cout << "[Arena] Restarted\n";
    }

    void ArenaGame::printDebugLog(World& world)
    {
        size_t total = 0, enemies = 0, projs = 0, particles = 0;

        world.forEach(Archetype::COMP_TYPE, [&](Entity e)
        {
            if (!e.hasEntityType()) return;
            total++;
            auto t = e.entityTypeId();
            if (t == TYPE_CHASER  || t == TYPE_TANK)       enemies++;
            if (t == TYPE_PROJECTILE)                       projs++;
            if (t == TYPE_PARTICLE)                         particles++;
        });

        std::cout << "[Debug] entities=" << total
                  << " enemies=" << enemies
                  << " projs=" << projs
                  << " particles=" << particles
                  << " | collisions last frame=" << lastCollisionCount
                  << " | kills=" << kills
                  << " | wave=" << waves.wave
                  << " | survivedSec=" << static_cast<int>(survivalTime) << "\n";
    }

}
