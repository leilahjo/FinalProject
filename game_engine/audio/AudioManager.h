//
// Created by greg8 on 11/26/2025.
//

#ifndef GAME_ENGINE_AUDIOPLAYER_H
#define GAME_ENGINE_AUDIOPLAYER_H

#include <string>
#include <vector>

#include "engine_core/ecs/ECSTypes.h"
#include "SoundEffect.h"

namespace Audio
{
    using namespace EngineCore;

    struct AudioManager
    {
        void initialize();

        SoundId loadAudioAsset(const std::string& path, uint8_t maxVoices, VoiceExhaustionBehavior behavior);
        bool playOneshot(SoundId soundId);

    private:
        std::vector<SoundEffect> soundEffects;
    };
}


#endif //GAME_ENGINE_AUDIOPLAYER_H
