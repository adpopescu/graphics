//
// Created by dapopesc on 17/12/15.
//

#include "Room.h"

Room::Room(VECTOR3D location, float size) {

    roomLocation = location;
    roomSize = size;
    meshSize = 8.0;
    shininess = 0.0;
    floorMesh = NULL;
    rightMesh = NULL;
    leftMesh = NULL;
    backMesh = NULL;
    frontMesh = NULL;
    ceilingMesh = NULL;
    ambient = VECTOR3D(0.0f,0.0f,0.0f);
    specular= VECTOR3D(0.0f,0.0f,0.0f);
    diffuse= VECTOR3D(0.9f,0.5f,0.0f);
}

void Room::initRoom() {

    //floor
    origin  = VECTOR3D(-(roomSize/2.0)+roomLocation.x,0.0f+roomLocation.y,(roomSize/2.0)+roomLocation.z);
    dir1v   = VECTOR3D(1.0f, 0.0f, 0.0f);
    dir2v   = VECTOR3D(0.0f, 0.0f,-1.0f);
    floorMesh = new QuadMesh((roomSize/meshSize), roomSize);
    floorMesh->InitMesh((roomSize/meshSize), origin, roomSize, roomSize, dir1v, dir2v);
    floorMesh->SetMaterial(ambient,diffuse,specular,shininess);

    //ceiling
    origin = VECTOR3D((roomSize/2.0)+roomLocation.x, heightOfRoom+roomLocation.y, -(roomSize/2.0)+roomLocation.z);
    dir1v = VECTOR3D(-1.0f, 0.0f, 0.0f);
    dir2v = VECTOR3D(0.0f, 0.0f, 1.0f);
    ceilingMesh = new QuadMesh((roomSize/meshSize), roomSize);
    ceilingMesh->InitMesh((roomSize/meshSize), origin, roomSize, roomSize, dir1v, dir2v);
    ceilingMesh->SetMaterial(ambient, diffuse, specular, shininess);


    //right wall
    origin  = VECTOR3D((roomSize/2.0)+roomLocation.x,0.0f+roomLocation.y,(roomSize/2.0)+roomLocation.z);
    dir1v   = VECTOR3D(0.0f, 1.0f, 0.0f);
    dir2v   = VECTOR3D(0.0f, 0.0f,-1.0f);
    rightMesh = new QuadMesh((roomSize/meshSize), roomSize);
    rightMesh->InitMesh((roomSize/meshSize), origin, heightOfRoom, roomSize, dir1v, dir2v);
    rightMesh->SetMaterial(ambient,diffuse,specular,shininess);

    //left wall
    origin  = VECTOR3D(-(roomSize/2.0)+roomLocation.x,heightOfRoom+roomLocation.y,(roomSize/2.0)+roomLocation.z);
    dir1v   = VECTOR3D(0.0f, -1.0f, 0.0f);
    dir2v   = VECTOR3D(0.0f, 0.0f,-1.0f);
    leftMesh = new QuadMesh((roomSize/meshSize), roomSize);
    leftMesh->InitMesh((roomSize/meshSize), origin, heightOfRoom, roomSize, dir1v, dir2v);
    leftMesh->SetMaterial(ambient,diffuse,specular,shininess);

    //back wall
    origin  = VECTOR3D((roomSize/2.0)+roomLocation.x, heightOfRoom+roomLocation.y, -(roomSize/2.0)+roomLocation.z);
    dir1v   = VECTOR3D(-1.0f, 0.0f, 0.0f);
    dir2v   = VECTOR3D(0.0f, -1.0f, 0.0f);
    backMesh = new QuadMesh((roomSize/meshSize), roomSize);
    backMesh->InitMesh((roomSize/meshSize), origin, roomSize, heightOfRoom, dir1v, dir2v);
    backMesh->SetMaterial(ambient,diffuse,specular,shininess);

    //front wall
    origin = VECTOR3D(-(roomSize/2.0)+roomLocation.x, 0.0f+roomLocation.y, (roomSize/2.0)+roomLocation.z);
    dir1v = VECTOR3D(0.0f, 1.0f, 0.0f);
    dir2v = VECTOR3D(1.0f, 0.0f, 0.0f);
    frontMesh = new QuadMesh((roomSize/meshSize), roomSize);
    frontMesh->InitMesh((roomSize/meshSize), origin, heightOfRoom, roomSize, dir1v, dir2v);
    frontMesh->SetMaterial(ambient, diffuse, specular, shininess);

}

void Room::drawRoom(GLuint *textures) {

    glBindTexture(GL_TEXTURE_2D, textures[0]);
    floorMesh->DrawMesh((roomSize/meshSize));
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    rightMesh->DrawMesh((roomSize/meshSize));
    leftMesh->DrawMesh((roomSize/meshSize));
    backMesh->DrawMesh((roomSize/meshSize));
    frontMesh->DrawMesh((roomSize/meshSize));
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    ceilingMesh->DrawMesh((roomSize/meshSize));

}
