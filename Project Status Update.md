# Project Status Update

## 1. GitHub Link

https://github.com/leilah-s/arena-survival-gep

## 2. Engine Progress

Engine features completed:

- **HP Component and HpCheckSystem** — added `currentHp` and `maxHp` fields via `COMP_HP`. The `HpCheckSystem` just checks every entity with that component each frame and flags it destroyed if HP is zero or below. Means the game never has to manually handle entity death — it just does damage and the system takes care of the rest.

- **Particle Component and ParticleSystem** — `COMP_PARTICLE` tracks lifetime and max lifetime. Each frame the system ticks down lifetime and adjusts alpha proportionally. When it expires the entity gets flagged destroyed. I tested this with the burst spawner and it handles 10-16 particles per enemy death without any issues.

- **Audio System** — `AudioManager` wraps raylib audio with a voice pool so multiple copies of a sound can play at once. Two exhaustion modes: DROP just silently skips if all voices are busy, REPLACE steals the oldest one. Shoot, hit, and die effects each use a different file now (`doot.wav`, `hit.wav`, `die.wav`).

- **Lua Scripting via sol2** — `BehaviorManager` embeds Lua 5.4 and handles grouping entities by behavior ID, then calling the registered update function each frame with the entity list and a provider to look up other entities (e.g. the player). Chaser enemies are fully driven by `fauna.lua` now — the C++ AI loop skips any entity with a behavior component. Added a print in the script to confirm it actually runs. Hot-reload still works on F5.

- **Parallax Background Rendering** — two dot-grid layers at different parallax factors (0.15 and 0.35). Each one auto-scrolls over time and also shifts based on the viewport position, so as the camera drifts with the player the layers move at different rates. Looks pretty good in practice.

- **Multi-Threaded Render Loop** — game logic runs on a background thread and writes frames into a triple-buffer swap. The main thread just grabs the latest ready frame and draws it without waiting on the game thread. I used a condition variable for the initial sync. The `BlockTimer` output shows the two loops are clearly running independently.

- **Debug Render Overlay** — toggle with F1. Draws a bounding box around each entity, a velocity arrow, and an HP bar above anything that has the HP component. All of this goes through the render thread using the snapshot data so there's no extra locking needed.

- **Type-Based Render Layering** — there's a map from entity type ID to a layer number. When the frame is built, each proxy gets tagged with its layer and they get sorted before drawing. Walls come out behind enemies, enemies behind projectiles, player on top. Works correctly with the parallax layers behind everything.

- **Arena Allocator** — used for the collision pair list and the render proxy array, both of which get rebuilt every frame. The allocator just bumps a pointer and resets at the end of each frame instead of doing a bunch of small heap allocations. Handles alignment with `std::align`.

- **Mouse World-Space Transform** — `InputManager` converts the raw mouse pixel position to world units using the same scale factor the renderer uses. The aim direction is correct even when the viewport has drifted from center.

- **Circle-Circle Collision** — `CollisionSystem::detect` checks the collider shape and picks the right test. Circle pairs use a radius-sum distance check. The separation pushes both entities apart along the contact axis.

- **On-Screen HUD** — HP bar (goes green → yellow → red), wave number, kill count, and time survived are drawn each frame. When the player dies a semi-transparent overlay comes up with the final stats and a prompt to restart with R.

## 3. Game Progress

- Player movement with WASD/arrows, mouse aim, left-click to shoot with a short cooldown — all working.
- Two enemy types: Chasers (small, fast, low HP) driven by `fauna.lua` via the Lua behavior system; Tanks (bigger, slow, higher HP) still using C++ AI.
- Waves escalate each round — wave N sends 3 + 2N chasers and N-1 tanks. Short grace period at the start and between waves.
- Invincibility frames for 0.8s after any hit.
- ~40% pickup drop on enemy death; pickups restore 30 HP capped at max.
- Particle bursts on hit, kill, pickup, and player death.
- Camera drifts slightly toward the player position for visible parallax effect.
- R to restart after death — clears all entities, resets everything, spawns fresh.

## 4. Blockers

None currently.

## 5. Profiling Experiment

My hunch going in was that `collision.detect` was the most likely thing to blow up as the game gets further into later waves — it's a brute-force O(n²) loop, so I figured it'd start hurting somewhere around wave 4 or 5 when there can be 20+ enemies plus particles and projectiles all at once.

The engine already spits out `BlockTimer` stats when a frame goes over 16 ms, so I piggybacked on that and also wrapped the detect call in a `steady_clock` measurement and averaged it over 300 frames at fixed entity counts.

Numbers I got (very roughly — there was some variance run to run):

| Entity count | detect time |
|---|---|
| ~30 | ~8 μs |
| ~80 | ~19 μs |
| ~200 | ~60-ish μs |

The ~200 case I had to fake by spawning a ton of enemies at once; that's not really a realistic wave count for this game.

I tried adding a quick Manhattan pre-cull before the actual circle test — basically skip the sqrt entirely if the bounding boxes are obviously not overlapping:

```cpp
float mdx = std::abs(entityA.x() - entityB.x());
float mdy = std::abs(entityA.y() - entityB.y());
float maxR = (std::min(entityA.width(), entityA.height()) +
              std::min(entityB.width(), entityB.height())) * 0.5f;
if (mdx > maxR || mdy > maxR)
    continue;
```

With the pre-cull:

| Entities | before | after |
|---|---|---|
| ~30 | 8 μs | 9 μs |
| ~80 | 19 μs | 17 μs |
| ~200 | ~61 μs | ~52 μs |

Honestly the gains were pretty underwhelming. It helped a bit at 200 entities but added a tiny overhead at 30 (probably just branch prediction noise). At the entity counts this game actually hits — call it 80ish at peak — the difference is basically noise and the detect time is already tiny compared to the 16 ms frame budget anyway.

I ended up reverting it. Not worth the extra code for a few microseconds when the real bottleneck in a slow frame was something else entirely (usually the particle burst after a wave clears).

## 6. LLM Usage

I used Claude (claude.ai) as a debugging and planning advisor throughout this project. Per the course policy, my full conversation log is available at:

https://docs.google.com/document/d/1CWk7fJ8eNuc9AyUEJiiAwP76RSu4OLXjdatEYrwhlmE/edit?usp=sharing

Topics where I used the LLM:
- Debugging the SwapBuffer condition variable (missed `notify_one` placement)
- Planning the audio voice-pool architecture (DROP vs REPLACE tradeoffs)
- Understanding sol2's reference-passing semantics for the Lua Entity binding
- Reviewing the circle collision separation formula
