#include "util.h"

using namespace std;

/** Rule of Seperation.
 ** @param bj Pointer to the boid
 ** @return The velocity vector for Seperation
 **/
vector Seperation(Boid* bj) {
  vector c = NullVector();
  It itb = flock->getIter();
  List boids = flock->getBoids();
  for (itb = boids.begin(); itb!=boids.end(); itb++) {
    float d = 1 + Norm(Diff(bj->getPos(), (*itb).getPos()));
    if ((*itb).getID() != bj->getID() && d < 20)
      c = Add(c, Mult(Diff(bj->getPos(), (*itb).getPos()),1/(2*d*d)));
  }
  return c;
}

/** Rule of Alignment.
 ** @param bj Pointer to the boid
 ** @return The velocity vector for Alignment
 **/
vector Alignment(Boid* bj) {
  vector mspeed = NullVector();
  It itb = flock->getIter();
  List boids = flock->getBoids();
  Boid* tmp;
  for (itb = boids.begin(); itb!=boids.end(); itb++) {
    tmp = &(*itb);
    float d = 1 + Norm(Diff(bj->getPos(), tmp->getPos()));
    if (bj->getID() != tmp->getID() && d < 20)
      mspeed = Add (mspeed, Mult(tmp->getSpeed(),1/(d*d)));
  }
  mspeed = Mult(mspeed,(float(1)/float(boids.size()-1)));
  return Diff(mspeed,bj->getSpeed());
}

/** Rule of Cohesion.
 ** @param bj Pointer to the boid
 ** @return The velocity vector for Cohesion
 **/
vector Cohesion(Boid* bj){
  vector c = NullVector();
  It itb = flock->getIter();
  List boids = flock->getBoids();
  Boid* tmp;
  for (itb = boids.begin(); itb!=boids.end(); itb++) {
    tmp = &(*itb);
    float d = 1 + Norm(Diff(bj->getPos(), tmp->getPos()));
    if (bj->getID() != tmp->getID() && d < 20)
      c = Add(c, Mult(tmp->getPos(),1/(2*d*d)));
  }
  c = Mult(c,(float(1)/float(boids.size()-1)));
  return Diff(c,bj->getPos());
}

/** Rule of Moving Towards the Goal.
 ** @param bj Pointer to the boid
 ** @return The velocity vector towards the goal
 **/
vector TowardsGoal(Boid* bj) {
  return Diff((flock->getGoal())->getPos(), bj->getPos());
}

void avoidObstacles(Boid* bj){
  ItObs itr;
  float normS = Norm(bj->getSpeed());
  for (itr = obs.begin(); itr!=obs.end();itr++) {
    (*itr).setZ(bj->getPos().z);
    It itb;
    vector v, p, r, newV, dir;
    vector towardOb = Diff((*itr).getLoc(), bj->getPos());
    v = Normalize(bj->getSpeed());
    p = Mult(v,Dot(towardOb,v));
    r = Diff(p,towardOb);
    if (Dot(towardOb,bj->getSpeed())>0) {
      if (Norm(r)<(*itr).getRadius()&&Norm(towardOb)<1.5*(*itr).getRadius()) {
        if (Norm(r) != 0) {
          newV = Normalize(Add(towardOb, Mult(Normalize(r),2*(*itr).getRadius())));
        } else {
          dir = Cross(Diff(bj->getPos(), NullVector()),towardOb);
          newV = Normalize(Add(towardOb, Mult(Normalize(dir),2*(*itr).getRadius())));
        }
        newV = Mult(newV, normS);
        bj->setSpeed(newV.x, newV.y, newV.z);
      }
    }
  }
}

/** Rule for the predator to chase a random bird.
 ** @param p Pointer to the predator
 ** @return The velocity vector towards the random bird
 **/
vector pursue(Boid* p){
  List boids = flock->getBoids();
  vector v;
  int idx = rand()%(boids.size()+1)-1;
  It itb = boids.begin();
  for (int i = 0; i< idx; i++) itb++;
  v = Diff(itb->getPos(), p->getPos());
  v = Mult(Normalize(v), 5.0);
  return v;
}

/** Rule for a bird to avoid predator
 ** @param p Pointer to the predator
 ** @param b Pointer to the bird
 ** @return The velocity vector fleeing away from the predator
 **/
vector flee(Boid* p, Boid* b){
  float d = Dist(p->getPos(), b->getPos());
  vector newV = NullVector();
  vector v = Normalize(p->getSpeed());
  vector t = Diff(b->getPos(), p->getPos());
  vector proj = Mult(v,Dot(t,v));
  vector r = Diff(proj,t);
  if (d<20&&Norm(r)<15) {
    newV = Add(Diff(NullVector(),Normalize(r)),v);
    if (d!=0) newV = Mult(newV, 50/d*d);
    return newV;
  }
  return newV;
}

/** Draw tower **/
void drawTower(){
  ItObs itr;
  for (itr = obs.begin(); itr != obs.end(); itr++) (*itr).draw();
}

/** Draw ground **/
void drawGround(){
  int i;
  glColor3f(0.2,1.5,0.5);
  glBegin(GL_LINES);
  for (i=0;i<100;i++) {
    glVertex3f(AREA-20*i,-AREA,0.1);
    glVertex3f(AREA-20*i, AREA,0.1);
    glVertex3f(-AREA,AREA-20*i,0.1);
    glVertex3f( AREA,AREA-20*i,0.1);
  }
  glEnd();
  glColor3f(0.3,0.5,0.3);
  glBegin(GL_POLYGON);
  glVertex3f(-AREA,-AREA,0);
  glVertex3f(-AREA, AREA,0);
  glVertex3f( AREA, AREA,0);
  glVertex3f( AREA,-AREA,0);
  glEnd();
}

/** Draw bird
 ** @param x x position
 ** @param y y position
 ** @param z z position
 ** @param turn parameter to control the direction of the boid
 ** @param id indicator for GOAL, PREDATOR, or normal bird
 ** @param wing parameter to control flapping
 **/
void DrawBird(float x, float y, float z, float turn, int id, int wing) {
  float dz=0.2*cos(wing*PI/10);
  float dz1=0;
  float red=float(id);
  glColor3f(1,1,0);
  glTranslatef(x,y,z);
  glScalef(2,2,2);
  glRotatef(turn,0,0,1);
  if (id == GOAL) glColor3f(1,1,1);
  else if(id == PREDATOR) glColor3f(1.0, 0.0, 0.0);
  else glColor3f(0,1-red,red*red);
  glBegin(GL_POLYGON);
  glVertex3f(0.4,0,dz1-0.2);
  glVertex3f(0.6,0,dz1-0.2);
  glVertex3f(0.5,0.15,0-0.2);
  glEnd();
  if (id == GOAL) glColor3f(1,1,1);
  else if(id == PREDATOR) glColor3f(1.0, 0.0, 0.0);
  else glColor3f(0,1-red,red*red);
  glBegin(GL_POLYGON);
  glVertex3f(0.5,0.15,0-0.2);
  glVertex3f(0.4,0.6,0);
  glVertex3f(0.6,0.6,0);
  glEnd();
  glBegin(GL_POLYGON);
  glVertex3f(0.5,0.15,0-0.2);
  glVertex3f(0.5,0.6,-0.1);
  glVertex3f(0.6,0.6,0);
  glEnd();
  glBegin(GL_POLYGON);
  glVertex3f(0.5,0.15,0-0.2);
  glVertex3f(0.4,0.6,0);
  glVertex3f(0.5,0.6,-0.1);
  glEnd();
  if (id == GOAL) glColor3f(1,1,1);
  else if(id == PREDATOR) glColor3f(1.0, 0.0, 0.0);
  else glColor3f(0.8,red,0);
  glBegin(GL_POLYGON);
  glVertex3f(0.5,0.8,0);
  glVertex3f(0.4,0.6,0);
  glVertex3f(0.6,0.6,0);
  glEnd();
  if (id == GOAL) glColor3f(1,1,1);
  else if(id == PREDATOR) glColor3f(1.0, 0.0, 0.0);
  else glColor3f(1,1,red);
  glBegin(GL_POLYGON);
  glVertex3f(0.5,0.6,-0.1);
  glVertex3f(0.25,0.5,dz);
  glVertex3f(0.3,0.4,dz);
  glVertex3f(0.48,0.3,0-0.1);
  glEnd();
  glBegin(GL_POLYGON);
  glVertex3f(0.4,0.6,0);
  glVertex3f(0.25,0.5,dz);
  glVertex3f(0.3,0.4,dz);
  glVertex3f(0.48,0.3,0-0.1);
  glEnd();
  if (id == GOAL) glColor3f(1,1,1);
  else if(id == PREDATOR) glColor3f(1.0, 0.0, 0.0);
  else glColor3f(0,1-red,red*red);
  glBegin(GL_POLYGON);
  glVertex3f(0.25,0.5,dz);
  glVertex3f(0.3,0.4,dz);
  glVertex3f(0.1,0.3,1.5*dz);
  glEnd();
  if (id == GOAL) glColor3f(1,1,1);
  else if(id == PREDATOR) glColor3f(1.0, 0.0, 0.0);
  else glColor3f(1,1,red);
  glBegin(GL_POLYGON);
  glVertex3f(0.5,0.6,-0.1);
  glVertex3f(0.75,0.5,dz);
  glVertex3f(0.7,0.4,dz);
  glVertex3f(0.52,0.3,0-0.1);
  glEnd();
  glBegin(GL_POLYGON);
  glVertex3f(0.6,0.6,0);
  glVertex3f(0.75,0.5,dz);
  glVertex3f(0.7,0.4,dz);
  glVertex3f(0.52,0.3,0-0.1);
  glEnd();
  if (id == GOAL) glColor3f(1,1,1);
  else if(id == PREDATOR) glColor3f(1.0, 0.0, 0.0);
  else glColor3f(0,1-red,red*red);
  glBegin(GL_POLYGON);
  glVertex3f(0.75,0.5,dz);
  glVertex3f(0.7,0.4,dz);
  glVertex3f(0.9,0.3,1.5*dz);
  glEnd();
  glRotatef(-turn,0,0,1);
  glScalef(0.5,0.5,0.5);
  glTranslatef(-x,-y,-z);
}

/** Draw shadow for each boid
 ** @param x x position
 ** @param y y position
 ** @param z z position
 ** @param turn parameter to control the direction of the boid
 **/
void DrawShadow(float x, float y, float z, float turn) {
  glTranslatef(x,y,0);
  glScalef(2,2,2);
  glRotatef(turn,0,0,1);
  glColor3f(0.2,0.2,0.2);
  glBegin(GL_POLYGON);
  glVertex3f(0.4,0,0.1);
  glVertex3f(0.6,0,0.1);
  glVertex3f(0.5,0.15,0.1);
  glEnd();
  glBegin(GL_POLYGON);
  glVertex3f(0.5,0.15,0.1);
  glVertex3f(0.4,0.6,0.1);
  glVertex3f(0.6,0.6,0.1);
  glEnd();
  glBegin(GL_POLYGON);
  glVertex3f(0.5,0.15,0.1);
  glVertex3f(0.5,0.6,0.1);
  glVertex3f(0.6,0.6,0.1);
  glEnd();
  glBegin(GL_POLYGON);
  glVertex3f(0.5,0.15,0.1);
  glVertex3f(0.4,0.6,0.1);
  glVertex3f(0.5,0.6,0.1);
  glEnd();
  glBegin(GL_POLYGON);
  glVertex3f(0.5,0.8,0.1);
  glVertex3f(0.4,0.6,0.1);
  glVertex3f(0.6,0.6,0.1);
  glEnd();
  glBegin(GL_POLYGON);
  glVertex3f(0.5,0.6,0.1);
  glVertex3f(0.25,0.5,0.1);
  glVertex3f(0.3,0.4,0.1);
  glVertex3f(0.48,0.3,0.1);
  glEnd();
  glBegin(GL_POLYGON);
  glVertex3f(0.4,0.6,0.1);
  glVertex3f(0.25,0.5,0.1);
  glVertex3f(0.3,0.4,0.1);
  glVertex3f(0.48,0.3,0.1);
  glEnd();
  glBegin(GL_POLYGON);
  glVertex3f(0.25,0.5,0.1);
  glVertex3f(0.3,0.4,0.1);
  glVertex3f(0.1,0.3,0.1);
  glEnd();
  glBegin(GL_POLYGON);
  glVertex3f(0.5,0.6,0.1);
  glVertex3f(0.75,0.5,0.1);
  glVertex3f(0.7,0.4,0.1);
  glVertex3f(0.52,0.3,0.1);
  glEnd();
  glBegin(GL_POLYGON);
  glVertex3f(0.6,0.6,0.1);
  glVertex3f(0.75,0.5,0.1);
  glVertex3f(0.7,0.4,0.1);
  glVertex3f(0.52,0.3,0.1);
  glEnd();
  glBegin(GL_POLYGON);
  glVertex3f(0.75,0.5,0.1);
  glVertex3f(0.7,0.4,0.1);
  glVertex3f(0.9,0.3,0.1);
  glEnd();
  glRotatef(-turn,0,0,1);
  glScalef(0.5,0.5,0.5);
  glTranslatef(-x,-y,0);
}




