//
// Created by dapopesc on 17/12/15.
//

#ifndef GRAPHICS_LEVEL_H
#define GRAPHICS_LEVEL_H

#include "Room.h"
#include <forward_list>

using namespace std;

class Level {


public:
    Level();
    void initLevel();
    void drawLevel(GLuint *textures);
    void getRoomBBox(VECTOR3D *min, VECTOR3D *max, VECTOR3D playerPos);
    bool insideRoom(Room *room, VECTOR3D *playerPos);

private:
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

};


#endif //GRAPHICS_LEVEL_H
