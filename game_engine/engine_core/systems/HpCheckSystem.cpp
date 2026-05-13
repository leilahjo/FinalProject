#include "HpCheckSystem.h"

#include "engine_core/World.h"
#include "engine_core/ecs/Archetype.h"
#include "engine_core/ecs/Entity.h"

namespace EngineCore
{
    void HpCheckSystem::update(World& world)
    {
        world.forEach(Archetype::COMP_HP | Archetype::COMP_STATE,
            [](Entity entity)
            {
                if (entity.currentHp() <= 0.0f)
                    entity.state() |= STATE_DESTROYED;
            });
    }
}
