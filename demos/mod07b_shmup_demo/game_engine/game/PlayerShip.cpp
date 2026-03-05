//
// Created by greg8 on 2/15/2026.
//

#include "PlayerShip.h"

#include "Game.h"

void Game::PlayerShip::init(Game& game, World& world)
{
    nextShootFrame = 0;

    shipRollLeftAnimation = world.animationSystem.createAnimation(STOP_AT_END, 0.5f, 4, 4);
    shipRollRightAnimation = world.animationSystem.createAnimation(STOP_AT_END, 0.5f, 0, 4);
    shipIdleAnimation = world.animationSystem.createAnimation(STOP_AT_END, 1.0f, 8, 1);

    playerShipSpriteSheetId = game.runtime.renderer.spriteManager.createSprite("assets/ship_roll_sheet.png", 4, 3, 2);
    playerLaserSpriteId = game.runtime.renderer.spriteManager.createSprite("assets/player_laser.png");

    playerArchetype = world.createArchetype(
        Archetype::COMP_POSITION
        | Archetype::COMP_VELOCITY
        | Archetype::COMP_SIZE
        | Archetype::COMP_COLLIDER
        | Archetype::COMP_ANIMATION
        | Archetype::COMP_SPRITE);

    laserArchetype = world.createArchetype(
        Archetype::COMP_POSITION
        | Archetype::COMP_VELOCITY
        | Archetype::COMP_SIZE
        | Archetype::COMP_STATE
        | Archetype::COMP_COLLIDER
        | Archetype::COMP_SPRITE);

    playerId = world.createEntity(playerArchetype,
                                  0, 0,
                                  0, 0,
                                  0.25, 0.25,
                                  green,
                                  Entity::STATE_DEFAULT,
                                  ColliderShape::RECT, Game::LAYER_PLAYER,
                                  {},
                                  playerShipSpriteSheetId);
}

void Game::PlayerShip::update(Game& game, World& world)
{
    // Input directly affects player velocity.
    Entity player = world.findEntity(playerId).value();

    // Target vx/vy makes handling contradictory inputs cleaner
    float targetVx = 0;
    float targetVy = 0;
    if (game.runtime.inputManager.keyLeft)
        targetVx -= PLAYER_MAX_SPEED;
    if (game.runtime.inputManager.keyRight)
        targetVx += PLAYER_MAX_SPEED;
    if (game.runtime.inputManager.keyUp)
        targetVy += PLAYER_MAX_SPEED;
    if (game.runtime.inputManager.keyDown)
        targetVy -= PLAYER_MAX_SPEED;

    // Accelerate towards target vx / vy
    if (player.vx() < targetVx)
    {
        player.vx() += PLAYER_ACCEL * world.frameDt;
        if (player.vx() > targetVx)
            player.vx() = targetVx;
    }
    if (player.vx() > targetVx)
    {
        player.vx() -= PLAYER_ACCEL * world.frameDt;
        if (player.vx() < targetVx)
            player.vx() = targetVx;
    }
    if (player.vy() < targetVy)
    {
        player.vy() -= PLAYER_ACCEL * world.frameDt;
        if (player.vy() < targetVy)
            player.vy() = targetVy;
    }
    if (player.vy() > targetVy)
    {
        player.vy() += PLAYER_ACCEL * world.frameDt;
        if (player.vy() > targetVy)
            player.vy() = targetVy;
    }

    // Manually set animation progress. This allows the physics to drive the animation. This incidentally makes
    // animation speed and playback mode irrelevant.
    if (player.vx() == 0)
        player.animation() = AnimationData{
            shipIdleAnimation, 0, 0, AnimationData::State::PAUSED
        };
    else if (player.vx() > 0)
        player.animation() = AnimationData{
            shipRollRightAnimation, player.vx() / PLAYER_MAX_SPEED, 0, AnimationData::State::PAUSED
        };
    else if (player.vx() < 0)
        player.animation() = AnimationData{
            shipRollLeftAnimation, -player.vx() / PLAYER_MAX_SPEED, 0, AnimationData::State::PAUSED
        };

    if (game.runtime.inputManager.keySpace && game.runtime.frameData.frame > nextShootFrame)
        shootLaser(game, world, player);
}


void Game::PlayerShip::shootLaser(Game& game, World& world, Entity player)
{
    nextShootFrame = game.runtime.frameData.frame + LASER_SHOOT_COOLDOWN_FRAMES;

    createLaserEntity(game, world, player, player.width() * 0.3);
    createLaserEntity(game, world, player, player.width() * 0.1);
    createLaserEntity(game, world, player, -player.width() * 0.1);
    createLaserEntity(game, world, player, -player.width() * 0.3);
}


void Game::PlayerShip::createLaserEntity(Game& game, World& world, Entity player, float horizontalOffset)
{
    float width = player.width() / 8;
    world.createEntity(laserArchetype,
                       player.x() + horizontalOffset, player.y() - player.height() / 2,
                       0, LASER_SPEED,
                       width, width * 4,
                       green,
                       Entity::STATE_DEFAULT,
                       ColliderShape::RECT, Game::LAYER_LASERS,
                       {},
                       playerLaserSpriteId);
}

void Game::PlayerShip::enforceBoundary(Game& game, World& world)
{
    Entity player = world.findEntity(playerId).value();
    if (player.left() < game.playerBoundary.left) player.x() = game.playerBoundary.left + player.width() / 2;
    if (player.right() > game.playerBoundary.right) player.x() = game.playerBoundary.right - player.width() / 2;
    if (player.bottom() < game.playerBoundary.bottom) player.y() = game.playerBoundary.bottom + player.height() / 2;
    if (player.top() > game.playerBoundary.top) player.y() = game.playerBoundary.top - player.height() / 2;

    //TODO replace with entity type
    for (EntityIndex entityIndex = 0; entityIndex < laserArchetype->getEntityCount(); entityIndex++)
    {
        auto entity = Entity{EntityLocation{laserArchetype, entityIndex}};
        if (entity.y() > game.playerProjectileBoundary.top)
            entity.state() |= Entity::STATE_DESTROYED;
    }
}
