//
// Created by greg8 on 1/27/2026.
//

#ifndef GAME_ENGINE_ENTITY_H
#define GAME_ENGINE_ENTITY_H

#include "Archetype.h"
#include "ECSTypes.h"
#include "raylib.h"

namespace GameEngine
{
    // Represents an entity in the game world. The struct itself is a lightweight view
    // into the arrays of an archetype.
    struct Entity
    {
        Archetype* archetype;
        EntityIndex entityIndex;

        // Position
        float& x() { return archetype->x[entityIndex]; }
        float& y() { return archetype->y[entityIndex]; }

        // Velocity
        float& vx() { return archetype->vx[entityIndex]; }
        float& vy() { return archetype->vy[entityIndex]; }

        // Size
        float& width() { return archetype->width[entityIndex]; }
        float& height() { return archetype->height[entityIndex]; }

        Color& color() { return archetype->color[entityIndex]; }
    };
}

#endif //GAME_ENGINE_GAMEOBJECT_H
