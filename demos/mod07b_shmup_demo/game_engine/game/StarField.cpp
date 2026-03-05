//
// Created by greg8 on 2/15/2026.
//

#include "StarField.h"

#include "Game.h"

using namespace Game;

void StarField::init(Game& game, World& world)
{
    archetype = world.createArchetype(
        Archetype::COMP_POSITION
        | Archetype::COMP_VELOCITY
        | Archetype::COMP_SIZE
        | Archetype::COMP_COLOR);

    for (size_t i = 0; i < 500; ++i)
        createStar(game, world);
}

void StarField::createStar(Game& game, World& world)
{
    float scale = 1 / 500.0f;
    float size = scale * randomFloat(1, 4);
    float speed = size * 30;
    float x = randomFloat(game.starBoundary.left, game.starBoundary.right);
    float y = randomFloat(game.starBoundary.bottom, game.starBoundary.top);
    world.createEntity(archetype,
                       x, y,
                       0, -speed,
                       size, size,
                       white,
                       Entity::STATE_DEFAULT,
                       ColliderShape::RECT, Game::LAYER_NONE,
                       {},
                       INVALID_SPRITE_ID);
}

void StarField::enforceBoundary(const Game& game, const World& world)
{
    // TODO do this with entity type
    for (EntityIndex entityIndex = 0; entityIndex < archetype->getEntityCount(); entityIndex++)
    {
        auto entity = Entity{EntityLocation{archetype, entityIndex}};
        if (entity.y() < game.starBoundary.bottom)
        {
            entity.y() = game.starBoundary.top;
            entity.x() = randomFloat(game.starBoundary.left, game.starBoundary.right);
        }
    }
}
