/*******************************************************************
	       Scene Modelling Program
********************************************************************/

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>
#include <utility>
#include <vector>
#include <list>
#include <iterator>
#include <iostream>
#include "VECTOR3D.h"
#include "CubeMesh.h"
#include "QuadMesh.h"
#include "AutoMesh.h"

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

using namespace std;

// Cube Mesh Array variables and initialization
list<CubeMesh*> cubeList;
list<AutoMesh*> autoList;

// also add a variable to keep track of current cube mesh
auto currentCube = cubeList.begin();
auto currentAuto = autoList.begin();

// Interaction State Variable
enum Action {TRANSLATE, ROTATE, SCALE, EXTRUDE, RAISE, SELECT, MULTIPLESELECT, DESELECT_ALL};
enum Action currentAction = TRANSLATE;

QuadMesh *floorMesh = NULL;
// Wall Mesh variables here
QuadMesh *rightMesh = NULL;
QuadMesh *leftMesh = NULL;
QuadMesh *backMesh = NULL;

typedef struct BoundingBox
{
    VECTOR3D min;
    VECTOR3D max;
} BBox;

BBox roomBBox;

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

    //right wall
    origin  = VECTOR3D(8.0f,0.0f,8.0f);
    dir1v   = VECTOR3D(0.0f, 1.0f, 0.0f);
    dir2v   = VECTOR3D(0.0f, 0.0f,-1.0f);
    rightMesh = new QuadMesh(meshSize, 16.0);
    rightMesh->InitMesh(meshSize, origin, 8.0, 16.0, dir1v, dir2v);

    rightMesh->SetMaterial(ambient,diffuse,specular,shininess);

    //left wall
    origin  = VECTOR3D(-8.0f,8.0f,8.0f);
    dir1v   = VECTOR3D(0.0f, -1.0f, 0.0f);
    dir2v   = VECTOR3D(0.0f, 0.0f,-1.0f);
    leftMesh = new QuadMesh(meshSize, 16.0);
    leftMesh->InitMesh(meshSize, origin, 8.0, 16.0, dir1v, dir2v);

    leftMesh->SetMaterial(ambient,diffuse,specular,shininess);

    //back wall
    origin  = VECTOR3D(8.0f,8.0f,-8.0f);
    dir1v   = VECTOR3D(-1.0f, 0.0f, 0.0f);
    dir2v   = VECTOR3D(0.0f, -1.0f, 0.0f);
    backMesh = new QuadMesh(meshSize, 16.0);
    backMesh->InitMesh(meshSize, origin, 16.0, 8.0, dir1v, dir2v);

    backMesh->SetMaterial(ambient,diffuse,specular,shininess);


    // Set up the bounding box of the room
    // Change this if you change your floor/wall dimensions
    roomBBox.min.Set(-8.0f, 0.0, -8.0);
    roomBBox.max.Set( 8.0f, 8.0,  8.0);
}



void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    // Set up the camera
    gluLookAt(0.0,6.0,22.0,0.0,0.0,0.0,0.0,1.0,0.0);

    // Draw all cubes (see CubeMesh.h)
    for(auto it : cubeList){
        drawCube(it);
    }
    for (auto it : autoList){
        it->drawTank();
    }

    // Draw floor and wall meshes
    floorMesh->DrawMesh(meshSize);
    rightMesh->DrawMesh(meshSize);
    leftMesh->DrawMesh(meshSize);
    backMesh->DrawMesh(meshSize);

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
            for ( auto it : cubeList){
                it->selected = false;
            }
            break;
    }
    glutPostRedisplay();
}

void functionKeys(int key, int x, int y){


    VECTOR3D min, max;

    if (key == GLUT_KEY_F1)
    {
        // Create and initialize new cube
        // becomes the currently selected cube
        for ( auto it : cubeList){
            it->selected = false;
        }
        cubeList.push_front(createCube());
        cubeList.front()->selected = true;
        currentCube = cubeList.begin();
    }

    if (key == GLUT_KEY_F2)
    {
        // Create and initialize new auto
        // becomes the currently selected auto
        for ( auto it : autoList){
            it->selected = false;
        }
        AutoMesh* newAuto = new AutoMesh;
        autoList.push_front(newAuto);
        autoList.front()->selected = true;
        currentAuto = autoList.begin();
    }

        // Do transformation code with arrow keys
        // GLUT_KEY_DOWN, GLUT_KEY_UP,GLUT_KEY_RIGHT, GLUT_KEY_LEFT
    else if (key == GLUT_KEY_DOWN)
    {
        switch (currentAction)
        {
            case TRANSLATE:
                for( auto it : cubeList){
                    if (it->selected == true){
                        it->tz += 1.0;
                        getBBox(it, &min, &max);
                        if (max.z > roomBBox.max.z) {
                            it->tz = roomBBox.max.z - 0.5 * (max.z - min.z);
                        }
                    }

                }
                break;

            case SCALE:
                for( auto it : cubeList){
                    if (it->selected == true){
                        it->sfz -= 1.0;
                        if (it->sfz < 1.0){
                            it->sfz = 1.0;
                        }
                    }
                }
                break;

            case ROTATE:
                break;

            case EXTRUDE:
                for( auto it : cubeList){
                    if (it->selected == true){
                        it->sfy -= 0.5;
                        it->ty -= 0.5;
                        if (it->sfy < 0.5) {
                            it->sfy = 0.5;
                            it->ty += 0.5;
                        }
                    }
                }
                break;

            case RAISE:
                for( auto it : cubeList){
                    if (it->selected == true){
                        it->ty -= 1.0;
                        getBBox(it, &min, &max);
                        if (min.y < roomBBox.min.y) {
                            it->ty = roomBBox.min.y + 0.5 * (max.y - min.y);
                        }
                    }
                }
                break;

            case SELECT:
                break;

            case MULTIPLESELECT:
                break;

            case DESELECT_ALL:
                break;
        }
    }

    else if (key == GLUT_KEY_UP)
    {
        switch (currentAction)
        {
            case TRANSLATE:
                for( auto it : cubeList){
                    if (it->selected == true){
                        it->tz -= 1.0;
                        getBBox(it, &min, &max);
                        if (min.z < roomBBox.min.z){
                            it->tz = roomBBox.min.z + 0.5 * (max.z - min.z);
                        }
                    }
                }
                break;

            case SCALE:
                for( auto it : cubeList){
                    if (it->selected == true){
                        it->sfz += 1.0;
                        getBBox(it, &min, &max);
                        //std::cout << "min: " << min.x << ", " << min.y << ", " << min.z << endl;
                        //std::cout << "max: " << max.x << ", " << max.y << ", " << max.z << endl;
                        if ((max.z > roomBBox.max.z) || (min.z < roomBBox.min.z) ||
                            (max.x > roomBBox.max.x) || (min.x < roomBBox.min.x)) {
                            it->sfz -= 1.0;
                        }
                    }
                }
                break;

            case ROTATE:
                break;

            case EXTRUDE:
                for( auto it : cubeList){
                    if (it->selected == true){
                        it->sfy += 0.5;
                        it->ty += 0.5;
                        getBBox(it, &min, &max);
                        if (max.y > roomBBox.max.y){
                            it->sfy -= 0.5;
                            it->ty -= 0.5;
                        }
                    }
                }
                break;

            case RAISE:
                for( auto it : cubeList){
                    if (it->selected == true){
                        it->ty += 1.0;
                        getBBox(it, &min, &max);
                        if (max.y > roomBBox.max.y){
                            it->ty = roomBBox.max.y - 0.5 * (max.y - min.y);
                        }
                    }
                }
                break;

            case SELECT:
                break;

            case MULTIPLESELECT:
                break;

            case DESELECT_ALL:
                break;
        }


    }

    else if (key == GLUT_KEY_RIGHT)
    {
        switch (currentAction)
        {
            case TRANSLATE:
                for( auto it : cubeList){
                    if (it->selected == true) {
                        it->tx += 1.0;
                        getBBox(it, &min, &max);
                        if (max.x > roomBBox.max.x){
                            it->tx = roomBBox.max.x - 0.5 * (max.x - min.x);
                        }
                    }
                }
                break;

            case SCALE:
                for( auto it : cubeList){
                    if (it->selected == true){
                        it->sfx += 1.0;
                        getBBox(it, &min, &max);
                        if ((max.z > roomBBox.max.z) || (min.z < roomBBox.min.z) ||
                            (max.x > roomBBox.max.x) || (min.x < roomBBox.min.x)) {
                            it->sfx -= 1.0;
                        }
                    }
                }
                break;

            case ROTATE:
                for ( auto it : cubeList){
                    if (it->selected == true){
                        it->angle += 5.0;
                        if (it->angle == 360.0){
                            it->angle = 0.0;
                        }
                    }
                }
                break;

            case EXTRUDE:
                break;

            case RAISE:
                break;

            case SELECT:
                for ( auto it : cubeList){
                    it->selected = false;
                }
                if (next(currentCube,1) == cubeList.end()){
                    printf("end of line");
                    currentCube = cubeList.begin();
                    (*currentCube)->selected = true;
                }
                else {
                    ++currentCube;
                    (*currentCube)->selected = true;
                }
                break;

            case MULTIPLESELECT:
                if (next(currentCube,1) == cubeList.end()){
                    printf("end of line");
                    currentCube = cubeList.begin();
                    (*currentCube)->selected = true;
                }
                else {
                    ++currentCube;
                    (*currentCube)->selected = true;
                }
                break;

            case DESELECT_ALL:
                break;
        }
    }

    else if (key == GLUT_KEY_LEFT)
    {
        switch (currentAction)
        {
            case TRANSLATE:
                for( auto it : cubeList){
                    if (it->selected == true){
                        it->tx -= 1.0;
                        getBBox(it, &min, &max);
                        if (min.x < roomBBox.min.x){
                            it->tx = roomBBox.min.x + 0.5 * (max.x - min.x);
                        }
                    }
                }
                break;

            case SCALE:
                for( auto it : cubeList){
                    if (it->selected == true){
                        it->sfx -= 1.0;
                        if (it->sfx < 1.0){
                            it->sfx = 1.0;
                        }
                    }
                }
                break;

            case ROTATE:
                for ( auto it : cubeList){
                    if (it->selected == true){
                        it->angle -= 5.0;
                        if (it->angle == -360.0){
                            it->angle = 0.0;
                        }
                    }
                }
                break;

            case EXTRUDE:
                break;

            case RAISE:
                break;

            case SELECT:
                for ( auto it : cubeList){
                    it->selected = false;
                }
                if (currentCube == cubeList.begin()){
                    printf("end of line");
                    currentCube = cubeList.end();
                    --currentCube;
                    (*currentCube)->selected = true;
                }
                else {
                    --currentCube;
                    (*currentCube)->selected = true;
                }
                break;

            case MULTIPLESELECT:
                if (currentCube == cubeList.begin()){
                    printf("end of line");
                    currentCube = cubeList.end();
                    --currentCube;
                    (*currentCube)->selected = true;
                }
                else {
                    --currentCube;
                    (*currentCube)->selected = true;
                }
                break;

            case DESELECT_ALL:
                break;
        }
    }
    glutPostRedisplay();
}

bool checkBounds(VECTOR3D *minCube, VECTOR3D *maxCube){

    VECTOR3D minCheck;
    VECTOR3D maxCheck;

    minCheck.x = minCube->x - roomBBox.min.x;
    minCheck.y = minCube->y - roomBBox.min.y;
    minCheck.z = minCube->z - roomBBox.min.z;

    maxCheck.x = roomBBox.max.x - maxCube->x;
    maxCheck.y = roomBBox.max.y - maxCube->y;
    maxCheck.z = roomBBox.max.z - maxCube->z;

    if((minCheck.x*minCheck.y*minCheck.z) < 0.0) return false;

    if((maxCheck.x*maxCheck.y*maxCheck.z) < 0.0) return false;

    return true;

}
