# Project Status Update

## 1. GitHub Link

https://github.com/leilah-s/arena-survival-gep

# 2.  Engine Features Completed

## HP Component and HpCheckSystem

Added `currentHp` and `maxHp` fields through `COMP_HP`. The `HpCheckSystem` checks entities with the HP component every frame and flags them for destruction when HP reaches zero or below.

This means the game itself never has to manually handle entity death since entities are automatically removed after taking enough damage.

---

## Particle Component and ParticleSystem

Added `COMP_PARTICLE` to track particle lifetime and maximum lifetime.

Every frame, the system:

* decreases particle lifetime
* fades particle alpha over time

Once the lifetime expires, the particle entity is automatically destroyed.

I tested this using particle burst effects and it handled around 10–16 particles per enemy death without issues.

---

## Audio System

The `AudioManager` now wraps raylib audio using a voice pool so multiple copies of the same sound can play at once.

There are two exhaustion modes:

* `DROP` skips playing a sound if all voices are busy
* `REPLACE` replaces the oldest active sound

Different sounds are now used for:

* shooting (`doot.wav`)
* hits (`hit.wav`)
* enemy deaths (`die.wav`)

---

## Lua Scripting via sol2

Implemented a `BehaviorManager` using Lua 5.4 and sol2.

Entities can now:

* be grouped by behavior ID
* update through Lua scripts every frame

Chaser enemies are fully controlled through `fauna.lua`, while the C++ AI system ignores entities that already have behavior components.

Hot reload support was also added with `F5`.

---

## Parallax Background Rendering

Added two scrolling dot-grid background layers with different parallax factors:

* `0.15`
* `0.35`

The layers:

* scroll automatically
* shift based on camera movement

This creates a depth effect while the camera follows the player.

---

## Multi-Threaded Render Loop

The game logic runs on a background thread while the main thread handles rendering.

Frames are passed through a triple-buffer system so rendering does not block the game thread.

A condition variable is used for startup synchronization.

`BlockTimer` output confirms that both threads run independently.

---

## Debug Render Overlay

Pressing `F1` toggles a debug overlay that renders:

* bounding boxes
* velocity arrows
* HP bars

The overlay uses render snapshot data, so no additional locking is required between threads.

---

## Type-Based Render Layering

Added a render layer system based on entity type IDs.

Render proxies are sorted before drawing so entities appear in the correct order:

* walls behind enemies
* enemies behind projectiles
* player on top

The system also works correctly with the parallax background layers.

---

## Arena Allocator

Implemented an arena allocator for:

* collision pair lists
* render proxy arrays

Since these structures are rebuilt every frame, the allocator:

* bumps a pointer forward during allocation
* resets at the end of each frame

This avoids repeated heap allocations.

Alignment is handled using `std::align`.

---

## Mouse World-Space Transform

The `InputManager` converts raw mouse screen coordinates into world-space coordinates using the renderer scale factor.

This keeps aiming accurate even when the camera moves away from the center.

---

## Circle-Circle Collision

Updated collision detection to support circle-circle collisions.

The collision system:

* checks collider shapes
* performs a radius-sum distance test for circles
* separates entities along the collision axis

---

## On-Screen HUD

Added a HUD that displays:

* HP bar
* wave number
* kill count
* survival time

When the player dies, a semi-transparent overlay appears with:

* final stats
* restart prompt

---

# Game Progress

Current gameplay features implemented:

* Player movement using WASD/arrow keys
* Mouse aiming
* Left-click shooting with cooldown
* Two enemy types:

  * Chasers: fast, low HP, controlled through Lua
  * Tanks: slower, higher HP, controlled through C++
* Waves scale over time using increasing enemy counts
* Short grace periods between waves
* Invincibility frames after taking damage
* ~40% pickup drop chance on enemy death
* Health pickups restore HP up to max value
* Particle effects on:

  * hits
  * kills
  * pickups
  * player death
* Camera drift to enhance parallax effect
* `R` resets the game after death

---

# Blockers

None currently.

---

# Profiling Experiment

I expected `collision.detect` to become the biggest performance issue during later waves because it uses a brute-force `O(n²)` collision loop.

I thought this would become noticeable once there were:

* many enemies
* particles
* projectiles

The engine already outputs `BlockTimer` statistics when a frame exceeds `16 ms`, so I added extra timing around collision detection using `steady_clock` and averaged the results over 300 frames.

## Collision Detection Timings

| Entity Count | Detect Time |
| ------------ | ----------- |
| ~30          | ~8 μs       |
| ~80          | ~19 μs      |
| ~200         | ~60 μs      |

The 200-entity case was artificially created and is much larger than normal gameplay conditions.

---

## Manhattan Pre-Cull Experiment

I tested a Manhattan-distance pre-cull to skip unnecessary square root calculations when entities were obviously too far apart to collide.

### Results

| Entities | Before | After  |
| -------- | ------ | ------ |
| ~30      | 8 μs   | 9 μs   |
| ~80      | 19 μs  | 17 μs  |
| ~200     | ~61 μs | ~52 μs |

The optimization helped slightly at very high entity counts but added small overhead at lower counts.

Since normal gameplay rarely exceeds around 80 entities and collision detection was already well below the frame budget, I decided the extra complexity was not worth keeping.

Most slow frames were actually caused by large particle bursts after wave clears instead of collision detection.


## 6. LLM Usage

I used Claude (claude.ai) as a debugging and planning advisor throughout this project. Per the course policy, my full conversation log is available at:

https://docs.google.com/document/d/1CWk7fJ8eNuc9AyUEJiiAwP76RSu4OLXjdatEYrwhlmE/edit?usp=sharing

Topics where I used the LLM:
- Debugging the SwapBuffer condition variable (missed `notify_one` placement)
- Planning the audio voice-pool architecture (DROP vs REPLACE tradeoffs)
- Understanding sol2's reference-passing semantics for the Lua Entity binding
- Reviewing the circle collision separation formula
