# Homework Instructions

1. Add an `EntityStatisticsSystem` that tracks the following information:
    1. Number of entities created this frame
    2. Number of entities destroyed this frame
    3. Number of entities ever created
    4. Number of entities ever destroyed
    5. Number of current entities

2. Update `Renderer` to display all of the data above, similarly to how it currently displays frame data.

3. Add a 6th "Number of ..." counter to your statitics system. This counter will
    a. Count the number of entities having a specific component. You should display the name of the component followed by the count.
    b. Once every 3 seconds, change the component being tracked. Skip components for which the entity count is 0. Make sure all components will eventually be iterated over.

4. Pressing the F1 key should show/hide all diagnostic output. That includes the frame data that was added in class and all of the entity statitistics you just added. Do **not** use the Raylib function `IsKeyPressed`. You will have to figure out how to emulate its behavior (reading the Raylib source code for hints is fine). 

### Code Style

Make sure you follow the [style guide](https://docs.google.com/document/d/1ik2bGHQSDmEYUMFrJsHvl4FQDzDaK-1RiZd-cGi014s/edit?usp=sharingODO) when writing your code.

## User Guide

*Write a short "user guide" for your work below (you are expected to modify this file). Minimally, you should write at least 1 paragraph describing your approach to the problem. Also include any inputs you've added or modified as part of the assignment, and exactly what they do.*

## Extra Credit
Worth up to 20% of the assignment point total. Can also be counted as 2.0 points toward the project "complexity score" if used in an appropriate way for your game.

The goal is to remove any reference to "archetype" from the `Game` code altogether. Instead, `World` should manage archetypes internally by finding or creating new archetypes based on the components now passed to `createEntity`.

Below you will find some refactoring steps to get you started. This list is not exhaustive.

1. Remove the `createArchetype` function from `World.h`.
2. Remove any archetype pointers from `Game.h`.
3. Update `createEntity` to take a component mask.
4. Update `Game.cpp` to pass a component mask into `createEntity` and remove all references to archetypes.

The rest is up to you. Think about what datastructure will be appropriate for maintaining efficient entity creation.

*NOTE: You do NOT need to worry about removal of empty archetypes resulting from entity removals.*