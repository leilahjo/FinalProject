//
// Created by greg8 on 1/27/2026.
//

#ifndef GAME_ENGINE_GAMEOBJECT_H
#define GAME_ENGINE_GAMEOBJECT_H

#include "raylib.h"

struct GameObject
{
    // World Coordinates
    float x = 0;
    float y = 0;
    float vx = 0;
    float vy = 0;

    // Do not want
    // float ax = 0;
    // float ay = 0;

    float width = 0.2;
    float height = 0.2;
    float radius = 0.2;

    // Move 1 game unit in 1 second.
    float speed = 0.5;


    Color color = GREEN;

    bool isRect = true;
};


#endif //GAME_ENGINE_GAMEOBJECT_H