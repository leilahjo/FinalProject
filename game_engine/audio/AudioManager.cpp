//
// Created by greg8 on 11/30/2025.
//

#include "AudioManager.h"
#include <memory>
#include "raylib.h"

namespace Audio
{
    void AudioManager::initialize(uint16_t maxEffects)
    {
        InitAudioDevice();
        this->maxEffects = maxEffects;
        soundEffects.resize(maxEffects);
        freeIndices.resize(maxEffects);
        for (size_t i = 0; i < freeIndices.size(); i++)
            freeIndices[i] = maxEffects - i - 1;
    }

    void AudioManager::deinitialize()
    {
        for (auto& soundEffect : soundEffects)
            soundEffect.unload();
        soundEffects.clear();
        freeIndices.clear();
        maxEffects = 0;
        CloseAudioDevice();
    }

    SoundId AudioManager::loadAudioAsset(const std::string& path, uint8_t maxVoices,
                                         VoiceExhaustionBehavior behavior)
    {
        if (freeIndices.empty() || maxVoices == 0)
            return INVALID_SOUND_ID;

        auto freeIndex = freeIndices.back();
        freeIndices.pop_back();
        soundEffects[freeIndex].load(path, maxVoices, behavior);

        return SoundId{freeIndex, soundEffects[freeIndex].generation};
    }

    bool AudioManager::unloadAudioAsset(SoundId soundId)
    {
        if (soundId.index >= soundEffects.size())
            return false;
        auto& soundEffect = soundEffects[soundId.index];
        if (soundEffect.generation != soundId.generation)
            return false;

        soundEffects[soundId.index].unload();
        freeIndices.push_back(soundId.index);

        return true;
    }

    bool AudioManager::playOneshot(SoundId soundId)
    {
        if (soundId.index >= soundEffects.size())
            return false;
        auto& soundEffect = soundEffects[soundId.index];
        if (soundEffect.generation != soundId.generation)
            return false;
        return soundEffect.playOneShot();
    }
}
