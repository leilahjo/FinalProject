//
// Created by greg8 on 1/27/2026.
//

#ifndef GAME_ENGINE_GAMEOBJECT_H
#define GAME_ENGINE_GAMEOBJECT_H

struct GameObject
{
    // World Coordinates
    float x = 0;
    float y = 0;

    float width = 0.2;
    float height = 0.2;

    // Move 1 game unit in 1 second.
    float speed = 0.5;
};


#endif //GAME_ENGINE_GAMEOBJECT_H