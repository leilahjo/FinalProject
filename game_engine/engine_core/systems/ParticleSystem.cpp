#include "ParticleSystem.h"

#include "engine_core/World.h"
#include "engine_core/ecs/Archetype.h"
#include "engine_core/ecs/Entity.h"

namespace EngineCore
{
    void ParticleSystem::update(World& world)
    {
        world.forEach(Archetype::COMP_PARTICLE | Archetype::COMP_STATE,
            [&](Entity entity)
            {
                entity.particleLifetime() -= world.frameDt;

                // Fade alpha proportional to remaining lifetime
                if (entity.hasColor() && entity.particleMaxLifetime() > 0.0f)
                {
                    float t = entity.particleLifetime() / entity.particleMaxLifetime();
                    if (t < 0.0f) t = 0.0f;
                    entity.color().a = static_cast<unsigned char>(t * 255.0f);
                }

                if (entity.particleLifetime() <= 0.0f)
                    entity.state() |= STATE_DESTROYED;
            });
    }
}
