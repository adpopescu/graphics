/*******************************************************************
	       Scene Modelling Program
********************************************************************/

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <utility>
#include <vector>
#include <forward_list>
#include <iterator>
#include <iostream>
#include "RobotMesh.h"
#include "Level.h"
#include "ProjectileMesh.h"
#include <SOIL/SOIL.h>

#define DEBUG_MODE 0

void initOpenGL(int w, int h);
void display(void);
void testDisplay(void);
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



// Interaction State Variable
enum Action {TRANSLATE, ROTATE, SCALE, EXTRUDE, RAISE, SELECT, MULTIPLESELECT, DESELECT_ALL, EXPLORE};
enum Action currentAction = TRANSLATE;

RobotMesh* playerMesh = NULL;
GLdouble playerMoveSpeed = 0.5;
int playerHealth = 40;

Level* level = NULL;

static const int numTextures = 6;
GLuint texture[numTextures];

//camera variables
GLdouble radiusCam, azimuthCam, inclinationCam;
GLdouble camPosX, camPosY, camPosZ;
GLdouble lookAtX, lookAtY, lookAtZ;

int mousePrevX = -1, mousePrevY = -1;


// Default Mesh Size

int main(int argc, char **argv)
{

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 720);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Robots Game");

    initOpenGL(1280,720);

    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotionHandler);
//    glutPassiveMotionFunc(mouseMotionHandler);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(functionKeys);
    glutMainLoop();
    return 0;
}

void makeTextures() {

    int width = 0;
    int height = 0;
    int channels = 0;

    glGenTextures(numTextures, texture);
    cout << glGetError() << endl;

    unsigned char* image = SOIL_load_image("textures/palace_floor.jpg", &width, &height, &channels, SOIL_LOAD_RGB);

    if (DEBUG_MODE){
        cout << "Width: " << width << endl << "Height: " << height << endl << "Channels: " << channels << endl;
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    cout << glGetError() << endl;
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    cout << glGetError() << endl;
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    cout << glGetError() << endl;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    cout << glGetError() << endl;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    cout << glGetError() << endl;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    cout << glGetError() << endl;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    cout << glGetError() << endl;
//    texture = SOIL_load_OGL_texture("textures/floor.jpg", 3, texture, SOIL_FLAG_POWER_OF_TWO|SOIL_FLAG_TEXTURE_REPEATS);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    if (DEBUG_MODE) {
        cout << "Load image into 2D texture: " << glGetError() << endl;
        cout << "texture: " << texture << endl;
    }


    image = SOIL_load_image("textures/metallic_wall.jpg", &width, &height, &channels, SOIL_LOAD_RGB);
    if (DEBUG_MODE) {
        cout << "Width: " << width << endl << "Height: " << height << endl << "Channels: " << channels << endl;
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    cout << glGetError() << endl;
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    cout << glGetError() << endl;
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    cout << glGetError() << endl;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    cout << glGetError() << endl;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    cout << glGetError() << endl;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    cout << glGetError() << endl;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    cout << glGetError() << endl;
//    texture = SOIL_load_OGL_texture("textures/floor.jpg", 3, texture, SOIL_FLAG_POWER_OF_TWO|SOIL_FLAG_TEXTURE_REPEATS);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    if (DEBUG_MODE) {
        cout << "Load image into 2D texture: " << glGetError() << endl;
        cout << "texture: " << texture << endl;
    }

    image = SOIL_load_image("textures/spaceship_ceiling.jpg", &width, &height, &channels, SOIL_LOAD_RGB);
    if (DEBUG_MODE) {
        cout << "Width: " << width << endl << "Height: " << height << endl << "Channels: " << channels << endl;
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    cout << glGetError() << endl;
    glBindTexture(GL_TEXTURE_2D, texture[2]);
    cout << glGetError() << endl;
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    cout << glGetError() << endl;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    cout << glGetError() << endl;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    cout << glGetError() << endl;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    cout << glGetError() << endl;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    cout << glGetError() << endl;
//    texture = SOIL_load_OGL_texture("textures/floor.jpg", 3, texture, SOIL_FLAG_POWER_OF_TWO|SOIL_FLAG_TEXTURE_REPEATS);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    if (DEBUG_MODE) {
        cout << "Load image into 2D texture: " << glGetError() << endl;
        cout << "texture: " << texture << endl;
    }

    image = SOIL_load_image("textures/tire.jpg", &width, &height, &channels, SOIL_LOAD_RGB);
    if (DEBUG_MODE) {
        cout << "Width: " << width << endl << "Height: " << height << endl << "Channels: " << channels << endl;
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    cout << glGetError() << endl;
    glBindTexture(GL_TEXTURE_2D, texture[3]);
    cout << glGetError() << endl;
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    cout << glGetError() << endl;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    cout << glGetError() << endl;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    cout << glGetError() << endl;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    cout << glGetError() << endl;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    cout << glGetError() << endl;
//    texture = SOIL_load_OGL_texture("textures/floor.jpg", 3, texture, SOIL_FLAG_POWER_OF_TWO|SOIL_FLAG_TEXTURE_REPEATS);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    if (DEBUG_MODE) {
        cout << "Load image into 2D texture: " << glGetError() << endl;
        cout << "texture: " << texture << endl;
    }

    image = SOIL_load_image("textures/battered_robot.jpg", &width, &height, &channels, SOIL_LOAD_RGB);
    if (DEBUG_MODE) {
        cout << "Width: " << width << endl << "Height: " << height << endl << "Channels: " << channels << endl;
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    cout << glGetError() << endl;
    glBindTexture(GL_TEXTURE_2D, texture[4]);
    cout << glGetError() << endl;
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    cout << glGetError() << endl;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    cout << glGetError() << endl;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    cout << glGetError() << endl;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    cout << glGetError() << endl;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    cout << glGetError() << endl;
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    if (DEBUG_MODE) {
        cout << "Load image into 2D texture: " << glGetError() << endl;
        cout << "texture: " << texture << endl;
    }

    image = SOIL_load_image("textures/blue_rusted_metal.jpg", &width, &height, &channels, SOIL_LOAD_RGB);
    if (DEBUG_MODE) {
        cout << "Width: " << width << endl << "Height: " << height << endl << "Channels: " << channels << endl;
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    cout << glGetError() << endl;
    glBindTexture(GL_TEXTURE_2D, texture[5]);
    cout << glGetError() << endl;
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
    cout << glGetError() << endl;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    cout << glGetError() << endl;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    cout << glGetError() << endl;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    cout << glGetError() << endl;
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    cout << glGetError() << endl;
//    texture = SOIL_load_OGL_texture("textures/floor.jpg", 3, texture, SOIL_FLAG_POWER_OF_TWO|SOIL_FLAG_TEXTURE_REPEATS);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    if (DEBUG_MODE) {
        cout << "Load image into 2D texture: " << glGetError() << endl;
        cout << "texture: " << texture << endl;
    }

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
    glClearColor(0.6, 0.6, 0.6, 0.0);
    glClearDepth(1.0f);
    glLoadIdentity();

    glHint(GL_PERSPECTIVE_CORRECTION_HINT , GL_NICEST);

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
    glEnable(GL_TEXTURE_2D);
    if (DEBUG_MODE) {
        cout << "Enable 2D Textures: " << glGetError() << endl;
    }


    // This one is important - renormalize normal vectors
    glEnable(GL_NORMALIZE);
    makeTextures();

    // Set up meshes

    // Set up wall meshes
    // Make sure direction vectors are such that the normals are pointing into the room
    // Use the right-hand-rule (cross product)
    // If you are confused about this, ask in class
    level = new Level;
    VECTOR3D origin = VECTOR3D(-5.0,0.0,10.0);
    playerMesh = new RobotMesh(origin, 0.0);
    srand(time(NULL));



    // Set up the bounding box of the room
    // Change this if you change your floor/wall dimensions

    //Starting Camera Position
    radiusCam = 10.0;
    inclinationCam = -3.0;
//    azimuthCam = PI;
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    // Set up the camera
    camPosX = playerMesh->tx;
    camPosY = playerMesh->ty + playerMesh->modelMaxCoords[1] + 0.5;
    camPosZ = playerMesh->tz;
    lookAtX = playerMesh->tx + radiusCam * cos(playerMesh->angle * 2 * PI / 360);
    lookAtY = camPosY + inclinationCam;
    lookAtZ = playerMesh->tz + radiusCam * -sin(playerMesh->angle * 2 * PI / 360);

    //cout << inclinationCam << endl << azimuthCam << endl << endl;
    gluLookAt(camPosX, camPosY, camPosZ, lookAtX, lookAtY, lookAtZ, 0.0, 1.0, 0.0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0,1.0,0.01*radiusCam,40.0*radiusCam);
    glMatrixMode(GL_MODELVIEW);

    // Make robots do things

    level->animateRooms(playerMesh->getPos());

    // Draw all objects
    VECTOR3D roomMin, roomMax;
    forward_list<ProjectileMesh*> projectileDeleteList;
    forward_list<RobotMesh*> robotDeleteList;

    if (DEBUG_MODE) {
        cout << endl << "Drawing Meshes" << endl;
    }
    for (auto it : level->projectileList){

        Room *currentRoom;
        cout << "X: " << it->getPos().GetX() << endl;
        currentRoom = level->getRoom(it->getPos());
        VECTOR3D robotMin, robotMax;

        for (auto rob : currentRoom->robots){
            rob->getBBox(&robotMin, &robotMax);
            if ((it->getPos().GetZ() < robotMax.GetZ()) && (it->getPos().GetZ() > robotMin.GetZ()) &&
                (it->getPos().GetX() < robotMax.GetX()) && (it->getPos().GetX() > robotMin.GetX())) {
                projectileDeleteList.push_front(it);
                robotDeleteList.push_front(rob);
            }
        }
        for (auto rob : robotDeleteList){
            if (DEBUG_MODE) {
                cout << endl << "Deleting Robot" << endl;
            }
            currentRoom->robots.remove(rob);
            currentRoom->robotCount--;
        }

        playerMesh->getBBox(&robotMin, &robotMax);
        if ((it->getPos().GetZ() < robotMax.GetZ()) && (it->getPos().GetZ() > robotMin.GetZ()) &&
            (it->getPos().GetX() < robotMax.GetX()) && (it->getPos().GetX() > robotMin.GetX())) {
            projectileDeleteList.push_front(it);
            playerHealth--;
            cout << "Player health: " << playerHealth << endl;
            if (playerHealth < 0){
                cout << "You lose." << endl;
                exit(0);
            }
        }

        if (level->getRoomBBox(&roomMin, &roomMax, it->getPos())){
            if (DEBUG_MODE) {
                cout << endl << "Projectile In Room" << endl;
            }
            it->drawMesh();
            it->tx += it->getSpeed() * cos(it->angle * 2 * PI / 360);
            it->tz -= it->getSpeed() * sin(it->angle * 2 * PI / 360);
            if ((it->getPos().GetZ() > roomMax.GetZ()) || (it->getPos().GetZ() < roomMin.GetZ()) ||
                (it->getPos().GetX() > roomMax.GetX()) || (it->getPos().GetX() < roomMin.GetX())) {
                projectileDeleteList.push_front(it);
            }
        }
        else{
            projectileDeleteList.push_front(it);
        }


    }
    for (auto it : projectileDeleteList){
        if (DEBUG_MODE) {
            cout << endl << "Deleting Projectile" << endl;
        }
        level->projectileList.remove(it);
        level->curCount--;
    }
    //draw player mesh
    if (playerMesh){
        playerMesh->drawMesh(texture);
    }

    // Draw floor and wall meshes
    level->drawLevel(texture);
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

    VECTOR3D playerMin, playerMax;
    VECTOR3D roomMin, roomMax;

//  cout << "prevX: " << mousePrevX << endl << "X: " << xMouse << endl;
//  cout << "prevY: " << mousePrevY << endl << "Y: " << yMouse << endl << endl;
    if (currentButton == GLUT_RIGHT_BUTTON) {

        if (xMouse > mousePrevX) {
            playerMesh->angle -= 0.5;
            playerMesh->getBBox(&playerMin, &playerMax);
            level->getRoomBBox(&roomMin, &roomMax, playerMesh->getPos());
            if ((playerMax.z > roomMax.z) || (playerMin.z < roomMin.z) ||
                (playerMax.x > roomMax.x) || (playerMin.x < roomMin.x)) {
                playerMesh->angle += 0.5;
            }
            if (playerMesh->angle == -360.0) {
                playerMesh->angle = 0.0;
            }
        }

        else if (xMouse < mousePrevX) {
            playerMesh->angle += 0.5;
            playerMesh->getBBox(&playerMin, &playerMax);
            level->getRoomBBox(&roomMin, &roomMax, playerMesh->getPos());
            if ((playerMax.z > roomMax.z) || (playerMin.z < roomMin.z) ||
                (playerMax.x > roomMax.x) || (playerMin.x < roomMin.x)) {
                playerMesh->angle -= 0.5;
            }
            if (playerMesh->angle == 360.0) {
                playerMesh->angle = 0.0;
            }
        }

        if (yMouse < mousePrevY){
            inclinationCam += 0.1;
            if (inclinationCam >= 20) {
                inclinationCam = 20;
            }
        }

        else if (yMouse > mousePrevY){
            inclinationCam -= 0.1;
            if (inclinationCam <= -20) {
                inclinationCam = -20;
            }
        }
    }

    mousePrevX = xMouse;
    mousePrevY = yMouse;
    glutPostRedisplay();
}

/* Handles input from the keyboard, non-arrow keys */
void keyboard(unsigned char key, int x, int y)
{
    VECTOR3D playerMin, playerMax;
    VECTOR3D roomMin, roomMax;
    switch (key)
    {
        case 'w':
            playerMesh->tx += playerMoveSpeed * cos(playerMesh->angle * 2 * PI / 360);
            playerMesh->tz -= playerMoveSpeed * sin(playerMesh->angle * 2 * PI / 360);
            playerMesh->getBBox(&playerMin, &playerMax);
            level->getRoomBBox(&roomMin, &roomMax, playerMesh->getPos());
            if (playerMax.z > roomMax.z){
                playerMesh->tz = roomMax.z - 0.5 * (playerMax.z - playerMin.z);
            }
            else if (playerMin.z < roomMin.z) {
                playerMesh->tz = roomMin.z + 0.5 * (playerMax.z - playerMin.z);
            }
            if (playerMax.x > roomMax.x) {
                playerMesh->tx = roomMax.x - 0.5 * (playerMax.x - playerMin.x);
            }
            else if (playerMin.x < roomMin.x) {
                playerMesh->tx = roomMin.x + 0.5 * (playerMax.x - playerMin.x);
            }
            break;
        case 's':
            playerMesh->tx -= playerMoveSpeed * cos(playerMesh->angle * 2 * PI / 360);
            playerMesh->tz += playerMoveSpeed * sin(playerMesh->angle * 2 * PI / 360);
            playerMesh->getBBox(&playerMin, &playerMax);
            level->getRoomBBox(&roomMin, &roomMax, playerMesh->getPos());
            if (playerMax.z > roomMax.z){
                playerMesh->tz = roomMax.z - 0.5 * (playerMax.z - playerMin.z);
            }
            else if (playerMin.z < roomMin.z) {
                playerMesh->tz = roomMin.z + 0.5 * (playerMax.z - playerMin.z);
            }
            if (playerMax.x > roomMax.x) {
                playerMesh->tx = roomMax.x - 0.5 * (playerMax.x - playerMin.x);
            }
            else if (playerMin.x < roomMin.x) {
                playerMesh->tx = roomMin.x + 0.5 * (playerMax.x - playerMin.x);
            }
            break;
        case 'a':
            playerMesh->tx += playerMoveSpeed * -sin(playerMesh->angle * 2 * PI / 360);
            playerMesh->tz -= playerMoveSpeed * cos(playerMesh->angle * 2 * PI / 360);
            playerMesh->getBBox(&playerMin, &playerMax);
            level->getRoomBBox(&roomMin, &roomMax, playerMesh->getPos());
            if (playerMax.z > roomMax.z){
                playerMesh->tz = roomMax.z - 0.5 * (playerMax.z - playerMin.z);
            }
            else if (playerMin.z < roomMin.z) {
                playerMesh->tz = roomMin.z + 0.5 * (playerMax.z - playerMin.z);
            }
            if (playerMax.x > roomMax.x) {
                playerMesh->tx = roomMax.x - 0.5 * (playerMax.x - playerMin.x);
            }
            else if (playerMin.x < roomMin.x) {
                playerMesh->tx = roomMin.x + 0.5 * (playerMax.x - playerMin.x);
            }
            break;
        case 'd':
            playerMesh->tx -= playerMoveSpeed * -sin(playerMesh->angle * 2 * PI / 360);
            playerMesh->tz += playerMoveSpeed * cos(playerMesh->angle * 2 * PI / 360);
            playerMesh->getBBox(&playerMin, &playerMax);
            level->getRoomBBox(&roomMin, &roomMax, playerMesh->getPos());
            if (playerMax.z > roomMax.z){
                playerMesh->tz = roomMax.z - 0.5 * (playerMax.z - playerMin.z);
            }
            else if (playerMin.z < roomMin.z) {
                playerMesh->tz = roomMin.z + 0.5 * (playerMax.z - playerMin.z);
            }
            if (playerMax.x > roomMax.x) {
                playerMesh->tx = roomMax.x - 0.5 * (playerMax.x - playerMin.x);
            }
            else if (playerMin.x < roomMin.x) {
                playerMesh->tx = roomMin.x + 0.5 * (playerMax.x - playerMin.x);
            }
            break;

            // For testing purposes only, to see rooms from outside.
//        case 'z':
//            playerMesh->ty -= playerMoveSpeed;
//            break;
//        case 'x':
//            playerMesh->ty += playerMoveSpeed;
//            break;
        case 'q':
            exit(0);
        case ' ':
            ProjectileMesh *tempProjectile = new ProjectileMesh(playerMesh);
            level->projectileList.push_front(tempProjectile);
            break;

    }
    glutPostRedisplay();
}

void functionKeys(int key, int x, int y){

    glutPostRedisplay();
}