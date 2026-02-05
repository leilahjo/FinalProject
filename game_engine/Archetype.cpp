//
// Created by greg8 on 2/3/2026.
//

#include "Archetype.h"

namespace GameEngine
{
    Archetype::Archetype(ComponentMask mask) : componentMask(mask), n(0)
    {
    }

    uint32_t Archetype::create(float x, float y,
                               float vx, float vy,
                               float width, float height, Color color)
    {
        if (componentMask & COMP_POSITION)
        {
            this->x.push_back(x);
            this->y.push_back(y);
        }

        if (componentMask & COMP_VELOCITY)
        {
            this->vx.push_back(vx);
            this->vy.push_back(vy);
        }

        if (componentMask & COMP_SIZE)
        {
            this->width.push_back(width);
            this->height.push_back(height);
        }

        if (componentMask & COMP_COLOR)
        {
            this->color.push_back(color);
        }

        return n++;
    }
}
