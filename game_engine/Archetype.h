//
// Created by greg8 on 2/3/2026.
//

#ifndef GAME_ENGINE_ARCHETYPE_H
#define GAME_ENGINE_ARCHETYPE_H

#include <vector>
#include <cstdint>

#include "ECSTypes.h"
#include "raylib.h"

namespace GameEngine
{
    struct Archetype
    {
        using ComponentMask = uint32_t;
        enum Components : ComponentMask
        {
            COMP_POSITION = 1 << 0,
            COMP_VELOCITY = 1 << 1,
            COMP_SIZE = 1 << 2,
            COMP_COLOR = 1 << 3
        };

        //Fixed per archetype.
        ComponentMask componentMask;

        Archetype(ComponentMask mask);

        // Position Component
        std::vector<float> x, y;

        // Velocity Component
        std::vector<float> vx, vy;

        // Size Component
        std::vector<float> width, height;

        // Color Component
        std::vector<Color> color;

        ArchIndex create(float x, float y, float vx, float vy, float width, float height, Color color);

        bool hasPosition() { return componentMask & COMP_POSITION; }
        bool hasVelocity() { return componentMask & COMP_VELOCITY; }
        bool hasSize() { return componentMask & COMP_SIZE; }
        bool hasColor() { return componentMask & COMP_COLOR; }

        size_t getCount() const { return n; }

    private:
        size_t n;
    };
}


#endif //GAME_ENGINE_ARCHETYPE_H