//
// Created by greg8 on 2/3/2026.
//

#ifndef ENGINE_CORE_RANDOM_H
#define ENGINE_CORE_RANDOM_H
#include <cstdint>

namespace EngineCore
{
    void setSeed(uint32_t seed);
    float randomFloat(float min, float max);
}

#endif //ENGINE_CORE_RANDOM_H
