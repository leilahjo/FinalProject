//
// Created by greg8 on 2/27/2026.
//

#ifndef GAME_ENGINE_AUDIOTYPES_H
#define GAME_ENGINE_AUDIOTYPES_H

namespace Audio
{
    enum VoiceExhaustionBehavior
    {
        DROP, // Discard the incoming request.
        REPLACE // Start the oldest sound over again.
    };
}

#endif //GAME_ENGINE_AUDIOTYPES_H
