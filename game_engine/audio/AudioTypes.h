//
// Created by greg8 on 2/27/2026.
//

#ifndef GAME_ENGINE_AUDIOTYPES_H
#define GAME_ENGINE_AUDIOTYPES_H

#include <cstdint>

namespace Audio
{
    enum VoiceExhaustionBehavior
    {
        DROP,    // Discard the incoming request.
        REPLACE  // Start the oldest sound over again.
    };

    struct MusicId
    {
        uint16_t index;
        uint32_t generation;
    };

    static constexpr MusicId INVALID_MUSIC_ID = {
        static_cast<uint16_t>(-1),
        static_cast<uint32_t>(-1)
    };
}

#endif //GAME_ENGINE_AUDIOTYPES_H
