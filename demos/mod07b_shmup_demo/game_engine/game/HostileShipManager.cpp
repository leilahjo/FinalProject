//
// Created by greg8 on 2/15/2026.
//

#include "HostileShipManager.h"

#include "Game.h"

void Game::HostileShipManager::init(Game& game, World& world)
{
    hostileShipArchetype = world.createArchetype(
        Archetype::COMP_POSITION
        | Archetype::COMP_VELOCITY
        | Archetype::COMP_SIZE
        | Archetype::COMP_STATE
        | Archetype::COMP_COLLIDER
        | Archetype::COMP_SPRITE);

    hostileShipSpriteId = game.runtime.renderer.spriteManager.createSprite("assets/hostile_ship.png");
}

void Game::HostileShipManager::update(Game& game, World& world)
{
    if (game.runtime.inputManager.keyOne)
        spawnMode = SpawnMode::NONE;
    else if (game.runtime.inputManager.keyTwo)
        spawnMode = SpawnMode::RANDOM;
    else if (game.runtime.inputManager.keyThree)
        spawnMode = SpawnMode::WAVES;

    switch (spawnMode)
    {
    case SpawnMode::NONE:
        // Do nothing.
        break;
    case SpawnMode::WAVES:
        updateSpawnsWaves(game, world);
        break;
    case SpawnMode::RANDOM:
        updateSpawnsRandom(game, world);
        break;
    }
}

void Game::HostileShipManager::updateSpawnsWaves(Game& game, World& world)
{
    if (game.runtime.frameData.frame % (2 * 60) == 0)
        for (int i = 0; i < 10; i++)
            spawnHostileShip(game, world, -1 + 2 * (i / 10.0f));
}

void Game::HostileShipManager::updateSpawnsRandom(Game& game, World& world)
{
    if (game.runtime.frameData.frame % 5 == 0)
        spawnHostileShip(game, world, randomFloat(-1, 1));
}


void Game::HostileShipManager::spawnHostileShip(Game& game, World& world, float x)
{
    world.createEntity(hostileShipArchetype,
                       x, game.hostileShipBoundary.top,
                       0, -HOSTILE_SHIP_SPEED,
                       HOSTILE_SHIP_WIDTH, HOSTILE_SHIP_HEIGHT,
                       green,
                       Entity::STATE_DEFAULT,
                       ColliderShape::RECT, Game::LAYER_HOSTILE_SHIPS,
                       {},
                       hostileShipSpriteId);
}

void Game::HostileShipManager::enforceBoundary(Game& game, World& world)
{
    // TODO do this with entity type
    for (EntityIndex entityIndex = 0; entityIndex < hostileShipArchetype->getEntityCount(); entityIndex++)
    {
        auto entity = Entity{EntityLocation{hostileShipArchetype, entityIndex}};
        if (entity.y() < game.hostileShipBoundary.bottom)
            entity.state() |= Entity::STATE_DESTROYED;
    }
}
