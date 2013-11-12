#ifndef ____util__
#define ____util__

#include <iostream>
#include <cstdlib>
#include <string>
#include <list>
#include "main.h"
#include "obstacle.h"
#include "vector.h"
#include "boid.h"
#include "flock.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class Boid;
class Obstacle;
class Flock;

extern int size;
extern Flock* flock;

/** Rule 1: Seperation.
 The boids should attempt to maintain a certain minimum separation distance from local flockmates to avoid overcrowding. **/
vector Seperation(Boid* bj);

/** Rule 2: Alignment.
 Move at roughly the same speed and steer towards the same heading as the local flockmates. **/
vector Alignment(Boid* bj);

/** Rule 3: Cohesion.
 Remain together as a group - move towards the average position (center of mass) of the local flock. **/
vector Cohesion(Boid* bj);

/** Rule 4: Moving towards the goal **/
vector TowardsGoal(Boid* bj);

/** Rule 5: Avoiding obstacles **/
void avoidObstacles(Boid* bj);

/** Rule for the predator to follow the birds **/
vector pursue(Boid* b);

/** Rule for the individual bird to avoid predator **/
vector flee(Boid* p, Boid* b);

/** Draw a series of observation towers **/
void drawTower();

/** Draw ground **/
void drawGround();

/** Draw individual bird **/
void DrawBird(float x, float y, float z, float turn, int id, int wing);

/** Draw the shadow of each bird **/
void DrawShadow(float x, float y, float z, float turn);

#endif 
