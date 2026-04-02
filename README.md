# Homework Instructions

The goal is to have a Lua script that
1. Spawns 50 sheep at 5 seconds of **game** time
2. Spawns 50 chickens at 10 seconds of **game** time
3. Spawns 50 pigs at 10 seconds of **real** time

This means we'll be tying 3 concepts together: timing, Lua scripting, and prefabs. 

The following instructions break this goal down into high level tasks, but the tasks themselves are left for you to figure out.

1. Remove the `for` loops in `Game::onStart` that spawn 50 of each animal.
2. Add two fields to `FrameData`
   1. `double elapsedGameTimeS;` *the amount of **game** time, in seconds, since the engine started running. This must be a multiple of the number of `frame`s that have elapsed.*
   2. `double elapsedRealTimeS;` *the amount of **real** time, in seconds, since the engine started running. This is expected to drift very slightly from `elapsedGameTimeS` over time.*
3. Update `BehaviorManager` to pass both elapsed timers and the frame counter to Lua behavior scripts via a single new parameter: a struct that has those 3 fields.
4. Define a `PrefabProvider` struct, analogous to `EntityProvider`, that exposes a `spawnPrefab(string prefabName, float x, float y)` function. This new function should call `prefabManager.spawnPrefab`. Naturally this means that `PrefabProvider` will need to hold a reference to both `World` and `PrefabManager`.
5. Update `BehaviorManager` to bind the `spawnPrefab` function and pass the `PrefabProvider` to Lua behavior scripts.
6. Write a Lua script that spawns the correct prefabs at the correct times (see top of these instructions). Make sure your logic **guarentees** that exactly the number of animals specified are spawned no matter what. No more. No less. Spawn animals with random positions within [-2, 2], [-2, 2].
7. Add a new entity with only one component: `COMP_BEHAVIOR`. Attach the Lua script that spawns entities to this entity.
8. Test your work.

### Code Style

Make sure you follow the [style guide](https://docs.google.com/document/d/1ik2bGHQSDmEYUMFrJsHvl4FQDzDaK-1RiZd-cGi014s/edit?usp=sharingODO) when writing your code.

## User Guide

*Write a short "user guide" for your work below (you are expected to modify this file). Minimally, you should write at least 1 paragraph describing your approach to the problem. Also include any inputs you've added or modified as part of the assignment, and exactly what they do.*

## Extra Credit
*Worth up to 5% of the assignment point total.*

The addition of a new "provider" parameter for every engine subsystem we want scripts to interact with won't scale well from a software architecture standpoint.

To address that, rename `EntityProvider` to `EngineApi` and add the `spawnPrefab` function to it. Remove the `PrefabProvider`.