//
// Created by greg8 on 3/7/2026.
//

#ifndef GAME_ENGINE_BEHAVIOR_H
#define GAME_ENGINE_BEHAVIOR_H
#include <filesystem>
#include <string>

#include "sol/sol.hpp"

namespace Scripting
{
    struct Behavior
    {
        bool load(sol::state& lua, const std::string& scriptPath);
        void reload(sol::state& lua);
        void unload();

    private:
        friend struct BehaviorManager;
        uint32_t generation = 1;
        sol::function updateFunction;

        std::string path;
    };
}

#endif //GAME_ENGINE_BEHAVIOR_H
