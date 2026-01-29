# Week 2 Homework Instructions

Add a "camera" that is controlled independently of the game objects in the game. The camera simply changes the view of the world, it is not a visible object within the world.

The camera should support panning and "latching". More explicitly, your camera should support all of the functionality below.

1. A "pan up" button.
2. A "pan left" button.
3. A "pan down" button.
4. A "pan right" button.
5. A "latch" button.
6. An "unlatch" button.

Use any keys you want but make sure you can still move the player independently of the camera.

Panning should be self explanatory: "pan up" means the camera moves up at a reasonable speed of your choosing. Don't forget to scale the camera's movement by `dt`.

"Latch" should immediately snap the camera to the "player" object we had in class and continue to follow it as the player moves.

"Unlatch" should decouple the camera and game object so it no longer follows the "player". The camera should retain its position at the time of unlatch. Panning in any direction should unlatch the camera. This should be the initial state of your camera (unlatched).

You are free to define any new classes, parameters, and functions as you see fit. If you decide to add a "Camera" class, remember to use the `game_engine` namespace to differentiate it from the Raylib camera class.

### Code Style

Make sure you follow the [style guide](https://docs.google.com/document/d/1ik2bGHQSDmEYUMFrJsHvl4FQDzDaK-1RiZd-cGi014s/edit?usp=sharingODO) when writing your code.

## User Guide

*Write a short "user guide" for your work below (you are expected to modify this file). Minimally, you should specify which keys you used for the 6 features above.*