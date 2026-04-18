# Arena Survival
*Leilah*

## Description

Arena Survival is a top-down single-screen shooter where the player moves with WASD, aims with the mouse, and fires with left-click. Enemies come in waves and chase the player. There are two enemy types: fast chasers with low HP and slower tanks with more HP. Enemies drop health pickups sometimes when they die. When the player's HP hits zero the game ends and shows a stats overlay; press R to restart without closing the window.

On the engine side I extended the class ECS with several new features. I added an HP component and system, a particle component and system with alpha fade, an audio manager with voice-pooling and separate sound effects, a Lua scripting system via sol2 for hot-reloadable enemy behaviors, a parallax background with multiple layers at different scroll rates, a multi-threaded game and render loop with a triple-buffer swap, a toggleable debug overlay, type-based draw ordering, an arena allocator for per-frame scratch memory, and mouse world-coordinate input.

## Game Tasks
*Complexity Point Subtotal: 15*

#### Task 1 — Core Arena Game Loop — 4 Complexity Points

WASD and arrow keys for movement, mouse aim, left-click to shoot with a cooldown, invincibility frames after getting hit, and clamping to keep everything inside the arena boundary. The player entity needs Position, Velocity, Size, Collider, HP, Animation, Sprite, and Rotation all at once, which is a good test for whether the ECS handles archetypes with a lot of components.

#### Task 2 — Wave Manager — 3 Complexity Points

Enemies come in waves where wave N spawns 3 + 2N chasers and N-1 tanks from random positions around the arena edges. There's a short grace period before wave 1 and between waves. The manager tracks how many enemies are still alive and only starts the next wave when the count hits zero. This keeps the entity lifecycle (spawn, track, kill) working consistently across multiple rounds.

#### Task 3 — Particle Burst Feedback — 2 Complexity Points

Whenever something noteworthy happens — enemy hit, enemy killed, player hit, pickup collected — a burst of small square particles flies out. Colors are different depending on what happened: orange for damage, green for heals, red for player death. It's mostly just visual feedback but it also validates that ParticleSystem can handle bursts of 10-16 entities at a time without problems.

#### Task 4 — Health Pickups — 2 Complexity Points

Enemies have about a 40% chance to drop a small pickup when they die. Pickups sit in their own collision layer and heal the player when touched, capped at max HP. This needed a fourth collision layer (player, enemy, projectile, pickup) and made sure the layer filtering works correctly.

#### Task 5 — On-Screen HUD — 2 Complexity Points

Draws an HP bar that shifts from green to yellow to red as HP drops, plus current wave, kill count, and survival time. On death a semi-transparent overlay shows final stats and a restart prompt. The HUD data goes through the existing render frame path so it doesn't need any extra synchronization with the render thread.

#### Task 6 — Soft Camera Drift — 1 Complexity Point

The viewport position tracks a small fraction of the player's world position (about 12%). The arena stays fully visible but the shift is enough to make the parallax layers move relative to each other as the player moves around.

#### Task 7 — In-Game Restart — 1 Complexity Point

Pressing R after death marks every entity for destruction, runs cleanup, resets all the game variables, and respawns the walls and player. No window close needed.

## Engine Tasks
*Complexity Point Subtotal: 28*

#### Task 1 — HP Component and System — 2 Complexity Points

`COMP_HP` stores `currentHp` and `maxHp` per entity. `HpCheckSystem` runs each frame and sets `STATE_DESTROYED` on anything that's hit zero. This way the game just writes to `currentHp` and the system handles the rest — entities don't get special-cased, they go through the normal `World::cleanup` path like everything else.

#### Task 2 — Particle Component and System — 3 Complexity Points

`COMP_PARTICLE` stores `particleLifetime` and `particleMaxLifetime`. `ParticleSystem` ticks the lifetime down each frame and fades the alpha as it goes. When it hits zero it flags the entity as `STATE_DESTROYED`. The game just calls `spawnBurst` and doesn't need to think about cleanup at all — the system takes care of it.

#### Task 3 — Audio System — 4 Complexity Points

`AudioManager` keeps a pool of `SoundEffect` objects where each one pre-loads N copies of the same sound for polyphonic playback. When a new oneshot comes in and all voices are busy, the behavior depends on the exhaustion mode — DROP just skips it, REPLACE steals the oldest voice. There's also music streaming with a fade-in and fade-out that works by adjusting the volume incrementally each frame. All audio calls happen on the engine thread only.

#### Task 4 — Lua Scripting with sol2 — 5 Complexity Points

`BehaviorManager` runs a Lua 5.4 interpreter via sol2. Each frame it groups entities by their behavior ID and calls the registered Lua update function with the entity list and an `EntityProvider` so the script can look up other entities (like the player). A C++ helper `applyFleeBehavior` is also exposed to Lua for cases where doing the loop in C++ is faster. Chaser enemies use `fauna.lua` for movement — the C++ AI skips anything with a behavior component. Hot-reload works at runtime with F5.

#### Task 5 — Parallax Background Rendering — 3 Complexity Points

`addParallaxLayer` registers a dot-grid background layer with a given color, parallax factor, and spacing. Each layer's offset combines two things: it shifts based on the current viewport position scaled by the parallax factor (so farther layers move less), and it auto-scrolls over time at the same rate (gives the background some movement even when the player is still). Two layers at different factors give a decent depth feel.

#### Task 6 — Multi-Threaded Game and Render Loops — 4 Complexity Points

The game logic thread writes completed frames into a triple-buffer swap and the render thread reads from the latest ready frame. A condition variable handles the initial sync so the render thread doesn't spin while waiting for the first frame. Entity data is only ever touched on the game thread; the renderer only reads the snapshot. I had a bug early on where the condition variable wasn't being notified in the right place, which caused the render thread to hang on startup.

#### Task 7 — Debug Render Overlay — 2 Complexity Points

F1 toggles per-entity debug overlays: a bounding box around each entity, a short line showing the velocity direction, and a colored HP bar above anything with the HP component. The toggle goes through the render frame snapshot so the render thread doesn't need any extra locking to read it.

#### Task 8 — Type-Based Render Layering — 2 Complexity Points

There's a map from entity type ID to a draw-order integer. When building a frame, each proxy gets tagged with its layer number and then the list gets sorted by layer before drawing. Walls come out behind enemies, projectiles draw in front of enemies, player draws on top. The sort is stable so entities at the same layer stay in consistent order.

#### Task 9 — Arena Allocator for Per-Frame Scratch Memory — 2 Complexity Points

`ArenaAllocator` is used for two scratch allocations that happen every frame: the collision pair list in `CollisionSystem::detect` and the RenderProxy array in `RenderFrame`. Both get wiped at the start of each frame with `resetFully`. The allocator handles alignment with `std::align` and is non-copyable so you can't accidentally alias it.

#### Task 10 — Mouse World-Space Coordinate Transform — 1 Complexity Point

`InputManager` converts the raw mouse pixel position into world units using the same scale factor the renderer computes from the viewport settings. The result accounts for the current viewport offset, so aim direction stays accurate when the camera has drifted.
