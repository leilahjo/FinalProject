//
// Created by greg8 on 1/27/2026.
//

#ifndef GAME_ENGINE_RENDERER_H
#define GAME_ENGINE_RENDERER_H
#include "timing.h"
#include "Game.h"

namespace GameEngine
{
    struct Renderer
    {
        static void DrawFrame(FrameData& frameData, Game::Game& game);
    };
}

#endif //GAME_ENGINE_RENDERER_H