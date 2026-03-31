//
// Created by greg8 on 2/21/2026.
//

#ifndef ENGINE_RUNTIME_ENGINE_RUNTIME_H
#define ENGINE_RUNTIME_ENGINE_RUNTIME_H

#include "engine_core/EngineCore.h"
#include "rendering/Rendering.h"

#include "IGame.h"
#include "audio/AudioManager.h"
#include "input/InputManager.h"
#include "scripting/BehaviorManager.h"

namespace EngineRuntime
{
    static constexpr uint64_t TARGET_FPS = 60;
    static constexpr float FRAME_DT = 1.0 / TARGET_FPS;

    using namespace EngineCore;
    using namespace Rendering;
    using namespace Input;
    using namespace Audio;
    using namespace Scripting;

    struct Runtime
    {
        World world = World(FRAME_DT);
        Renderer renderer;
        AudioManager audioManager;
        InputManager inputManager;
        BehaviorManager behaviorManager;
        ArenaAllocator frameAllocator = ArenaAllocator(100 * 1024 * 1024); // 100 MB.

        FrameData frameData = {};

        int run(IGame& game, int windowWidth, int windowHeight, const std::string& windowTitle);
    };
}

#endif //ENGINE_RUNTIME_ENGINE_RUNTIME_H
