# Homework Instructions

Our multi-threading has introduced a few problems related to the fact that Raylib is designed only to be called from the main thread. We're going to addres two of them:

### 1. Audio

1. **Add** two functions and a mutex to `AudioManager`'s API:
```
bool enqueueOneshot(SoundId soundId);
void playAllQueuedOneshots();
//...
std::mutex oneshotMutex;
```
2. **Remove** the `playOneShot` function from `AudioManager`
3. Replace the old calls to `AudioManager::playOneShot` with calls to `AudioManager::enqueueOneShot`
4. Implement the two new functions. Use a `FrameAllocator` or a `ScratchBuffer` to avoid heap allocations. You can use the same instance being used for `Collision`s, or a new instance. 
5. Call `playAllQueuedOneshots()` in an appropriate place. Remember that Raylib APIs are designed to be called from the "main" thread, and that does apply to `PlaySound`.

You should use the mutex to prevent concurrent access to the "queue" while minimizing the duration that locks are held.

### 2. Input

Think about how we could use two `InputManager`s to maintain these two invariants:

1. The `InputManager` state accesible to the engine thread should only change at a well defined function call (such as `inputManager.update()`). It should not change at arbitary points mid-frame.
2. For every true value returned from an invocation of Raylib's `IsKeyPressed`, the engine thread must process exactly that many "presses". 

Hint: Use an `atomic<bool>` for any "KeyDown" variables. `atomic<bool>` has an `load` function and a `store` function and these are the only functions you should use. See https://cplusplus.com/reference/atomic/atomic/

Hint: Use an `atomic<uint32_t>` for any "Pressed" variables. `atomic<uint32_t>` has an `exchange` function and a `fetch_add` function and these are the only functions you should use. See https://cplusplus.com/reference/atomic/atomic/

To help you test your solution, map a single press of the `A` button to the spawning of exactly one animated square. Then add the following line of code in an appropriate place in `runEngineLoop` to simulate a very slow engine loop. Test very rapid presses of `A` (more than 2 per 250 ms).

```
std::this_thread::sleep_for(std::chrono::milliseconds(250));
```

*When you are done testing please comment out this `sleep_for` call.*

*As you think about invariant 2 and what the assignment is asking you to do, you may discover a problem. This invariant is carefully worded. We are **not** concerned with the number of **physical** key presses since we are relying on Raylib for this course. In a production engine, you would be handling OS key events directly and avoid the problem*

### Code Style

Make sure you follow the [style guide](https://docs.google.com/document/d/1ik2bGHQSDmEYUMFrJsHvl4FQDzDaK-1RiZd-cGi014s/edit?usp=sharingODO) when writing your code.

## User Guide

*Write a short "user guide" for your work below (you are expected to modify this file). Minimally, you should write at least 1 paragraph describing your approach to the problem. Also include any inputs you've added or modified as part of the assignment, and exactly what they do.*

## Extra Credit
*Worth up to 10% of the assignment point total.*

Implement a lock-free `SwapBuffer`. You'll want to use a single `atomic<uint32_t>` and encode an index value and a boolean value within it. Explain why that is, along with how your general approach works. 

**Warning:** this is a challenging pedagogical exercise. It's only a minor performance boost for our engine and not remotely straightforward.