//
// Created by greg8 on 1/27/2026.
//

#ifndef GAME_ENGINE_INPUTMANAGER_H
#define GAME_ENGINE_INPUTMANAGER_H


struct InputManager
{
    bool moveLeft = false;
    bool moveRight = false;
    bool moveUp = false;
    bool moveDown = false;

    void Update();
};


#endif //GAME_ENGINE_INPUTMANAGER_H