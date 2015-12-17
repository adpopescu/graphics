//
// Created by dapopesc on 17/12/15.
//

#include "Level.h"


Level::Level() {

    room1_size = 32.0;
    room2_size = 16.0;
    room3_size = 64.0;

    room1_Origin = VECTOR3D(0.0, 0.0, 0.0);
    room2_Origin = VECTOR3D(-(room1_size/2.0+room2_size/2.0), 0.0, 0.0);
    room3_Origin = VECTOR3D((room1_size/2.0+room3_size/2.0), 0.0, 0.0);

    initLevel();
}


void Level::initLevel() {

    Room* tempRoom = new Room(room1_Origin, room1_size);
    rooms.push_front(tempRoom);
    tempRoom = new Room(room2_Origin, room2_size);
    rooms.push_front(tempRoom);
    tempRoom = new Room(room3_Origin, room3_size);
    rooms.push_front(tempRoom);

    for (auto it : rooms){
        it->initRoom();
    }

}

void Level::drawLevel(GLuint* textures) {

    for (auto it : rooms){
        it->drawRoom(textures);
    }

}
