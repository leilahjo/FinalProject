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
        voices.resize(maxVoices);
        voices[0] = LoadSound(path.c_str());
        for (size_t i = 0; i < maxVoices; i++)
            voices[i] = LoadSoundAlias(voices[0]);

        voiceExhaustionBehavior = behavior;
    }

    bool SoundEffect::playOneShot()
    {
        auto& nextVoice = voices[nextVoiceIndex];
        if (IsSoundPlaying(nextVoice) && voiceExhaustionBehavior == DROP)
            return false;
        PlaySound(nextVoice);
        nextVoiceIndex = (nextVoiceIndex + 1) % voices.size();
        return true;
    }
}
