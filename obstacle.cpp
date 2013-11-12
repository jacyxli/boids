#include "obstacle.h"

/** Constructor
 **/
Obstacle::Obstacle(){
  loc = NullVector(); radius = 0; angle = 0;
}
/** Constructor
 ** @param l
 ** @param r
 **/
Obstacle::Obstacle(vector l, float r){
    loc = l; radius = r; angle = 0;
}

/** Getters **/
float Obstacle::getRadius(){return radius; }
vector Obstacle::getLoc(){return loc; }
/** Setters **/
void Obstacle::setZ(float z){loc.z = z;}

/** Draw obstacles **/
void Obstacle::draw(){
    //draw teaPot at loc.x, loc.y and at the height of the radius
    glColor3f(1,1,1);
    glPushMatrix();
    glTranslatef(loc.x,loc.y,radius);
    glScalef(radius/2,radius/2,radius);
    glRotatef(90,1,0,0);
    glRotatef(angle, 0,1,0);
    glutWireTeapot(1);
    glTranslatef(-loc.x,-loc.y,-radius);
    glPopMatrix();
}

/** Rotate obstacles **/
void Obstacle::rotate(){ angle++; }
