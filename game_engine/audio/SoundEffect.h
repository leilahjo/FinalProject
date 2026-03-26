//
// Created by greg8 on 2/27/2026.
//

#ifndef GAME_ENGINE_SOUNDEFFECT_H
#define GAME_ENGINE_SOUNDEFFECT_H

#include <cstdint>
#include <vector>
#include <string>

#include "AudioTypes.h"
#include "raylib.h"

namespace Audio
{
    struct SoundEffect
    {
        SoundEffect() = default;

        void load(const std::string& path, uint8_t maxVoices, VoiceExhaustionBehavior behavior);
        void unload();
        bool playOneShot();

        // Copy semantics (disallowed)
        SoundEffect(const SoundEffect&) = delete;
        SoundEffect& operator=(const SoundEffect&) = delete;

        // Move semantics
        SoundEffect& operator=(SoundEffect&& other) = delete;
        SoundEffect(SoundEffect&& other) noexcept;

        friend struct AudioManager;

    private:
        std::vector<Sound> voices;
        uint8_t nextVoiceIndex = 0;
        VoiceExhaustionBehavior voiceExhaustionBehavior = DROP;
        uint32_t generation = 1;
    };
}

#endif //GAME_ENGINE_SOUNDEFFECT_H
