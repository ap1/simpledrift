// ---------------------------------------------------
//  Written by Anjul Patney in 2012
// ---------------------------------------------------

#include <iostream>
#include <cstdio>
#include <cstdlib>

#include <GL/glew.h>
#include <GL/glut.h>

#include "common_inline.h"
#include "vecs.h"
#include "car.h"

#include <windows.h>

// for memory debugging
// http://vld.codeplex.com/
// #include <vld.h>

using namespace std;


// ---------------------------------------------------
// structs
// ---------------------------------------------------
struct tmstate{
  vec2f lastpos;
  bool leftclicked;
  bool rightclicked;
};


// ---------------------------------------------------
// Function Prototypes 
// ---------------------------------------------------
void init(int argc, char* argv[]);
void mainDisplay();
void mainKeypress(unsigned char key, int x, int y);
void mainMousepress(int button, int state, int x, int y);
void mainMousemove(int x, int y);
void mainResize(int width, int height);
void idle();
void set_camera();
void setVSync(int interval);

// ----------------------------------------
// global variables
// ----------------------------------------

nissan_370z c1;

// window pointers
int mainWindow = 0;

// window size
int W = 800, H = 800;

// mouse state
tmstate mouse;

// camera angles
float theta, phi, camdist;

// has anything in the scene changed?
bool sceneChanged=true;

// when did we start rendering?
clock_t frameStartTime=0;
clock_t frameCounterStartTime=clock();
int frameCounter=-1;

bool debDraw = true;

// ---------------------------------------------------
// Function Definitions
// ---------------------------------------------------


int main(int argc, char* argv[]){

  frameStartTime = clock();

  TIME_SO_FAR("Initializing");
  init(argc, argv);

  TIME_SO_FAR("Starting main loop");
  glutMainLoop();

}



void init(int argc, char* argv[]){

  // initialize OpenGL
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(W, H);

  // create main window
  glutInitWindowPosition(300, 0);
  mainWindow = glutCreateWindow("main Renderer");
  glutSetWindow(mainWindow);

  glutDisplayFunc  (mainDisplay);
  glutReshapeFunc  (mainResize);
  glutKeyboardFunc (mainKeypress);
  glutMouseFunc    (mainMousepress);
  glutMotionFunc   (mainMousemove);

  glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

  glutIdleFunc(idle);

  glewInit();

  set_camera();

  // disable vsync
  setVSync(0);
}

void idle(){
}

void set_camera(){

  glutSetWindow(mainWindow); 

  // camera
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //gluPerspective(curSc_->cam().fovy()*180.0f/M_PI, curSc_->cam().aspect(), 0.1f, 10000.0f);
  gluOrtho2D(-W/100, W/100, -H/100, H/100);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

}

// ----------------------------------------
// Main Display
// ----------------------------------------
void mainDisplay(){
  frameStartTime = clock();

  // Main rendering code
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  float bodytheta   = 45.0f * M_PI / 180.0f;
  float wheeltheta  =  0.0f * M_PI / 180.0f;

  c1.draw(vec2f(0.0f,0.0f), vec2f(-sin(bodytheta),cos(bodytheta)),
                            vec2f(-sin(wheeltheta),cos(wheeltheta
                            )));

  glutSwapBuffers();

  // check the FPS
  if(frameCounter== -1) {
    frameCounter = 0;
    frameCounterStartTime = clock();
  }

  frameCounter++;

  clock_t frameEndTime = clock();

  float timeSinceCounter = ((float)(frameEndTime - frameCounterStartTime))/(float)CLOCKS_PER_SEC;

  if(timeSinceCounter > 2.0f){
    float fps = (float)frameCounter / timeSinceCounter;
    frameCounter = 0;
    frameCounterStartTime = clock();
    printf("%0.2f fps\n",fps);
  }

  glutPostRedisplay();

  //sceneChanged = false;
}

// ----------------------------------------
// Main Resize
// ----------------------------------------
void mainResize(int width, int height){
  W=width;
  H=height;
  glViewport(0,0,W,H);
}

// ----------------------------------------
// Main Mousepress
// ----------------------------------------
void mainMousepress(int button, int state, int x, int y){
  mouse.lastpos=vec2f(x,y);
  mouse.leftclicked=false;
  mouse.rightclicked=false;

  if(button==GLUT_LEFT_BUTTON){
    mouse.leftclicked=true;
  }else if(button==GLUT_RIGHT_BUTTON){
    mouse.rightclicked=true;
  }
}


// ----------------------------------------
// Main Mousemove
// ----------------------------------------
void mainMousemove(int x, int y){
  vec2f del = vec2f(x,y) - mouse.lastpos;
  if(del.mag()>100.0f) del = vec2f(0,0);

  del*=0.01f;

  if(mouse.leftclicked){
    theta+=del.y();
    phi  +=del.x();

    if(theta<=M_PI*0.01f)   theta=M_PI*0.01f;
    if(theta>=M_PI*0.99f)   theta=M_PI*0.99f;
  }else if(mouse.rightclicked){
    camdist += (del.y()*camdist*0.2f);
  }

  mouse.lastpos = vec2f(x,y);

  set_camera();

  glutSetWindow(mainWindow); 
  glutPostRedisplay();

  sceneChanged=true;
}

// ----------------------------------------
// Main Keypress
// ----------------------------------------
void mainKeypress(unsigned char key, int x, int y){

  float movestep=0.05f;
  switch(key){
    case 'D':
    case 'd':
      debDraw = !debDraw;
      mainKeypress(' ',0,0);
      break;
    case ' ':
      sceneChanged=true;
      glutSetWindow(mainWindow);
      glutPostRedisplay();
      break;
    case 27:
      exit(0);
      break;
  };
}



#ifndef UNIX

typedef BOOL (APIENTRY *PFNWGLSWAPINTERVALFARPROC)( int );
PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = 0;

void setVSync(int interval=1)
{
  const char *extensions = (char*)glGetString( GL_EXTENSIONS );

  if( strstr( extensions, "WGL_EXT_swap_control" ) == 0 )
    return; // Error: WGL_EXT_swap_control extension not supported on your computer.\n");
  else
  {
    wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress( "wglSwapIntervalEXT" );

    if( wglSwapIntervalEXT )
      wglSwapIntervalEXT(interval);
  }
}
#else
void setVSync(int interval=1)
{
}
#endif