//
// Created by greg8 on 11/26/2025.
//

#ifndef GAME_ENGINE_AUDIOPLAYER_H
#define GAME_ENGINE_AUDIOPLAYER_H

#include <string>
#include <vector>

// raylib.h pulled in for Music type; audio is already a layered module.
#include "raylib.h"

#include "engine_core/ecs/ECSTypes.h"
#include "AudioTypes.h"
#include "SoundEffect.h"

namespace Audio
{
    using namespace EngineCore;

    struct MusicTrack
    {
        Music    stream    = {};
        bool     loaded    = false;
        uint32_t generation = 0;
        float    volume    = 1.0f;
        float    fadeTarget= 1.0f;
        float    fadeSpeed = 0.0f; // units/sec; 0 = already at target
    };

    struct AudioManager
    {
        void initialize(uint16_t maxEffects);
        void deinitialize();

        // Sound effects
        SoundId loadAudioAsset(const std::string& path, uint8_t maxVoices, VoiceExhaustionBehavior behavior);
        bool    unloadAudioAsset(SoundId soundId);
        bool    playOneshot(SoundId soundId);

        // Music — graceful no-op if file missing (IsMusicValid returns false)
        MusicId loadMusic(const std::string& path);
        bool    unloadMusic(MusicId id);
        bool    playMusic(MusicId id, float fadeIn = 0.0f);
        bool    stopMusic(MusicId id, float fadeOut = 0.0f);
        void    updateMusic(float dt);

    private:
        std::vector<SoundEffect> soundEffects;
        std::vector<uint16_t>   freeEffectIndices;
        uint16_t maxEffects = 0;

        static constexpr uint16_t MAX_MUSIC_TRACKS = 8;
        MusicTrack musicTracks[MAX_MUSIC_TRACKS];
    };
}

#endif //GAME_ENGINE_AUDIOPLAYER_H
