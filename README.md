# Homework Instructions

1. **Add a `PING_PONG` `AnimationPlaybackMode`.**

In the `PING_PONG` playback mode, when `progress` reaches or exceeds `1`, it should then *decrease* each frame until it reaches or goes below `0`, then increase again back toward `1`, and so on indefinitely.

*Hint: Add a bool `playingBackward` (or similar) to `AnimationData` to track whether the animation is currently moving “forward” or “backward” through time. This should only be used by the `PING_PONG` playback mode.*

A "pingpong.png" image has been added that you will use to test your implementation. You must use the "pingpong.png" file for an animated sprite with at least one entity in your game.

2. **Implement the 4 directional idle and running animations in the provided sprite sheets.**

*Note: this part of the assignment is unrelated to part 1 above.*

For this part, try to use the provided engine as-is: you do not need to modify any engine code. This is a chance to practice using the engine as a game developer would. This means you'll be working mostly in *Game.cpp* and any new constructs you come up with. 

At a minimum, your game logic should:
- Switch between idle and run animations based on keyboard input.
- Use the correct directional animation (e.g., idle-up, run-right, etc.) based on keyboard input. There are 8 possible movement directions when combining inputs (up and left for example) but only 4 directions provided in the sprite sheet so use the convention of prefering left/right animations in those cases. 

### Code Style

Make sure you follow the [style guide](https://docs.google.com/document/d/1ik2bGHQSDmEYUMFrJsHvl4FQDzDaK-1RiZd-cGi014s/edit?usp=sharingODO) when writing your code.

## User Guide

*Write a short "user guide" for your work below (you are expected to modify this file). Minimally, you should write at least 1 paragraph describing your approach to the problem. Also include any inputs you've added or modified as part of the assignment, and exactly what they do.*

## Extra Credit
*Worth up to 10% of the assignment point total. Can also be counted as 1.0 points toward the project "complexity score" if used in an appropriate way for your game.*

Recall that we made the blue squares flash pink when colliding with other blue squares. We implemented this in Game.cpp by manipulating the color component of entities in Game.cpp. 

**Replace** this animation with a completely new type of animation: a permanent "wobble". The blue squares should always wobble rather than start animating in response to a collision. You will still use the frameless animation data's `progress()`. You'll want to use a `LOOP` animation playback mode, and the animation should be continuous with no perceptible "jumps" or "snaps" in the animation.

Specifications:
1. When `progress()` is 0, `width()` should be maximized and `height()` should be minimized.
2. When `progress()` is 1, `width()` should be minimized and `height()` should be maximized.
3. Between 0 and 1, width and height should interpolate.

The magnitude of the wobble is up to you and can be hard coded in Game.cpp. Just make the wobble obvious visually. 

Note that linear interpolation will work, but it will not look as good as a "sprung" wobble using **sinusoidal interpolation**. For full credit you must implement "sprung" wobble.