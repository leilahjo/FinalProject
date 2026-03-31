//
// Created by greg8 on 1/27/2026.
//

#ifndef INPUT_INPUTMANAGER_H
#define INPUT_INPUTMANAGER_H

namespace Input
{
    struct InputManager
    {
        bool keyLeft = false;
        bool keyRight = false;
        bool keyUp = false;
        bool keyDown = false;

        bool keySpace = false;

        bool keyF5Pressed = false;

        void update();
    };
}

#endif //INPUT_INPUTMANAGER_H
