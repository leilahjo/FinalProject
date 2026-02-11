//
// Created by greg8 on 2/10/2026.
//

#include "World.h"

namespace GameEngine
{
    World::World()
    {
        entityRecords.resize(MAX_ENTITIES);
        for (EntityRecordIndex i = 0; i < entityRecords.size(); i++)
            entityRecords[i] = EntityRecord{EntityId{i, 0}, INVALID_ENTITY_LOCATION};
        freeIndices.resize(MAX_ENTITIES);
        for (size_t i = 0; i < freeIndices.size(); i++)
            freeIndices[i] = MAX_ENTITIES - i - 1;
    }

    Entity World::createEntity(Archetype* archetype,
                      float x, float y,
                      float vx, float vy,
                      float width, float height,
                      Color color,
                      Entity::State state)
    {
        EntityIndex entityIndex = archetype->createEntity(x, y,
                                                          vx, vy,
                                                          width, height,
                                                          color,
                                                          state);
        return Entity{archetype, entityIndex};
    }

    void World::cleanup()
    {
        for (ArchIndex archIndex = 0; archIndex < archetypes.size(); archIndex++)
        {
            auto& archetype = archetypes[archIndex];

            if (!archetype->hasState())
                continue;

            for (EntityIndex entityIndex = 0; entityIndex < archetype->getEntityCount();)
            {
                if (archetype->state[entityIndex] & Entity::State::STATE_DESTROYED)
                    archetype->removeEntity(entityIndex);
                else
                    entityIndex++;
            }
        }
    }
}
