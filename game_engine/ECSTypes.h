//
// Created by greg8 on 2/5/2026.
//

#ifndef GAME_ENGINE_ECSTYPES_H
#define GAME_ENGINE_ECSTYPES_H

#include <cstdint>

namespace GameEngine
{
    struct Archetype;

    using ArchIndex = std::uint32_t;
    static constexpr ArchIndex INVALID_ARCH_INDEX = -1; //0xFFFFFFFF

    using EntityIndex = uint32_t;

    using EntityRecordIndex = uint32_t;

    using ComponentMask = uint32_t;

    using ColliderLayerId = uint8_t;
    static constexpr ColliderLayerId DEFAULT_COLLISION_LAYER_ID = 0;

    enum struct ColliderShape
    {
        RECT, CIRCLE
    };

    using AnimationId = std::uint32_t;
    static constexpr ArchIndex INVALID_ANIMATION_ID = -1;

    struct EntityLocation
    {
        Archetype* archetype;
        EntityIndex entityIndex;

        bool operator==(const EntityLocation& entity) const = default;
    };

    static constexpr EntityLocation INVALID_ENTITY_LOCATION = {nullptr, INVALID_ARCH_INDEX};

    struct EntityId
    {
        EntityRecordIndex recordIndex;
        uint32_t generation;
        bool operator==(const EntityId& entityId) const = default;
    };

    static constexpr EntityId INVALID_ENTITY_ID = {static_cast<uint32_t>(-1), static_cast<uint32_t>(-1)};

    struct EntityRecord
    {
        uint32_t generation;
        EntityLocation location;
    };

    using SpriteId = uint32_t;
    static constexpr SpriteId INVALID_SPRITE_ID = -1; //0xFFFFFFFF
}

#endif //GAME_ENGINE_ECSTYPES_H
