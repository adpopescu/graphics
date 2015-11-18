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
#include "QuadMesh.h"
#include "AutoMesh.h"
#include "Mesh.h"
#include "TableMesh.h"
#include "IceCreamConeMesh.h"

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
list<Mesh*> meshList;

// also add a variable to keep track of current cube mesh
auto currentMesh = meshList.begin();

// Interaction State Variable
enum Action {TRANSLATE, ROTATE, SCALE, EXTRUDE, RAISE, SELECT, MULTIPLESELECT, DESELECT_ALL, EXPLORE};
enum Action currentAction = TRANSLATE;

bool exploreMode = false;
Mesh* exploreMesh = NULL;
GLdouble exploreSpeed = 2.0;

QuadMesh *floorMesh = NULL;
// Wall Mesh variables here
QuadMesh *rightMesh = NULL;
QuadMesh *leftMesh = NULL;
QuadMesh *backMesh = NULL;


//camera variables
GLdouble radiusCam, azimuthCam, inclinationCam;
GLdouble camPosX, camPosY, camPosZ;
GLdouble lookAtX, lookAtY, lookAtZ;

int mousePrevX = -1, mousePrevY = -1;

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
    roomBBox.min.Set(-8.0, 0.0, -8.0);
    roomBBox.max.Set( 8.0, 8.0,  8.0);

    //Starting Camera Position
    radiusCam = 20.0;
    inclinationCam = -PI/2.0;
    azimuthCam = PI;


}



void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    // Set up the camera
    if (exploreMode){
        camPosX = exploreMesh->tx;
        camPosY = exploreMesh->ty + exploreMesh->modelMaxCoords[1] + 2.0;
        camPosZ = exploreMesh->tz;
        lookAtX = exploreMesh->tx + cos(exploreMesh->angle*2*PI/360);
        lookAtY = camPosY;
        lookAtZ = exploreMesh->tz + -sin(exploreMesh->angle*2*PI/360);
    }
    else {
        camPosX = radiusCam * sin(inclinationCam) * sin(azimuthCam);
        camPosY = radiusCam * cos(inclinationCam);
        camPosZ = radiusCam * sin(inclinationCam) * cos(azimuthCam);
        lookAtX = 0.0;
        lookAtY = 0.0;
        lookAtZ = 0.0;
    }

    //cout << inclinationCam << endl << azimuthCam << endl << endl;

    gluLookAt(camPosX, camPosY, camPosZ, lookAtX, lookAtY, lookAtZ, 0.0, 1.0, 0.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0,1.0,0.01*radiusCam,4.0*radiusCam);
    glMatrixMode(GL_MODELVIEW);

    // Draw all objects
    for (auto it : meshList){
        it->drawMesh();
    }
    //draw explore mesh
    if (exploreMesh){
        exploreMesh->drawMesh();
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
    if (mousePrevX == -1 && mousePrevY == -1){
        mousePrevX = xMouse;
        mousePrevY = yMouse;
    }
    if (currentButton == GLUT_LEFT_BUTTON)
    {
//        cout << "prevX: " << mousePrevX << endl << "X: " << xMouse << endl;
//        cout << "prevY: " << mousePrevY << endl << "Y: " << yMouse << endl << endl;
        if (xMouse > mousePrevX){
            azimuthCam -= 0.01;
            if (azimuthCam >= 2*PI) {
                azimuthCam = 0.0;
            }
        }
        else if (xMouse < mousePrevX){
            azimuthCam += 0.01;
            if (azimuthCam <= 0.0) {
                azimuthCam = 2*PI;
            }
        }
        if (yMouse > mousePrevY){
            inclinationCam += 0.01;
            if (inclinationCam >= -0.01) {
                inclinationCam = -0.01;
            }
        }
        else if (yMouse < mousePrevY){
            inclinationCam -= 0.01;
            if (inclinationCam <= -PI/2) {
                inclinationCam = -PI/2;
            }
        }

    }

    if (currentButton == GLUT_RIGHT_BUTTON){
        if (yMouse > mousePrevY){
            radiusCam += 0.5;
            if (radiusCam >= 100) {
                radiusCam = 100;
            }
        }
        else if (yMouse < mousePrevY){
            radiusCam -= 0.5;
            if (radiusCam <= 5) {
                radiusCam = 5;
            }
        }

    }
    // TODO: Implement middle button movement of what the camera is looking at. Will depend on current orientation.
//    if (currentButton == GLUT_MIDDLE_BUTTON)
//    {
//        if (xMouse > mousePrevX){
//             -= 0.01;
//            if (azimuthCam >= 2*PI) {
//                azimuthCam = 0.0;
//            }
//        }
//        else if (xMouse < mousePrevX){
//            azimuthCam += 0.01;
//            if (azimuthCam <= 0.0) {
//                azimuthCam = 2*PI;
//            }
//        }
//        if (yMouse > mousePrevY){
//            inclinationCam += 0.01;
//            if (inclinationCam >= -0.01) {
//                inclinationCam = -0.01;
//            }
//        }
//        else if (yMouse < mousePrevY){
//            inclinationCam -= 0.01;
//            if (inclinationCam <= -PI/2) {
//                inclinationCam = -PI/2;
//            }
//        }
//
//    }

    mousePrevX = xMouse;
    mousePrevY = yMouse;
    glutPostRedisplay();
}


VECTOR3D ScreenToWorld(int x, int y)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    // Read all pixels at given screen XY from the Depth Buffer
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );
    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);
    return VECTOR3D((float)posX, (float)posY, (float)posZ);
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
            for ( auto it : meshList){
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
        for ( auto it : meshList){
            it->selected = false;
        }
        Mesh* newMesh = new Mesh;
        meshList.push_front(newMesh);
        meshList.front()->selected = true;
        currentMesh = meshList.begin();
    }

    else if (key == GLUT_KEY_F2)
    {
        // Create and initialize new auto
        // becomes the currently selected auto
        for ( auto it : meshList){
            it->selected = false;
        }
        AutoMesh* newAuto = new AutoMesh;
        meshList.push_front(newAuto);
        meshList.front()->selected = true;
        currentMesh = meshList.begin();
    }

    else if (key == GLUT_KEY_F3)
    {
        // Create and initialize new auto
        // becomes the currently selected auto
        for ( auto it : meshList){
            it->selected = false;
        }
        TableMesh* newTable = new TableMesh;
        meshList.push_front(newTable);
        meshList.front()->selected = true;
        currentMesh = meshList.begin();
    }

    else if (key == GLUT_KEY_F4)
    {
        // Create and initialize new auto
        // becomes the currently selected auto
        for ( auto it : meshList){
            it->selected = false;
        }
        IceCreamConeMesh* newIceCreamCone = new IceCreamConeMesh;
        meshList.push_front(newIceCreamCone);
        meshList.front()->selected = true;
        currentMesh = meshList.begin();
    }

        // enter exploration mode
    else if (key == GLUT_KEY_F5)
    {
        if (!exploreMode){
            currentAction = EXPLORE;
            exploreMode = true;
            exploreMesh = new AutoMesh;
        }
    }

        //exit exploration mode
    else if (key == GLUT_KEY_F6)
    {
        if (exploreMode){
            for ( auto it : meshList){
                it->selected = false;
            }
            currentAction = TRANSLATE;
            exploreMode = false;
            meshList.push_front(exploreMesh);
            meshList.front()->selected = true;
            currentMesh = meshList.begin();
            exploreMesh = NULL;
        }
    }

        // Do transformation code with arrow keys
        // GLUT_KEY_DOWN, GLUT_KEY_UP,GLUT_KEY_RIGHT, GLUT_KEY_LEFT
    else if (key == GLUT_KEY_DOWN)
    {
        switch (currentAction)
        {
            case TRANSLATE:
                for( auto it : meshList){
                    if (it->selected == true){
                        it->tz += 1.0;
                        it->getBBox(&min, &max);
                        if (max.z >= roomBBox.max.z) {
                            it->tz = roomBBox.max.z - 0.5 * (max.z - min.z);
                        }
                    }
                }
                break;

            case SCALE:
                for( auto it : meshList){
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
                for( auto it : meshList){
                    if (it->selected == true){
                        it->sfy -= 0.5;
                        if (it->sfy < 0.5) {
                            it->sfy = 0.5;
                        }
                    }
                }
                break;

            case RAISE:
                for( auto it : meshList){
                    if (it->selected == true){
                        it->ty -= 1.0;
                        it->getBBox(&min, &max);
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

            case EXPLORE:
                exploreMesh->tx -= exploreSpeed * cos(exploreMesh->angle*2*PI/360);
                exploreMesh->tz += exploreSpeed * sin(exploreMesh->angle*2*PI/360);
                exploreMesh->getBBox(&min, &max);
                if (max.z > roomBBox.max.z){
                    exploreMesh->tz = roomBBox.max.z - 0.5 * (max.z - min.z);
                }
                else if (min.z < roomBBox.min.z) {
                    exploreMesh->tz = roomBBox.min.z + 0.5 * (max.z - min.z);
                }
                if (max.x > roomBBox.max.x) {
                    exploreMesh->tx = roomBBox.max.x - 0.5 * (max.x - min.x);
                }
                else if (min.x < roomBBox.min.x) {
                    exploreMesh->tx = roomBBox.min.x + 0.5 * (max.x - min.x);
                }
        }
    }

    else if (key == GLUT_KEY_UP)
    {
        switch (currentAction)
        {
            case TRANSLATE:
                for( auto it : meshList){
                    if (it->selected == true){
                        it->tz -= 1.0;
                        it->getBBox(&min, &max);
                        if (min.z <= roomBBox.min.z){
//                            std::cout << "Tz(before collision): " << it->tz << std::endl;
                            it->tz = roomBBox.min.z + 0.5 * (max.z - min.z);
//                            std::cout << "Tz(after collision): " << it->tz << std::endl;
//                            std::cout << "DifferenceZ(max-min): " << (max.z - min.z) << endl;
//                            std::cout << "Room MinZ: " << roomBBox.min.z << endl;
                        }
                    }
                }
                break;

            case SCALE:
                for( auto it : meshList){
                    if (it->selected == true){
                        it->sfz += 1.0;
                        it->getBBox(&min, &max);
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
                for( auto it : meshList){
                    if (it->selected == true){
                        it->sfy += 0.5;
                        it->getBBox(&min, &max);
                        if ((max.y > roomBBox.max.y) || (min.y < roomBBox.min.y)){
                            it->sfy -= 0.5;
                        }
                    }
                }
                break;

            case RAISE:
                for( auto it : meshList){
                    if (it->selected == true){
                        it->ty += 1.0;
                        it->getBBox(&min, &max);
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

            case EXPLORE:
                exploreMesh->tx += exploreSpeed * cos(exploreMesh->angle*2*PI/360);
                exploreMesh->tz -= exploreSpeed * sin(exploreMesh->angle*2*PI/360);
                exploreMesh->getBBox(&min, &max);
                if (max.z > roomBBox.max.z){
                    exploreMesh->tz = roomBBox.max.z - 0.5 * (max.z - min.z);
                }
                else if (min.z < roomBBox.min.z) {
                    exploreMesh->tz = roomBBox.min.z + 0.5 * (max.z - min.z);
                }
                if (max.x > roomBBox.max.x) {
                    exploreMesh->tx = roomBBox.max.x - 0.5 * (max.x - min.x);
                }
                else if (min.x < roomBBox.min.x) {
                    exploreMesh->tx = roomBBox.min.x + 0.5 * (max.x - min.x);
                }
        }
    }

    else if (key == GLUT_KEY_RIGHT)
    {
        switch (currentAction) {
            case TRANSLATE:
                for (auto it : meshList) {
                    if (it->selected == true) {
                        it->tx += 1.0;
                        it->getBBox(&min, &max);
                        if (max.x > roomBBox.max.x) {
                            it->tx = roomBBox.max.x - 0.5 * (max.x - min.x);
                        }
                    }
                }
                break;

            case SCALE:
                for (auto it : meshList) {
                    if (it->selected == true) {
                        it->sfx += 1.0;
                        it->getBBox(&min, &max);
                        if ((max.z > roomBBox.max.z) || (min.z < roomBBox.min.z) ||
                            (max.x > roomBBox.max.x) || (min.x < roomBBox.min.x)) {
                            it->sfx -= 1.0;
                        }
                    }
                }
                break;

            case ROTATE:
                for (auto it : meshList) {
                    if (it->selected == true) {
                        it->angle += 5.0;
                        it->getBBox(&min, &max);
                        if ((max.z > roomBBox.max.z) || (min.z < roomBBox.min.z) ||
                            (max.x > roomBBox.max.x) || (min.x < roomBBox.min.x)) {
                            it->angle -= 5.0;
                        }
                        if (it->angle == 360.0) {
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
                for (auto it : meshList) {
                    it->selected = false;
                }
                if (next(currentMesh, 1) == meshList.end()) {
                    currentMesh = meshList.begin();
                    (*currentMesh)->selected = true;
                }
                else {
                    ++currentMesh;
                    (*currentMesh)->selected = true;
                }
                break;

            case MULTIPLESELECT:
                if (next(currentMesh, 1) == meshList.end()) {
                    currentMesh = meshList.begin();
                    (*currentMesh)->selected = true;
                }
                else {
                    ++currentMesh;
                    (*currentMesh)->selected = true;
                }
                break;

            case DESELECT_ALL:
                break;

            case EXPLORE:
                exploreMesh->angle -= 5.0;
                exploreMesh->getBBox(&min, &max);
                if ((max.z > roomBBox.max.z) || (min.z < roomBBox.min.z) ||
                    (max.x > roomBBox.max.x) || (min.x < roomBBox.min.x)) {
                    exploreMesh->angle += 5.0;
                }
                if (exploreMesh->angle == -360.0) {
                    exploreMesh->angle = 0.0;
                }
        }
    }

    else if (key == GLUT_KEY_LEFT)
    {
        switch (currentAction)
        {
            case TRANSLATE:
                for( auto it : meshList){
                    if (it->selected == true){
                        it->tx -= 1.0;
                        it->getBBox(&min, &max);
                        if (min.x < roomBBox.min.x){
                            it->tx = roomBBox.min.x + 0.5 * (max.x - min.x);
                        }
                    }
                }
                break;

            case SCALE:
                for( auto it : meshList){
                    if (it->selected == true){
                        it->sfx -= 1.0;
                        if (it->sfx < 1.0){
                            it->sfx = 1.0;
                        }
                    }
                }
                break;

            case ROTATE:
                for ( auto it : meshList){
                    if (it->selected == true){
                        it->angle -= 5.0;
                        it->getBBox(&min, &max);
                        if ((max.z > roomBBox.max.z) || (min.z < roomBBox.min.z) ||
                            (max.x > roomBBox.max.x) || (min.x < roomBBox.min.x)) {
                            it->angle += 5.0;
                        }
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
                for ( auto it : meshList){
                    it->selected = false;
                }
                if (currentMesh == meshList.begin()){
                    currentMesh = meshList.end();
                    --currentMesh;
                    (*currentMesh)->selected = true;
                }
                else {
                    --currentMesh;
                    (*currentMesh)->selected = true;
                }
                break;

            case MULTIPLESELECT:
                if (currentMesh == meshList.begin()){
                    currentMesh = meshList.end();
                    --currentMesh;
                    (*currentMesh)->selected = true;
                }
                else {
                    --currentMesh;
                    (*currentMesh)->selected = true;
                }
                break;

            case DESELECT_ALL:
                break;

            case EXPLORE:
                exploreMesh->angle += 5.0;
                exploreMesh->getBBox(&min, &max);
                if ((max.z > roomBBox.max.z) || (min.z < roomBBox.min.z) ||
                    (max.x > roomBBox.max.x) || (min.x < roomBBox.min.x)) {
                    exploreMesh->angle -= 5.0;
                }
                if (exploreMesh->angle == 360.0) {
                    exploreMesh->angle = 0.0;
                }
        }
    }
    glutPostRedisplay();
}