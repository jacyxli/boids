#ifndef ____flock__
#define ____flock__

#include <iostream>
#include <cstdlib>
#include <string>
#include <list>
#include "main.h"
#include "obstacle.h"
#include "vector.h"
#include "boid.h"

#define MAX_DIS 120

using namespace std;

extern bool hide;
extern int i;

class Boid;
class Obstacle;

typedef std::list<Boid>::iterator It;
typedef std::list<Boid> List;

/** A Flock class that stores each a list of boids
 ** @author Jacy Li, Peiying Wen
 **/
class Flock {
private:
  It itb;
  List boids;
  Boid *goal;
  Boid *predator;
  int size;
public:
  /** Constructor **/
  Flock();
  /** Getters **/
  int getSize();
  Boid* getGoal();
  Boid* getPredator();
  It getIter();
  List getBoids();
  /** Calculate the centroid **/
  vector centroid();
  /** Calculate the radius **/
  float radius();
  /** Add a random boid **/
  void addBoid();
  /** Remove the first boid **/
  void removeBoid();
  /** Draw boids **/
  void drawBoids();
  /** Update movement for boids **/
  void moveBoids();
};

#endif
