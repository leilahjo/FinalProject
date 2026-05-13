//
// Marks entities with COMP_HP | COMP_STATE as destroyed when currentHp drops to zero.
// Runs after collision handling so damage applied that frame is reflected immediately.
//

#ifndef ENGINE_CORE_HPCHECKSYSTEM_H
#define ENGINE_CORE_HPCHECKSYSTEM_H

namespace EngineCore
{
    struct World;

    struct HpCheckSystem
    {
        void update(World& world);
    };
}

#endif //ENGINE_CORE_HPCHECKSYSTEM_H
