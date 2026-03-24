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
        voices.resize(1);
        voices[0] = LoadSound(path.c_str());

        voiceExhaustionBehavior = behavior;
    }

    bool SoundEffect::playOneShot()
    {
        PlaySound(voices[0]);
        return true;
    }
}
