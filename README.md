# Boids: 3D Flocking Simulation

This program simulates the behavior of a 3D flock of synthetic birds (or boids). Boids show emergent behaviors according to basic rules of separation, alignment, and cohesion, and the flock always follows a steering goal. They are also able to avoid obstacles and predators. 

## Features (and Keyboard shortcuts)
**Goal Control:**
- left: turn left
- right: turn right
- up: ascend along z
- down: descend along z
- v: slow down
- b: speed up

**Multiple Views:	**
- 0: reset to default settings
- 1: default view
- 2: trailing view
- 3: side view
- Z: zoom down
- z: zoom up
- X: zoom in
- x: zoom out
- p/P: pause/resume
- d/D: advance one single step
- q/Q/escape: quit
- +/=: create a new boid
- -/_: remove a random boid

**Extra Features:**
- Flapping: animated boids so that they flap their wings.
- Shadows: Drew a shadow of each boid on the ground. 
- Obstacles: Added a number of obstacles(3-D Teapot) in random locations with randomized size. When an obstacle is present, the boids split up to avoid the obstacle. And they will regroup in a natural way once they have cleared the obstacle.
- Predator: Implemented a predator boid that fly around randomly. The regular boids will avoid the predator at all costs.
- Lighting: there is a very simple lighting setup so the teapots look nicer.

## Compile
Standard Compilation - use make (make clean removes unnecessary files).

    make
    ./boids
    
## Files
-README
- C++ files: main.cpp vector.cpp boid.cpp obstacle.cpp util.cpp
- Header files: main.h vector.h boid.h obstacle.h util.h
- Executable C program: boids
- Makefile

This program is divided into four separate files

1) vector.cpp
This is our self-constructed vector library.

2) obstacle.cpp
This class contains a vector of the obstacle location, and the radius. The angle is the variable to keep obstacle spinning.

3) boid.cpp
This class contains the position and the speed of the boid. The integer id stores the value to identify the type of the void (e.g. goal, predator, regular boids). It also provides methods for drawing and moving the void.

4) flock.cpp
This class contains a list of boids, including the steering goal, the predator, and regular birds. It also provides methods for calculating the centroid and radius of the flock, adding and deleting birds, and drawing/moving the flock.

5) util.cpp
This is the utility file that stores all the rules for boid kinematics and the drawing functions. 

6) main.cpp
This is where the main() is at. For user-interactive purpose it handles situations with mouse and keyboard input, sets the game speed and difficulty, and controls the evolving of the gaming process.

## Notes:
There are still certain problems with viewing in special occasions.
