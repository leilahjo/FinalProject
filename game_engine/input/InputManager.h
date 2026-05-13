//
// Created by greg8 on 1/27/2026.
//

#ifndef INPUT_INPUTMANAGER_H
#define INPUT_INPUTMANAGER_H

namespace Input
{
    struct InputManager
    {
        // Keyboard
        bool keyLeft  = false;
        bool keyRight = false;
        bool keyUp    = false;
        bool keyDown  = false;
        bool keySpace = false;
        bool keyF5Pressed = false;

        // Debug-render toggle (F1 key)
        bool keyF1Pressed = false;

        // Restart key
        bool keyRPressed = false;

        // Mouse buttons
        bool mouseLeft        = false; // held down
        bool mouseLeftPressed = false; // pressed this frame

        // Mouse position in screen pixels
        float mouseScreenX = 0.0f;
        float mouseScreenY = 0.0f;

        // Mouse position in world units — needs viewport info passed in from the renderer
        float mouseWorldX = 0.0f;
        float mouseWorldY = 0.0f;

        // Call every frame. viewportX/Y come from the last rendered frame so world-space
        // conversion matches what the player actually sees.
        void update(float viewportX = 0.0f, float viewportY = 0.0f, float viewportScale = 2.0f);
    };
}

#endif //INPUT_INPUTMANAGER_H
