//
// Created by greg8 on 2/15/2026.
//

#ifndef GAME_ENGINE_STARFIELD_H
#define GAME_ENGINE_STARFIELD_H

#include "engine_core/EngineCore.h"

namespace Game
{
    struct Game;
    using namespace EngineCore;

    struct StarField
    {
        Archetype* archetype;

        void init(Game& game, World& world);
        void enforceBoundary(const Game& game, const World& world);

    private:
        void createStar(Game& game, World& world);
    };
}


#endif //GAME_ENGINE_STARFIELD_H
