//
// Created by greg8 on 2/3/2026.
//

#include "KinematicsData.h"

namespace GameEngine
{
    uint32_t KinematicsData::create(float x, float y, float vx, float vy)
    {
        this->x.push_back(x);
        this->y.push_back(y);
        this->vx.push_back(vx);
        this->vy.push_back(vy);
        return this->x.size() - 1;
    }
}
