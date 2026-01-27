//
// Created by greg8 on 1/27/2026.
//

#ifndef GAME_ENGINE_RENDERER_H
#define GAME_ENGINE_RENDERER_H
#include "timing.h"
#include "GameObject.h"


struct Renderer
{
public:
    static void DrawFrame(const FrameData& frameData, const GameObject& gameObject);
};

#endif //GAME_ENGINE_RENDERER_H