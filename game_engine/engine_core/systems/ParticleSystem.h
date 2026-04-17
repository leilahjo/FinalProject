//
// Drives particle entities: ticks down lifetime, fades alpha, marks destroyed at zero.
// Particles need COMP_PARTICLE + COMP_STATE. Color fading also needs COMP_COLOR.
//

#ifndef ENGINE_CORE_PARTICLESYSTEM_H
#define ENGINE_CORE_PARTICLESYSTEM_H

namespace EngineCore
{
    struct World;

    struct ParticleSystem
    {
        void update(World& world);
    };
}

#endif //ENGINE_CORE_PARTICLESYSTEM_H
