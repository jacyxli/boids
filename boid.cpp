#include "boid.h"

using namespace std;

/** Constructor
 ** @param index the index of each boid
 **/
Boid::Boid(int index){
  if (index == GOAL ){
    speed = NewVec(0.1, 0.1, 0.0);
    pos = NewVec(-400, -400, 30);
  } else if (index == PREDATOR) {
    pos = NewVec(rand()%500/1.0-250,rand()%500/1.0-250,rand()%500/1.0-250);
    speed = NewVec(rand()%100/100.0-0.5,rand()%100/100.0-0.5,rand()%100/100.0-0.5);
  } else{
    speed = NewVec(rand()%100/200.0-0.25,rand()%100/200.0-0.25,rand()%100/200.0-0.25);
    pos = NewVec((float)(rand()%50)/2-450, (float)(rand()%50)/2-450, 20+rand()%7/3);
  }
  wing = rand()%360;
  id = index;
}

/** Getters **/
vector Boid::getPos(){ return pos;}
vector Boid::getSpeed(){ return speed;}
int Boid::getID(){ return id;}
/** Setters **/
void Boid::setSpeed(GLdouble x, GLdouble y, GLdouble z) {
  speed.x = x; speed.y = y; speed.z = z;
}
void Boid::setPos(GLdouble x, GLdouble y, GLdouble z){
  pos.x = x; pos.y = y; pos.z = z;
}
void Boid::setPos(vector p){ pos = p;}

/** draw the boid **/
void Boid::draw(){
  float turn;
  float norma = sqrt(speed.x*speed.x+speed.y*speed.y);
  float turn1 = acos(speed.x/norma)*180/PI;
  float turn2 = asin(speed.y/norma)*180/PI;
  turn = turn1;
  if (turn2<0) turn=-turn1;
  turn = turn-90;
  glDisable(GL_LIGHTING);
  DrawBird(pos.x,pos.y,pos.z,turn,id,wing);
  DrawShadow(pos.x,pos.y,pos.z,turn);
  glEnable(GL_LIGHTING);
}

/** Limit the moving area of the boid **/
void Boid::boundary(){
  if (pos.x<-AREA || pos.x> AREA) { alpha=180-alpha; pos.x=895*pos.x/AREA; }
  if (pos.y<-AREA || pos.y> AREA) { alpha=360-alpha; pos.y=895*pos.y/AREA; }
  if (pos.z<  10) { beta =90 ; pos.z=10; }
  if (pos.z>HEIGHT) { beta =95 ; pos.z=HEIGHT; }
}

/** Move the boid
 ** @param b the boid
 ** @param div velocity coefficient
 **/
void Boid::move(Boid* b, int div){
  vector tmp;
  vector v[5];
  float w[5];
  if (b->getID() == PREDATOR) {
    w[0] = 0; w[1] = 0; w[2] = 0.002; w[3] = 0; w[4] = 2;
    v[4] = Mult(pursue(b), w[4]);
  } else if (b->getID() == GOAL){
    w[0] = 0; w[1] = 0; w[2] = 0; w[3] = 0; w[4] = .01/div;
    v[4] = NullVector();
  } else{
    w[0] = 0.01/div; w[1] = 0.005/div; w[2] = 0.0005/div; w[3] = 0.01/div; w[4] = .01/div;
    v[4] = Mult(flee(flock->getPredator(), b), w[4]);
  }
  v[0] = Mult( Seperation(b), w[0]);
  v[1] = Mult( Alignment(b) , w[1]);
  v[2] = Mult( Cohesion(b), w[2]);
  v[3] = Mult( TowardsGoal(b),w[3]);
  tmp = b->getSpeed();
  for (int i = 0; i< 5; i++) tmp = Add(tmp, v[i]);
  float norm = Norm(tmp);
  if (norm > 1)   tmp = Mult(tmp,1/norm);
  if (norm < 0.1) tmp = Mult(tmp,0.2/(norm+0.01));
  b->setSpeed(tmp.x, tmp.y, tmp.z);
  avoidObstacles(b);
  pos = Add(pos,speed);
  wing = wing+1;
}


