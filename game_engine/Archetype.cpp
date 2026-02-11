//
// Created by greg8 on 2/3/2026.
//

#include "Archetype.h"

namespace GameEngine
{
    Archetype::Archetype(ComponentMask mask) : componentMask(mask), n(0)
    {
    }

    EntityIndex Archetype::createEntity(float x, float y,
                                  float vx, float vy,
                                  float width, float height,
                                  Color color,
                                  Entity::State state)
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

        if (componentMask & COMP_STATE)
        {
            this->state.push_back(state);
        }

        return n++;
    }

    bool Archetype::removeEntity(EntityIndex entityIndex)
    {
        if (entityIndex >= n)
            return false;
        EntityIndex lastIndex = n - 1;

        if (componentMask & COMP_POSITION)
        {
            x[entityIndex] = x[lastIndex];
            x.pop_back();
            y[entityIndex] = y[lastIndex];
            y.pop_back();
        }

        if (componentMask & COMP_VELOCITY)
        {
            vx[entityIndex] = vx[lastIndex];
            vx.pop_back();
            vy[entityIndex] = vy[lastIndex];
            vy.pop_back();
        }

        if (componentMask & COMP_SIZE)
        {
            width[entityIndex] = width[lastIndex];
            width.pop_back();
            height[entityIndex] = height[lastIndex];
            height.pop_back();
        }

        if (componentMask & COMP_COLOR)
        {
            color[entityIndex] = color[lastIndex];
            color.pop_back();
        }
        
        if (componentMask & COMP_STATE)
        {
            state[entityIndex] = state[lastIndex];
            state.pop_back();
        }
        
        n--;
        return true;
    }
}
