# Homework Instructions

We'll add real-time spacial attenuation and stereo panning to "doot" sound playback in the class repo.

1. Add a `PlaybackId` that uniquely identifies a playing voice. Update `AudioManager::playOneshot` to return it. It will need a sensical `INVALID` value. 

    A `PlaybackId` must uniquely identify
    - One *specific voice instance,*
    - belonging to one *specific sound effect.*

It should not contain a raylib `Sound` directly or transitively, it ony *identifies* one.

2. Declare and define a `AudioManager::setSoundVolume(PlaybackId, float volume)` which calls raylib's `SetSoundVolume(Sound, float volume)` appropriately.

    You will need to resolve the correct `voice` within the correct `SoundEffect` using your `PlaybackId`.

3. Declare and define a `AudioManager::setSoundPan(PlaybackId, float pan)` which calls raylib's `SetSoundPan(Sound, float pan)` appropriately.

    Again, you will need to resolve the correct `voice` within the correct `SoundEffect` using your `PlaybackId`.

4. Update `Game::onUpdatePostCollisions` to use `AudioManager::SetSoundVolume` to implement linear distance attenuation such that a "doot" at exactly the player's position has volume 1, while a "doot" at the (largest-dimension) edge of the screen has volume 0. You do not need to continuously update the volume, just set it once, when playback starts.

5. Update `Game::onUpdatePostCollisions` to use `AudioManager::SetSoundpan` so that the "doot" sound pans linearly.
    - At the left edge of the screen, pan = 0.0f.
    - At the right edge of the screen, pan = 1.0f.
    - At intermediate X values, pan should interpolate linearly.
    - You do not need to continuously update the pan, just set it once, when playback starts.

*Note: You might wonder exactly what position to use for representing the position of the sound emission. You can choose any of: the midpoint of the two colliding squares, or either square's center.* 

### Code Style

Make sure you follow the [style guide](https://docs.google.com/document/d/1ik2bGHQSDmEYUMFrJsHvl4FQDzDaK-1RiZd-cGi014s/edit?usp=sharingODO) when writing your code.

## User Guide

*Write a short "user guide" for your work below (you are expected to modify this file). Minimally, you should write at least 1 paragraph describing your approach to the problem. Also include any inputs you've added or modified as part of the assignment, and exactly what they do.*

## Extra Credit
*Worth up to 5% of the assignment point total. Can also be counted as 0.5 points toward the project "complexity score" if used in an appropriate way for your game.*

There are quite a few patterns common to both `Sprite` and `SoundEffect`, as well as `SpriteManager` and `AudioManager`. Use polymorphism or composition to reduce the redundancy.