//
// Created by greg8 on 2/3/2026.
//

#ifndef GAME_ENGINE_KINEMATICSDATA_H
#define GAME_ENGINE_KINEMATICSDATA_H

#include <vector>
#include <cstdint>

struct KinematicsData
{
    std::vector<float> x, y, vx, vy;
    
    uint32_t create(float x, float y, float vx, float vy);
    
    size_t getCount() const { return x.size(); }
};


#endif //GAME_ENGINE_KINEMATICSDATA_H