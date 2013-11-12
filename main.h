#ifndef ____main__
#define ____main__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <list>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "vector.h"
#include "boid.h"
#include "util.h"
#include "obstacle.h"
#include "flock.h"

#define PI 3.141592653589793238462643
#define ESCAPE_KEY 27
#define DEFAULT 1
#define TRAILING 2
#define SIDE 3
#define AREA 900
#define HEIGHT 100
#define TIMERMSECS 50
#define NUM_OBS 15

class Boid;
class Obstacle;
class Flock;

typedef std::list<Obstacle>::iterator ItObs;
extern std::list<Obstacle> obs;

/** Resize the viewing window **/
void resize(int width, int height);

/** Set lighting parameters **/
void makeLighting();

/** Set eye position for glLookAt **/
void setEye();

/** Initialize **/
void init();

/** Display callback function **/
void display();

/** Set position of camera **/
void makeCamera();

/** Keyboard callback function **/
void handleKeyboard(unsigned char key,int x, int y);

/** Special Keyboard callback function **/
void handleSpecialKeyboard(int key,int x, int y);

/** Timer callback function **/
void update(int value);

#endif