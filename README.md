# Homework Instructions

We'll implement Circle-Circle and Circle-Rect collision detection and resolution.

1. Start by changing the initial 10 moving squares to circles. Add a `renderShape` component and use it in all archetypes. This should be an `enum struct` with `CIRCLE` and `RECT`.
    
    To keep things simple, we'll assume entities are using collider shapes, render shapes, and width/height in a consistent way. That is, "circle entities" will always have a `CIRCLE` collider shape and render shape, and will have the same width and height (and you should adhere to this when creating entities). 
    
    With that said, it will still help readability to add a `float radius()` function to Entity.h that returns half the width. Do so.
  
    You'll need to update the `Renderer` to call `DrawCircle` as appropriate.

2. Add Circle-Circle collision detection. This should happen immediately after the AABB check we did in class. *Hint: circles overlap if the distance between their centers is less than the sum of their radii.*.
3. Add Circle-Rect collision detection. This should happen immediately after the AABB check we did in class. Here are some helpful tips:
    1. You can determine the closest point on the rect to the circle center as follows:
        ```
        float closestX = std::clamp(circle.x(), rect.left(), rect.right());
        float closestY = std::clamp(circle.y(), rect.bottom(), rect.top());
        ```
    2. The circle and rect intersect if the distance from that point is less than the radius. See if you can do this without using square roots (which are relatively slow). This is a classic optimization in "game math". 
    
    3. The approach above ignores the case of a circle being contained completely within a rect. Find a simple way to handle that case.
4. Add Circle-Circle collision resolution. The idea is simply to separate the moveable circle(s) along the line joining their centers, then reflect the velocities about that line (aka "normal"). Here are some code fragments you might find useful:

```
    float dx = b.x() - a.x();
    float dy = b.y() - a.y();
    float dist = std::sqrt(dx * dx + dy * dy);
    // Normal from A to B
    float nx = dx / dist;
    float ny = dy / dist;
```

```
    // Reflect velocity about normal:
    float vx = a.vx();
    float vy = a.vy();
    float vn = vx * nx + vy * ny; // component along normal (a dot product)
    // Only reflect velocities if A is not already moving away from B.
    if (vn < 0.0f) {
        a.vx() = vx - 2.0f * vn * nx;
        a.vy() = vy - 2.0f * vn * ny;
    }
```

5. Add Circle-Rect collision resolution. The idea is to move the circle and rect away from each other along the line joining the cicrle's center and the closest point on the rect (calculated above in part 3), and reflect velocties about that line (as done in part 4 above).

**Warning:** Square roots are relatively expensive computation in collision detection algorithms. You should be able to avoid all square roots in the circle-circle and circle-rect collision **detection** code. Using a square root there will result in a minor, 2 point penalty.

**Note:** Accurate collision resolution is hard and solution may have rare edge cases. Only > 50% collisions need to look "realistic". Less than that may receive a penalty.

### Code Style

Make sure you follow the [style guide](https://docs.google.com/document/d/1ik2bGHQSDmEYUMFrJsHvl4FQDzDaK-1RiZd-cGi014s/edit?usp=sharingODO) when writing your code.

## User Guide

*Write a short "user guide" for your work below (you are expected to modify this file). Minimally, you should write at least 1 paragraph describing your approach to the problem. Also include any inputs you've added or modified as part of the assignment, and exactly what they do.*

## Extra Credit

*Worth up to 20% of the assignment point total. Can also be counted as 2.0 points toward the project "complexity score" if used in an appropriate way for your game.*

Implement sort and sweep in CollisionSystem::detect. You do not need to persist ordering across frames and you only need to implement 1 dimension (x or y)! 

*Hint: run an O(N log(N)) sorting algorithm (you can use a library) on each of the `colliderEntities` vectors. Then update the entity traversal loop to take advantage of the ordering.*
