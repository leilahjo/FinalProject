//
// Created by greg8 on 2/28/2026.
//

#include "SoundEffect.h"

#include <string>
#include <utility>

namespace Audio
{
    void SoundEffect::load(const std::string& path, uint8_t maxVoices, VoiceExhaustionBehavior behavior)
    {
        unload();
        voices.resize(maxVoices);
        nextVoiceIndex = 0;

        voices[0] = LoadSound(path.c_str());
        for (int i = 1; i < maxVoices; ++i)
            voices[i] = LoadSoundAlias(voices[0]);

        voiceExhaustionBehavior = behavior;
    }

    void SoundEffect::unload()
    {
        if (!voices.empty())
        {
            for (size_t i = 1; i < voices.size(); ++i)
                UnloadSoundAlias(voices[i]);
            UnloadSound(voices[0]);
            voices.clear();
            nextVoiceIndex = 0;

            generation++;
        }
    }

    bool SoundEffect::playOneShot()
    {
        if (voices.empty())
            return false;
        auto& nextVoice = voices[nextVoiceIndex];
        if (IsSoundPlaying(nextVoice) && voiceExhaustionBehavior == DROP)
            return false;
        PlaySound(nextVoice);
        nextVoiceIndex = (nextVoiceIndex + 1) % voices.size();
        return true;
    }

    SoundEffect::SoundEffect(SoundEffect&& other) noexcept
        : voices(std::move(other.voices))
          , nextVoiceIndex(other.nextVoiceIndex)
          , voiceExhaustionBehavior(other.voiceExhaustionBehavior)
          , generation(other.generation)
    {
        other.voices.clear();
        other.nextVoiceIndex = 0;
        other.generation = 0;
    }
}
