# Homework Instructions

1. Add intra-layer ordering to `Renderer` by sorting by y coordinate. Within a layer (including the "unlayered" entities vector), entities with lower world-space y should be drawn later than entities with higher world-space y. It may be prudent to temporarily disable rock-on-rock collisions to make testing this easier, and give them a very low initial velocity limited to `0.01`. Make the intra-layer ordering configurable by having a `bool useIntraLayerOrdering[]` in `Renderer`, one boolean per layer, and a final `bool` for unlayered entities.

2. In class we replaced all `std::vector<Collision>` usages with `Collision*` and `size_t collisionCount`. We did this so we could use the `ArenaAllocator` thereby avoiding all heap allocation for collision processing. This works well from a performance perspective but it's inconvenient and error-prone. And we can make it even faster (see extra credit)!

    Define a `ScratchBuffer<T>` that wraps a pointer which is allocated by an `ArenaAllocator` passed in to its constructor. Minimally, you should implement all of the signatures below. Do this in `engine_core/memory_management/ScratchBuffer.h`.

    ```
    template<typename T>
    struct ScratchBuffer {
        // ScratchBuffer requires exclusive control of the allocator while elements are being appended.
        ScratchBuffer(std::unique_ptr<ArenaAllocator> allocator);

        // Returns true if there was sufficient space left in the allocator to append the desired element, otherwise false.
        bool append(const T& value);

        T& operator[](size_t i);
        const T& operator[](size_t i) const;

        size_t size() const;

        // When elements are no longer being added, relinquish control of the allocator.
        std::unique_ptr<ArenaAllocator> relinquishAllocator();
    private:
        // ...
    };
    ```

    Some guidance:
    - The general usage pattern for this scratch buffer should be
        1. Instantiate a scratch buffer which holds exclusive control over the underlying allocator while being used for appending items.
        2. Append a bunch of items to it.
        3. Relinquish control of the underlying allocator.
        4. Use the `ScratchBuffer` for **reads only** from now on.
    - Do not worry about C++ container semantics (Copy constructors, copy assignment operators, etc.), you can assume T is trivially copyable.


3. Replace all of the `Collision*` and `size_t collisionCount` pairs that we pass around with `ScratchBuffer<Collision>` instead.

    **Don't forget:** the point of using the arena allocator is to avoid heap allocations. Make sure you don't introduce new heap allocations (however small) as part of this assignment. Your solution should not call `new`, `delete`, or create any new `std::vector` growth in the collision-processing path.

### Code Style

Make sure you follow the [style guide](https://docs.google.com/document/d/1ik2bGHQSDmEYUMFrJsHvl4FQDzDaK-1RiZd-cGi014s/edit?usp=sharingODO) when writing your code.

## User Guide

*Write a short "user guide" for your work below (you are expected to modify this file). Minimally, you should write at least 1 paragraph describing your approach to the problem. Also include any inputs you've added or modified as part of the assignment, and exactly what they do.*

## Extra Credit
*Worth up to 5% of the assignment point total. Can also be counted as 0.5 points toward the project "complexity score" if used in an appropriate way for your game.*

You *can* still emulate the behavior of `vector` that doubles the "capacity" of the `ScratchBuffer` when `append` exceeds the current capacity. Write a paragraph explaining why we might want to and why it can be done without moving any data (unlike vector's implementation). Then do it. Be sure to handle allocator size limits correctly here.
