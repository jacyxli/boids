#ifndef ____boid__
#define ____boid__

#include <iostream>
#include <cstdlib>
#include <string>
#include <list>
#include "vector.h"
#include "util.h"
#include "main.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

extern float alpha, beta;
/** Indicators for boid type **/
#define GOAL 0
#define PREDATOR 99

/** A Boid class that stores each individual boid
 ** @author Jacy Li, Peiying Wen
 **/
class Boid{
private:
  vector pos;
  vector speed;
  int id;
  int wing;
public:
  /** Constructor **/
  Boid(int index);
  /** Getters **/
  vector getPos();
  vector getSpeed();
  int getID();
  /** Setters **/
  void setPos(GLdouble x, GLdouble y, GLdouble z);
  void setPos(vector p);
  void setSpeed(GLdouble x, GLdouble y, GLdouble z);
  /** Draw boid **/
  void draw();
  /** Update boid's movements **/
  void move(Boid* b, int div);
  /** Limit the moving area of the boid **/
  void boundary();
};

#endif
