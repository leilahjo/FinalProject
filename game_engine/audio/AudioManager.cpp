//
// Created by greg8 on 11/30/2025.
//

#include "AudioManager.h"
#include <memory>
#include "raylib.h"

namespace Audio
{
    void AudioManager::initialize()
    {
        InitAudioDevice();
    }

    SoundId AudioManager::loadAudioAsset(const std::string& path, uint8_t maxVoices,
                                         VoiceExhaustionBehavior behavior)
    {
        auto soundEffect = SoundEffect();
        soundEffect.load(path, maxVoices, behavior);
        soundEffects.emplace_back(soundEffect);
        return soundEffects.size() - 1;
    }


    bool AudioManager::playOneshot(SoundId soundId)
    {
        return soundEffects[soundId].playOneShot();
    }
}
