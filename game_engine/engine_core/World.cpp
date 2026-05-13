//
// Created by greg8 on 11/9/2025.
//

#include "World.h"

namespace EngineCore
{
    World::World(float frameDt) : frameDt(frameDt)
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
        archetypes.push_back(std::make_unique<Archetype>(mask));
        return archetypes.back().get();
    }

    EntityId World::createEntity(Archetype* archetype,
                                 float x, float y,
                                 float vx, float vy,
                                 float width, float height,
                                 GameColor color,
                                 State state,
                                 ColliderShape colliderShape, ColliderLayerId colliderLayerId,
                                 AnimationData animationData,
                                 SpriteId spriteId,
                                 EntityTypeId entityType,
                                 BehaviorId behaviorId,
                                 float maxHp, float currentHp,
                                 float rotation,
                                 float particleLifetime, float particleMaxLifetime)
    {
        if (freeIndices.empty())
            return INVALID_ENTITY_ID;

        EntityRecordIndex recordIndex = freeIndices.back();
        freeIndices.pop_back();
        EntityRecord& entityRecord = entityRecords[recordIndex];

        auto entityId = EntityId{recordIndex, entityRecord.generation};

        EntityIndex entityIndex = archetype->createEntity(entityId,
            x, y, vx, vy, width, height,
            color, state,
            colliderShape, colliderLayerId,
            animationData, spriteId, entityType, behaviorId,
            maxHp, currentHp, rotation,
            particleLifetime, particleMaxLifetime);

        entityRecord.location = {archetype, entityIndex};
        return entityId;
    }

    EntityId World::createEntity(Archetype* archetype, const EntitySpec& s)
    {
        return createEntity(archetype,
            s.x, s.y, s.vx, s.vy, s.width, s.height,
            s.color, s.state,
            s.colliderShape, s.colliderLayerId,
            s.animationData, s.spriteId, s.entityType, s.behaviorId,
            s.maxHp, s.currentHp, s.rotation,
            s.particleLifetime, s.particleMaxLifetime);
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
                if (entity.state() & STATE_DESTROYED)
                    removeEntity(entity.id());
                else
                    entityIndex++;
            }
        }
    }
}
