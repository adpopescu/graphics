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
    void drawLevel(GLuint* textures);

private:
    forward_list<Room*> rooms;
    VECTOR3D room1_Origin;
    VECTOR3D room2_Origin;
    VECTOR3D room3_Origin;
    float room1_size;
    float room2_size;
    float room3_size;

};


#endif //GRAPHICS_LEVEL_H
