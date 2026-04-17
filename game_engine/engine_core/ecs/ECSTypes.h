//
// Created by greg8 on 2/5/2026.
//

#ifndef ENGINE_CORE_ECSTYPES_H
#define ENGINE_CORE_ECSTYPES_H

#include <cstdint>

namespace EngineCore
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

    struct SpriteId
    {
        uint16_t index;
        uint32_t generation;
    };

    static constexpr SpriteId INVALID_SPRITE_ID = {static_cast<uint16_t>(-1), static_cast<uint32_t>(-1)};

    using EntityTypeId = uint16_t;

    struct SoundId
    {
        uint16_t index;
        uint32_t generation;
    };

    static constexpr SoundId INVALID_SOUND_ID = {static_cast<uint16_t>(-1), static_cast<uint16_t>(-1)};

    struct BehaviorId
    {
        uint16_t index;
        uint32_t generation;
        bool operator==(const BehaviorId& behaviorId) const = default;
    };
    static constexpr BehaviorId INVALID_BEHAVIOR_ID = {static_cast<uint16_t>(-1), static_cast<uint32_t>(-1)};

    using StateMask = std::uint32_t;

    enum State : StateMask
    {
        STATE_DEFAULT = 0,
        STATE_DESTROYED = 1 << 0,
    };
}

#endif //ENGINE_CORE_ECSTYPES_H
