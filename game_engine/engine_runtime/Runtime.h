//
// Created by greg8 on 2/21/2026.
//

#ifndef ENGINE_RUNTIME_ENGINE_RUNTIME_H
#define ENGINE_RUNTIME_ENGINE_RUNTIME_H

#include <stop_token>

#include "engine_core/EngineCore.h"
#include "rendering/Rendering.h"

#include "IGame.h"
#include "audio/AudioManager.h"
#include "input/InputManager.h"
#include "scripting/Scripting.h"

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
        AudioManager audioManager;
        InputManager inputManager;
        BehaviorManager behaviorManager;
        PrefabManager prefabManager;

        int run(IGame& game, int windowWidth, int windowHeight, const std::string& windowTitle,
                std::optional<uint32_t> benchmarkFrameLimit = std::nullopt);

    private:
        Renderer renderer;

        uint64_t runEngineLoop(std::stop_token token, IGame& game, std::optional<uint32_t> benchmarkFrameLimit);
    };
}

#endif //ENGINE_RUNTIME_ENGINE_RUNTIME_H
