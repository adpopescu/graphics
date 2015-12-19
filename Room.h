//
// Created by dapopesc on 17/12/15.
//

#ifndef GRAPHICS_ROOM_H
#define GRAPHICS_ROOM_H

#include <forward_list>
#include "QuadMesh.h"
#include "VECTOR3D.h"
#include "RobotMesh.h"

using namespace std;

class Room {

public:
    Room(VECTOR3D location, float size, int door[4]);
    void initRoom();
    void drawRoom(GLuint *textures);
    void getBBox(VECTOR3D* min, VECTOR3D* max);


    // Origin of the room
    VECTOR3D roomLocation;
    // Size of the room, rooms are squares and have standardized height.
    float roomSize;
    int* doorsInRoom;

private:
    int meshSize;
    float shininess;

    forward_list<RobotMesh*> robots;

    const float heightOfRoom = 8.0;
    const float doorSize = 4.0;

    QuadMesh *floorMesh;
    QuadMesh *rightMesh1;
    QuadMesh *rightMesh2;
    QuadMesh *leftMesh1;
    QuadMesh *leftMesh2;
    QuadMesh *backMesh;
    QuadMesh *frontMesh;
    QuadMesh *ceilingMesh;

    VECTOR3D origin;
    VECTOR3D dir1v;
    VECTOR3D dir2v;
    VECTOR3D ambient;
    VECTOR3D specular;
    VECTOR3D diffuse;

};


#endif //GRAPHICS_ROOM_H
