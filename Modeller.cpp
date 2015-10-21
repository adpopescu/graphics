/*******************************************************************
	       Scene Modelling Program
********************************************************************/

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <utility>
#include <vector>
#include "VECTOR3D.h"
#include "CubeMesh.h"
#include "QuadMesh.h"

void initOpenGL(int w, int h);
void display(void);
void reshape(int w, int h);
void mouse(int button, int state, int x, int y);
void mouseMotionHandler(int xMouse, int yMouse);
void keyboard(unsigned char key, int x, int y);
void functionKeys(int key, int x, int y);
VECTOR3D ScreenToWorld(int x, int y);

static int currentButton;
static unsigned char currentKey;

GLfloat light_position0[] = {-6.0,  12.0, 0.0,1.0};
GLfloat light_position1[] = { 6.0,  12.0, 0.0,1.0};
GLfloat light_diffuse[]   = {1.0, 1.0, 1.0, 1.0};
GLfloat light_specular[]  = {1.0, 1.0, 1.0, 1.0};
GLfloat light_ambient[]   = {0.2, 0.2, 0.2, 1.0};


// Cube Mesh Array variables and initialization
// ...
// also add a variable to keep track of current cube mesh

// Interaction State Variable
enum Action {TRANSLATE, ROTATE, SCALE, EXTRUDE, RAISE, SELECT, MULTIPLESELECT, DESELECT_ALL};
enum Action currentAction = TRANSLATE;

QuadMesh *floorMesh = NULL;
// Wall Mesh variables here
// ...

struct BoundingBox
{
  VECTOR3D min;
  VECTOR3D max;
} BBox;

// Default Mesh Size
int meshSize = 16;

int main(int argc, char **argv)
{
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Scene Modeller");

  initOpenGL(500,500);

  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(mouseMotionHandler);
  glutKeyboardFunc(keyboard);
  glutSpecialFunc(functionKeys);
  glutMainLoop();
  return 0;
}



// Setup openGL */
void initOpenGL(int w, int h)
{
  // Set up viewport, projection, then change to modelview matrix mode - 
  // display function will then set up camera and modeling transforms
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0,1.0,0.2,80.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  // Set up and enable lighting
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
  
  glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
  glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
  
  // Other OpenGL setup
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
  glClearColor(0.6, 0.6, 0.6, 0.0);  
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  // This one is important - renormalize normal vectors 
  glEnable(GL_NORMALIZE);
  
  //Nice perspective.
  glHint(GL_PERSPECTIVE_CORRECTION_HINT , GL_NICEST);
  
  // Set up meshes
  VECTOR3D origin  = VECTOR3D(-8.0f,0.0f,8.0f);
  VECTOR3D dir1v   = VECTOR3D(1.0f, 0.0f, 0.0f);
  VECTOR3D dir2v   = VECTOR3D(0.0f, 0.0f,-1.0f);
  floorMesh = new QuadMesh(meshSize, 16.0);
  floorMesh->InitMesh(meshSize, origin, 16.0, 16.0, dir1v, dir2v);
  
  VECTOR3D ambient = VECTOR3D(0.0f,0.0f,0.0f);
  VECTOR3D specular= VECTOR3D(0.0f,0.0f,0.0f);
  VECTOR3D diffuse= VECTOR3D(0.9f,0.5f,0.0f);
  float shininess = 0.0;
  floorMesh->SetMaterial(ambient,diffuse,specular,shininess);

  // Set up wall meshes
  // Make sure direction vectors are such that the normals are pointing into the room
  // Use the right-hand-rule (cross product) 
  // If you are confused about this, ask in class
  
  // Set up the bounding box of the room
  // Change this if you change your floor/wall dimensions
  BBox.min.Set(-8.0f, 0.0, -8.0);
  BBox.max.Set( 8.0f, 6.0,  8.0);
}



void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  glLoadIdentity();
  
  // Set up the camera
  gluLookAt(0.0,6.0,22.0,0.0,0.0,0.0,0.0,1.0,0.0);
  
  // Draw all cubes (see CubeMesh.h)
  //...
  
  // Draw floor and wall meshes
  floorMesh->DrawMesh(meshSize);

  glutSwapBuffers();
}


// Called at initialization and whenever user resizes the window */
void reshape(int w, int h)
{
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0,1.0,0.2,40.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

}

VECTOR3D pos = VECTOR3D(0,0,0);

// Mouse button callback - use only if you want to 
void mouse(int button, int state, int x, int y)
{
  currentButton = button;

  switch(button)
  {
  case GLUT_LEFT_BUTTON:
    if (state == GLUT_DOWN)
	{
	  ;
	  
	}
	break;
  case GLUT_RIGHT_BUTTON:
    if (state == GLUT_DOWN)
	{
	  ; 
	}
	break;
  default:
	break;
  }
  glutPostRedisplay();
}

// Mouse motion callback - use only if you want to 
void mouseMotionHandler(int xMouse, int yMouse)
{
  if (currentButton == GLUT_LEFT_BUTTON)
  {
	  ;
  }
  glutPostRedisplay();
}


VECTOR3D ScreenToWorld(int x, int y)
{
	// you will need this if you use the mouse
	return VECTOR3D(0);
}// ScreenToWorld()

/* Handles input from the keyboard, non-arrow keys */
void keyboard(unsigned char key, int x, int y)
{
  switch (key) 
  {
  case 't':
      currentAction = TRANSLATE;
	  break;
  case 's':
	  currentAction = SCALE;
	  break;
  case 'r':
	  currentAction = ROTATE;
	  break;
  case 'e':
	  currentAction = EXTRUDE;
	  break;
  case 'h':
	  currentAction = RAISE;
	  break;
  case 'c':
	  currentAction = SELECT;
	  break;
  case '+':
	  currentAction = MULTIPLESELECT;
	  break;
  case '-':
	  currentAction = DESELECT_ALL;
	  break;
  }
  glutPostRedisplay();
}

void functionKeys(int key, int x, int y)
{
  VECTOR3D min, max;

  if (key == GLUT_KEY_F1)
  {
    // Create and initialize new cube
    // becomes the currently selected cube
    
  }
  /*
  // Do transformation code with arrow keys
  // GLUT_KEY_DOWN, GLUT_KEY_UP,GLUT_KEY_RIGHT, GLUT_KEY_LEFT
  else if (...)
  {
  }
  */
  glutPostRedisplay();
}

