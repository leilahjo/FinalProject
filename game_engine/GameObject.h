//
// Created by greg8 on 1/27/2026.
//

#ifndef GAME_ENGINE_GAMEOBJECT_H
#define GAME_ENGINE_GAMEOBJECT_H

#include "KinematicsData.h"
#include "raylib.h"

namespace GameEngine
{
    struct GameObject
    {
    private:
        KinematicsData* kinematicsData;
        size_t index;

    public:
        GameObject(
            KinematicsData* kinematicsData,
            size_t index,
            float width,
            float height,
            float radius,
            float speed,
            Color color,
            bool isRect
        )
            : width(width)
              , height(height)
              , radius(radius)
              , speed(speed)
              , color(color)
              , isRect(isRect)
              , index(index)
              , kinematicsData(kinematicsData)
        {
        }

        // World Coordinates
        float& x() { return kinematicsData->x[index]; }
        float& y() { return kinematicsData->y[index]; }
        float& vx() { return kinematicsData->vx[index]; }
        float& vy() { return kinematicsData->vy[index]; }

        float width;
        float height;
        float radius;

        // World units per second
        float speed;

        Color color;
        bool isRect;
    };
}

#endif //GAME_ENGINE_GAMEOBJECT_H
