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
            entityRecords[i] = EntityRecord{0, INVALID_ENTITY_LOCATION};
        freeIndices.resize(MAX_ENTITIES);
        for (size_t i = 0; i < freeIndices.size(); i++)
            freeIndices[i] = MAX_ENTITIES - i - 1;
    }

    Archetype* World::createArchetype(ComponentMask mask)
    {
        archetypes.push_back(
            std::make_unique<Archetype>(mask)
        );
        return archetypes.back().get();
    }

    EntityId World::createEntity(Archetype* archetype,
                                 float x, float y,
                                 float vx, float vy,
                                 float width, float height,
                                 Color color,
                                 Entity::State state)
    {
        if (freeIndices.empty())
            return INVALID_ENTITY_ID;

        EntityRecordIndex recordIndex = freeIndices.back();
        EntityRecord& entityRecord = entityRecords[recordIndex];
        freeIndices.pop_back();

        auto entityId = EntityId{recordIndex, entityRecord.generation};

        EntityIndex entityIndex = archetype->createEntity(entityId,
                                                          x, y,
                                                          vx, vy,
                                                          width, height,
                                                          color,
                                                          state);

        entityRecord.location = {archetype, entityIndex};

        return entityId;
    }

    bool World::removeEntity(EntityId entityId)
    {
        if (entityId == INVALID_ENTITY_ID || entityId.recordIndex >= entityRecords.size())
            return false;
        auto& destroyedRecord = entityRecords[entityId.recordIndex];
        if (destroyedRecord.generation != entityId.generation)
            return false;

        EntityId relocatedEntityId = destroyedRecord.location.archetype->removeEntity(
            destroyedRecord.location.entityIndex);
        if (relocatedEntityId != INVALID_ENTITY_ID)
        {
            auto& relocatedRecord = entityRecords[relocatedEntityId.recordIndex];
            relocatedRecord.location = destroyedRecord.location;
        }

        destroyedRecord.generation++;

        freeIndices.push_back(entityId.recordIndex);
        return true;
    }

    std::optional<Entity> World::findEntity(EntityId entityId)
    {
        if (entityId == INVALID_ENTITY_ID || entityId.recordIndex >= entityRecords.size())
            return std::nullopt;
        auto& entityRecord = entityRecords[entityId.recordIndex];
        if (entityRecord.generation != entityId.generation)
            return std::nullopt;

        return Entity{entityRecord.location};
    }

    void World::cleanup()
    {
        for (auto& archetype : archetypes)
        {
            if (!archetype->hasState())
                continue;

            for (EntityIndex entityIndex = 0; entityIndex < archetype->getEntityCount();)
            {
                auto entity = Entity{archetype.get(), entityIndex};
                if (entity.state() & Entity::STATE_DESTROYED)
                    removeEntity(entity.id());
                else
                    entityIndex++;
            }
        }
    }
}
