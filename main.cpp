#include "main.h"
using namespace std;

float alpha=45;
float beta =90;
int viewMode = DEFAULT;
bool hide = false, pauseScreen = false, adv = false;
int zaxis=0;
float zoom=1.0;
int w = 1200, h = 800;
float ratio= w/h;
vector trailing, side, center, midpoint;
std::list<Obstacle> obs;
Flock* flock;
int i;
float goal_acc = 1;

/** Resize window **/
void resize(int width, int height){
  if (h == 0) h = 1; //prevent a divide by zero, when window is too short
  glViewport(0,0,width,height); // viewport is the entire window
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30.0, width/(GLdouble)height,5.0,AREA);
  glMatrixMode(GL_MODELVIEW);
}

/** Set lighting parameters **/
void makeLighting() {
  // The specifications for 3 light sources
  GLfloat pos0[] = {0.0,0.0,0.9,0.0};      // w=0: infinite distance
  GLfloat dif0[] = {0.3,0.3,0.8,1.0};
  GLfloat pos1[] = {5.0,-5.0,0.0,0.0};   // Light from below
  GLfloat dif1[] = {0.4,0.4,0.4,1.0};    // Fainter
  GLfloat globalambient[] = {0.3,0.3,0.3,1.0};
  // Set ambient globally, default ambient for light sources 0
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT,globalambient);
  glLightfv(GL_LIGHT0,GL_POSITION,pos0);
  glLightfv(GL_LIGHT0,GL_DIFFUSE,dif0);
  glLightfv(GL_LIGHT1,GL_POSITION,pos1);
  glLightfv(GL_LIGHT1,GL_DIFFUSE,dif1);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
}

/** Set eye position **/
void setEye() {
  center = flock->centroid();
  midpoint = Mult(Add(center, (flock->getGoal())->getPos()), 0.5);
  vector u = Diff((flock->getGoal())->getPos(), center); // from C to G
  vector tmp = NullVector();
  tmp.x = u.x; tmp.y = u.y;
  vector p = Cross(u, tmp); // p is normal to u
  float test = Dot(u, (flock->getGoal())->getSpeed());
  if (test<0) {u = Diff(NullVector(),u); p = Diff(NullVector(),p);}
  float dist = Dist((flock->getGoal())->getPos(), center);
  float r = flock->radius()/4;
  // For trailing view
  vector norm = Normalize(u);
  trailing.x = center.x - norm.x*(dist+4*r)*zoom;
  trailing.y = center.y - norm.y*(dist+4*r)*zoom;
  trailing.z = center.z + (dist+0.5*r)*zoom;
  // For side view
  norm = Normalize(p);
  side.x = midpoint.x - norm.x*(dist+2*r)*zoom;
  side.y = midpoint.y - norm.y*(dist+2*r)*zoom;
  side.z = trailing.z;
}

/** Initialization **/
void init() {
  // Create environment
  glEnable(GL_DEPTH_TEST);
  glLineWidth(1.0);
  glPointSize(1.0);
  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
  glFrontFace(GL_CW);
  glDisable(GL_CULL_FACE);
  glClearColor(0.0,0.0,0.0,0.5);
  glEnable(GL_COLOR_MATERIAL);
  // Initialize objects
  for (i = 0; i<NUM_OBS; i++) {
    Obstacle* obstacle;
    if (i == 0) obstacle = new Obstacle(NullVector(), 20);
    else obstacle = new Obstacle(NewVec(rand()%1000-500,rand()%1000-500,10), 15+rand()%10);
    obs.push_back(*obstacle);
  }
  flock = new Flock();
  setEye();
}

/** Display **/
void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glPushMatrix();
  makeCamera();
  makeLighting();
  drawGround();
  drawTower();
  flock->drawBoids();
  glPopMatrix();
  glutSwapBuffers();
}

/** Set multiple views **/
void makeCamera() {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(40.0, ratio,5.0, AREA);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  switch(viewMode){
    case DEFAULT:
      gluLookAt(0,0,50+zaxis, midpoint.x,midpoint.y,midpoint.z, 0.0,0.0,1.0);
      break;
    case TRAILING:
      gluLookAt(trailing.x,trailing.y,trailing.z+zaxis, center.x,center.y, center.z, 0.0,0.0,1.0);
      break;
    case 3:
      gluLookAt(side.x,side.y,side.z+zaxis, midpoint.x, midpoint.y, midpoint.z, 0.0,0.0,1.0);
      break;
  }
}

/** Handle keyboard input from the user **/
void handleKeyboard(unsigned char key, int x, int y) {
  vector speed = (flock->getGoal())->getSpeed();
  switch (key) {
    case 'q':
    case 'Q':
    case ESCAPE_KEY: exit(0); break;
    case 'p':
    case 'P':
      pauseScreen = !pauseScreen;
      if (adv && !pauseScreen){
        adv = false;
        glutTimerFunc(TIMERMSECS,update, 1);} break;
    case 'd':
    case 'D':
      if (adv) glutTimerFunc(TIMERMSECS,update, 1);
      else adv = !adv; pauseScreen = true; break;
    case 'v':
      case 'V': goal_acc+=0.1; break;
    case 'b':
      case 'B': if(goal_acc>0.2) goal_acc-=0.1; break;
    case 'h': hide = !hide; break;
    case '1': viewMode = DEFAULT; break;
    case '2': viewMode = TRAILING; break;
    case '3': viewMode = SIDE; break;
    case '+':
    case '=': flock->addBoid(); break;
    case '-':
    case '_': if(flock->getSize()>3) flock->removeBoid(); break;
    case 'z': zaxis++; break;
    case 'Z': zaxis--; break;
    case 'x': if (zoom > 0) zoom -= 0.05;break;
    case 'X': if (zoom > 0) zoom += 0.05; break;
    case '0': zaxis = 0; zoom = 1.0; viewMode = DEFAULT;
    default:
      break;
  }
  glutPostRedisplay();
}

/** Handle special keyboard input from the user **/
void handleSpecialKeyboard(int key, int x, int y) {
  switch (key) {
    case GLUT_KEY_UP:    if (beta> 2) beta -=2;  break;
    case GLUT_KEY_DOWN:  if (beta<180) beta +=2;  break;
    case GLUT_KEY_LEFT:  alpha+=2; break;
    case GLUT_KEY_RIGHT: alpha-=2; break;
  }
  (flock->getGoal())->setSpeed(ratio*cos(PI*alpha/180),ratio*sin(PI*alpha/180),ratio*cos(beta*PI/180));
  glutPostRedisplay();
}

/** Update animations
 ** @param value TimerFunc parameter
 **/
void update(int value) {
  if(!pauseScreen||adv){
      //vector speed = (flock->getGoal())->getSpeed();
    ItObs itr;
    if (beta>91) beta-=0.1;
    if (beta<89) beta+=0.1;
    (flock->getGoal())->boundary();
    (flock->getGoal())->setSpeed(goal_acc*ratio*cos(PI*alpha/180), goal_acc*ratio*sin(PI*alpha/180), goal_acc*ratio*cos(beta*PI/180));
    flock->moveBoids();
    for (itr = obs.begin(); itr != obs.end(); itr++) (*itr).rotate();
  }
  setEye();
  if (!adv) glutTimerFunc(TIMERMSECS, update, value);
  else if(adv) {
    if (value >0) glutTimerFunc(TIMERMSECS, update, --value);
  }
  glutPostRedisplay();
}

/** MAIN FUNCTION
 **/
int main(int argc,char **argv) {
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowPosition( 50, 50 );
  glutInitWindowSize(w, h);
  glutCreateWindow("LOVELY BOIDS - JACY AND PEIYING'S VERY FIRST 3D ANIMATION!");
  glutReshapeFunc(resize);
  glutDisplayFunc(display);
  glutTimerFunc(TIMERMSECS, update, 1);
  glutKeyboardFunc(handleKeyboard);
  glutSpecialFunc(handleSpecialKeyboard);
  init();
  glutMainLoop();
  return(0);
}


