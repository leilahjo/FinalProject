//
// Created by greg8 on 1/27/2026.
//

#ifndef GAME_ENGINE_GAMEOBJECT_H
#define GAME_ENGINE_GAMEOBJECT_H

#include "Archetype.h"
#include "ECSTypes.h"
#include "raylib.h"

namespace GameEngine
{
    struct Entity
    {
        Archetype* archetype;
        ArchIndex archIndex;

        // World Coordinates

        // Position
        float& x() { return archetype->x[archIndex]; }
        float& y() { return archetype->y[archIndex]; }

        // Velocity
        float& vx() { return archetype->vx[archIndex]; }
        float& vy() { return archetype->vy[archIndex]; }

        // Size
        float& width() { return archetype->width[archIndex]; }
        float& height() { return archetype->height[archIndex]; }

        Color& color() { return archetype->color[archIndex]; }
    };
}

#endif //GAME_ENGINE_GAMEOBJECT_H
