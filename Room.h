//
// Created by dapopesc on 17/12/15.
//

#ifndef GRAPHICS_ROOM_H
#define GRAPHICS_ROOM_H

#include "QuadMesh.h"
#include "VECTOR3D.h"



class Room {

public:
    Room(VECTOR3D location, float size);
    void initRoom();
    void drawRoom(GLuint *textures);
    // Origin of the room
    VECTOR3D roomLocation;
    // Size of the room, rooms are squares and have standardized height.
    float roomSize;

private:
    int meshSize;
    float shininess;

    const float heightOfRoom = 8.0;

    QuadMesh *floorMesh;
    QuadMesh *rightMesh;
    QuadMesh *leftMesh;
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
