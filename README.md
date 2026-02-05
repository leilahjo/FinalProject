# Homework Instructions

1. Add an acceleration component represented by two float vectors named `ax` and `ay`. 

2. Instantiate a new archetype that uses this component and use it only for the blue entities that are created when holding the space bar.

3. Update `KinematicsSystem` to use `ax` and `ay` to update `vx` and `vy` in the same that it already uses `vx` and `vy` to update `x` and `y`. *Don't forget to check that the archetype actually has the acceleration component!*

4. Initialize all new entities in this archetype with "gravity" in the form of some constant negative `ay`.

5. Add the following controls:
   - Pressing the "+" key **increases** gravity by some constant amount. *Holding* the key does not continue increasing gravity.
   - Pressing the "-" key **decreases** gravity by some constant amount. *Holding* the key does not continue decreasing gravity.

The same gravity should apply to all existing entities (for which it applies) and newly added entities!

**Important:** You must update the gravity values of every entity, not multiply by a separate scaling factor just before integrating. This is to give you practice with iterating over entities within an archetype.

*Note: in class I mentioned that storage for acceleration on a per-entity basis is unusual because acceleration tends to be either directly controlled by the game, constant accross many entities, or just 0. That's not in conflict with adding an acceleration component to an ECS. The nice thing about an ECS is that only the entities that are defined to use the acceleration component will incur the extra storage and computation cost.*

### Code Style

Make sure you follow the [style guide](https://docs.google.com/document/d/1ik2bGHQSDmEYUMFrJsHvl4FQDzDaK-1RiZd-cGi014s/edit?usp=sharingODO) when writing your code.

## User Guide

*Write a short "user guide" for your work below (you are expected to modify this file). Minimally, you should write at least 1 paragraph describing your approach to the problem. Also include any inputs you've added or modified as part of the assignment, and exactly what they do.*

## Extra Credit
Worth up to 10% of the assignment point total. Can also be counted as 1.0 points toward the project "complexity score" if used in an appropriate way for your game.

Make the space bar exert an outward "force" on all entities near the player. Nearby entities are those within 0.5 world units of the player. How you implement the force is up to you. Some options:

1. While the Q button is held down, modify the x and y coordinates of the nearby entities directly.
2. When the Q button is pressed down, modify the vx and vy components of the nearby entities.
3. When the Q button is pressed down, modify the ax and ay components of the nearby entities, but have them gradually revert to (0, -g)
