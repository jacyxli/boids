#ifndef ____Obstacle__
#define ____Obstacle__

#include <iostream>
#include "vector.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/** A Obstacle class that stores the location and radius of the obstacle
 ** @author Jacy Li, Peiying Wen
 **/
class Obstacle{
private:
    vector loc;
    float radius;
    float angle; 
public:
    /** Constructor **/
    Obstacle(); //default constructor
    Obstacle(vector l, float r); //initialize loc and radius
    /** Getters **/
    float getRadius();
    vector getLoc();
    /** Setters **/
    void setZ(float z);
    /** Draw Obstacle **/
    void draw();
    /** Rotate Obstacle along z-axis **/
    void rotate();
};

#endif 
