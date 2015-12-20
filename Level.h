//
// Created by dapopesc on 17/12/15.
//

#ifndef GRAPHICS_LEVEL_H
#define GRAPHICS_LEVEL_H

#include "Room.h"
#include "ProjectileMesh.h"
#include <cmath>
#include <forward_list>

using namespace std;

class Level {


public:
    Level();
    void initLevel();
    void drawLevel(GLuint *textures);
    bool getRoomBBox(VECTOR3D *min, VECTOR3D *max, VECTOR3D objectPos);
    Room* getRoom(VECTOR3D objectPos);
    void animateRooms(VECTOR3D playerPos);
    bool allRobotsDead();
    forward_list<ProjectileMesh*> projectileList;
    int curCount = 0;
    int maxLevelRobots = 10;
    int robotsSoFar = 0;

private:
    bool insideRoom(Room *room, VECTOR3D *objectPos);

    forward_list<Room*> rooms;
    VECTOR3D room1_Origin;
    VECTOR3D room2_Origin;
    VECTOR3D room3_Origin;
    float room1_size;
    float room2_size;
    float room3_size;
    int room1_doors[4] = {1,1,0,0};
    int room2_doors[4] = {1,0,0,0};
    int room3_doors[4] = {0,1,0,0};

    int proCount = 10;
    float enemySpeed = 0.01;


};


#endif //GRAPHICS_LEVEL_H
