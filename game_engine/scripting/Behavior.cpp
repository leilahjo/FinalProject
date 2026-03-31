//
// Created by greg8 on 3/7/2026.
//

#include "Behavior.h"
#include <iostream>

namespace Scripting
{
    bool Behavior::load(sol::state& lua, const std::string& scriptPath)
    {
        path = scriptPath;

        sol::protected_function_result result = lua.safe_script_file(
            scriptPath,
            &sol::script_pass_on_error // Pass error back rather than throwing it.
        );

        if (!result.valid())
        {
            sol::error err = result;
            std::cerr << "Lua Load Error for " << scriptPath << ": " << err.what() << "\n";
            return false;
        }

        updateFunction = result;
        return true;
    }

    void Behavior::reload(sol::state& lua)
    {
        load(lua, path);
    }

    void Behavior::unload()
    {
        updateFunction = sol::nil;
        generation++;
    }
}
