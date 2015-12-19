//
// Created by dapopesc on 17/12/15.
//

#include "Room.h"

Room::Room(VECTOR3D location, float size, int door[4]) {

    doorsInRoom = door;
    roomLocation = location;
    roomSize = size;
    meshSize = 8.0;
    shininess = 0.0;
    floorMesh = NULL;
    rightMesh1 = NULL;
    rightMesh2 = NULL;
    leftMesh1 = NULL;
    rightMesh2 = NULL;
    backMesh = NULL;
    frontMesh = NULL;
    ceilingMesh = NULL;
    ambient = VECTOR3D(0.0f,0.0f,0.0f);
    specular= VECTOR3D(0.0f,0.0f,0.0f);
    diffuse= VECTOR3D(0.9f,0.5f,0.0f);
}

void Room::initRoom() {

    //floor
    origin = VECTOR3D(-(roomSize / 2.0) + roomLocation.x, 0.0f + roomLocation.y, (roomSize / 2.0) + roomLocation.z);
    dir1v = VECTOR3D(1.0f, 0.0f, 0.0f);
    dir2v = VECTOR3D(0.0f, 0.0f, -1.0f);
    floorMesh = new QuadMesh((roomSize / meshSize), roomSize);
    floorMesh->InitMesh((roomSize / meshSize), origin, roomSize, roomSize, dir1v, dir2v);
    floorMesh->SetMaterial(ambient, diffuse, specular, shininess);

    //ceiling
    origin = VECTOR3D((roomSize / 2.0) + roomLocation.x, heightOfRoom + roomLocation.y, -(roomSize / 2.0) + roomLocation.z);
    dir1v = VECTOR3D(-1.0f, 0.0f, 0.0f);
    dir2v = VECTOR3D(0.0f, 0.0f, 1.0f);
    ceilingMesh = new QuadMesh((roomSize / meshSize), roomSize);
    ceilingMesh->InitMesh((roomSize / meshSize), origin, roomSize, roomSize, dir1v, dir2v);
    ceilingMesh->SetMaterial(ambient, diffuse, specular, shininess);


    //right wall
    if (doorsInRoom[0] == 1) {
        origin = VECTOR3D((roomSize / 2.0) + roomLocation.x, 0.0f + roomLocation.y, (roomSize / 2.0) + roomLocation.z);
        dir1v = VECTOR3D(0.0f, 1.0f, 0.0f);
        dir2v = VECTOR3D(0.0f, 0.0f, -1.0f);

        rightMesh1 = new QuadMesh((roomSize / meshSize), roomSize);
        rightMesh1->InitMesh((roomSize / meshSize), origin, heightOfRoom, (roomSize-doorSize)/2, dir1v, dir2v);
        rightMesh1->SetMaterial(ambient, diffuse, specular, shininess);

        origin = VECTOR3D((roomSize / 2.0) + roomLocation.x, 0.0f + roomLocation.y,
                          (roomSize / 2.0) - (roomSize+doorSize)/2.0 + roomLocation.z);
        rightMesh2 = new QuadMesh((roomSize / meshSize), roomSize);
        rightMesh2->InitMesh((roomSize / meshSize), origin, heightOfRoom, (roomSize-doorSize)/2.0, dir1v, dir2v);
        rightMesh2->SetMaterial(ambient, diffuse, specular, shininess);
    }
    else {
        origin = VECTOR3D((roomSize / 2.0) + roomLocation.x, 0.0f + roomLocation.y, (roomSize / 2.0) + roomLocation.z);
        dir1v = VECTOR3D(0.0f, 1.0f, 0.0f);
        dir2v = VECTOR3D(0.0f, 0.0f, -1.0f);
        rightMesh1 = new QuadMesh((roomSize / meshSize), roomSize);
        rightMesh1->InitMesh((roomSize / meshSize), origin, heightOfRoom, roomSize, dir1v, dir2v);
        rightMesh1->SetMaterial(ambient, diffuse, specular, shininess);
    }


    //left wall
    if (doorsInRoom[1] == 1) {
        origin = VECTOR3D(-(roomSize / 2.0) + roomLocation.x, heightOfRoom + roomLocation.y,
                          (roomSize / 2.0) + roomLocation.z);
        dir1v = VECTOR3D(0.0f, -1.0f, 0.0f);
        dir2v = VECTOR3D(0.0f, 0.0f, -1.0f);
        leftMesh1 = new QuadMesh((roomSize / meshSize), roomSize);
        leftMesh1->InitMesh((roomSize / meshSize), origin, heightOfRoom, (roomSize-doorSize)/2, dir1v, dir2v);
        leftMesh1->SetMaterial(ambient, diffuse, specular, shininess);

        origin = VECTOR3D(-(roomSize / 2.0) + roomLocation.x, heightOfRoom + roomLocation.y,
                          (roomSize / 2.0) - (roomSize+doorSize)/2.0 + roomLocation.z);
        leftMesh2 = new QuadMesh((roomSize / meshSize), roomSize);
        leftMesh2->InitMesh((roomSize / meshSize), origin, heightOfRoom, (roomSize-doorSize)/2, dir1v, dir2v);
        leftMesh2->SetMaterial(ambient, diffuse, specular, shininess);
    }
    else {
        origin = VECTOR3D(-(roomSize / 2.0) + roomLocation.x, heightOfRoom + roomLocation.y,
                          (roomSize / 2.0) + roomLocation.z);
        dir1v = VECTOR3D(0.0f, -1.0f, 0.0f);
        dir2v = VECTOR3D(0.0f, 0.0f, -1.0f);
        leftMesh1 = new QuadMesh((roomSize / meshSize), roomSize);
        leftMesh1->InitMesh((roomSize / meshSize), origin, heightOfRoom, roomSize, dir1v, dir2v);
        leftMesh1->SetMaterial(ambient, diffuse, specular, shininess);
    }

    //back wall
    if (doorsInRoom[2] == 1) {

    }
    else {
        origin = VECTOR3D((roomSize / 2.0) + roomLocation.x, heightOfRoom + roomLocation.y,
                          -(roomSize / 2.0) + roomLocation.z);
        dir1v = VECTOR3D(-1.0f, 0.0f, 0.0f);
        dir2v = VECTOR3D(0.0f, -1.0f, 0.0f);
        backMesh = new QuadMesh((roomSize / meshSize), roomSize);
        backMesh->InitMesh((roomSize / meshSize), origin, roomSize, heightOfRoom, dir1v, dir2v);
        backMesh->SetMaterial(ambient, diffuse, specular, shininess);
    }

    //front wall
    if (doorsInRoom[3] == 1) {

    }
    else {
        origin = VECTOR3D(-(roomSize / 2.0) + roomLocation.x, 0.0f + roomLocation.y, (roomSize / 2.0) + roomLocation.z);
        dir1v = VECTOR3D(0.0f, 1.0f, 0.0f);
        dir2v = VECTOR3D(1.0f, 0.0f, 0.0f);
        frontMesh = new QuadMesh((roomSize / meshSize), roomSize);
        frontMesh->InitMesh((roomSize / meshSize), origin, heightOfRoom, roomSize, dir1v, dir2v);
        frontMesh->SetMaterial(ambient, diffuse, specular, shininess);
    }


    RobotMesh* tempRobot = new RobotMesh(roomLocation);
    robots.push_front(tempRobot);

}

void Room::drawRoom(GLuint *textures) {

    glBindTexture(GL_TEXTURE_2D, textures[0]);
    floorMesh->DrawMesh((roomSize/meshSize));
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    if (doorsInRoom[0] == 1){
        rightMesh1->DrawMesh((roomSize/meshSize));
        rightMesh2->DrawMesh((roomSize/meshSize));
    }
    else{
        rightMesh1->DrawMesh((roomSize/meshSize));
    }
    if (doorsInRoom[1] == 1){
        leftMesh1->DrawMesh((roomSize/meshSize));
        leftMesh2->DrawMesh((roomSize/meshSize));
    }
    else{
        leftMesh1->DrawMesh((roomSize/meshSize));
    }
    backMesh->DrawMesh((roomSize/meshSize));
    frontMesh->DrawMesh((roomSize/meshSize));
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    ceilingMesh->DrawMesh((roomSize/meshSize));

    for (auto it : robots){
        it->drawMesh(textures);
    }

}
