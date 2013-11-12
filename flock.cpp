#include "flock.h"

using namespace std;

/** Constructor **/
Flock::Flock(){
  goal = new Boid(GOAL);
  size = 30; // initial size for the flock
  for (i=1; i<size; i++){
    Boid* boid = new Boid(i);
    boids.push_back(*boid);
  }
  predator = new Boid(PREDATOR);
}

/** Getters **/
int Flock::getSize(){ return size;}
It Flock::getIter(){ return itb; }
List Flock::getBoids(){ return boids;}
Boid* Flock::getGoal(){ return goal; }
Boid* Flock::getPredator(){ return predator; }

/** Add a random boid **/
void Flock::addBoid(){
  Boid* boid;
  if ((signed)boids.size()>size) {size++; return;}
  boid = new Boid(size);
  boids.push_back(*boid);
  size++;
}

/** Remove the first boid **/
void Flock::removeBoid(){
  boids.pop_front(); size--;
}

/** Draw the boids **/
void Flock::drawBoids(){
  itb = boids.begin();
  for (;itb != boids.end();itb++) (*itb).draw();
  predator->draw();
  if (!hide) goal->draw();
}

/** Move the boids **/
void Flock::moveBoids(){
  ItObs itr;
  float div = 1;
  predator->move(predator, div);
  goal->move(goal, div);
  for (itb = boids.begin(); itb != boids.end(); itb++){
    Boid* boid = &(*itb);
    for (itr = obs.begin(); itr != obs.end(); itr++) {
      if (Norm(Diff(boid->getPos(),(*itr).getLoc()))<(*itr).getRadius()) {
        div = 1/Norm(Diff(boid->getPos(),(*itr).getLoc()));
        break;
      }
    }
    boid->move(boid, div);
  }
}

/** Calculate the centroid of the flock
 ** @return the vector of the centroid
 **/
vector Flock::centroid(){
  itb = boids.begin();
  vector c = NullVector();
  for (;itb!=boids.end();itb++) {
    float d = Dist(goal->getPos(), (*itb).getPos());
    if (d < MAX_DIS){ // not to include boids too far away
      c.x += (*itb).getPos().x;
      c.y += (*itb).getPos().y;
      c.z += (*itb).getPos().z;
    }
  }
  return Mult(c,1.0/boids.size());
}

/** Calculate the radius
 ** @return the largest distance from any boid to the goal
 **/
float Flock::radius(){
  itb = boids.begin();
  float r = 0.0;
  for (;itb!=boids.end();itb++){
    float d = Dist(goal->getPos(), (*itb).getPos());
    if (d>r && d<MAX_DIS) r = d;
  }
  return r;
}
