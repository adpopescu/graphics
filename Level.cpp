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

    Room* tempRoom = new Room(room1_Origin, room1_size, room1_doors);
    rooms.push_front(tempRoom);
    tempRoom = new Room(room2_Origin, room2_size, room2_doors);
    rooms.push_front(tempRoom);
    tempRoom = new Room(room3_Origin, room3_size, room3_doors);
    rooms.push_front(tempRoom);

    for (auto it : rooms){
        it->initRoom();
    }

}

void Level::drawLevel(GLuint *textures) {

    for (auto it : rooms){
        it->drawRoom(textures);
    }

}

void Level::getRoomBBox(VECTOR3D *min, VECTOR3D *max, VECTOR3D playerPos) {

    for (auto it : rooms){
        if (insideRoom(it, &playerPos)){
            it->getBBox(min, max);
            if ((playerPos.GetX() > it->roomLocation.GetX()) && it->doorsInRoom[0]){
                cout << "In right side of room" << endl;
                if ((playerPos.GetZ() > (it->roomLocation.GetZ() - 1)) && (playerPos.GetZ() < (it->roomLocation.GetZ() + 1))){
                    max->SetX(max->GetX() + 2.0);
                }
            }
            else if ((playerPos.GetX() < it->roomLocation.GetX()) && it->doorsInRoom[1]){
                cout << "In left side of room" << endl;
                if ((playerPos.GetZ() > (it->roomLocation.GetZ() - 1)) && (playerPos.GetZ() < (it->roomLocation.GetZ() + 1))){
                    min->SetX(min->GetX() - 2.0);
                }
            }
            cout << "Player Pos X: {" << playerPos.GetX() <<  "}" << std::endl;
            cout << "Player Pos Y: {" << playerPos.GetY() <<  "}" << std::endl;
            cout << "Player Pos Z: {" << playerPos.GetZ() <<  "}" << std::endl;

            cout << "Room BBox X: {" << min->GetX() << ", " << max->GetX() << "}" << std::endl;
            cout << "Room BBox Y: {" << min->GetY() << ", " << max->GetY() << "}" << std::endl;
            cout << "Room BBox Z: {" << min->GetZ() << ", " << max->GetZ() << "}" << std::endl;

            return;
        }
    }
    exit(222);

}

bool Level::insideRoom(Room *room, VECTOR3D *playerPos) {
    VECTOR3D roomMin, roomMax;

    room->getBBox(&roomMin, &roomMax);

    if (playerPos->GetX() <= roomMax.GetX() && playerPos->GetX() > roomMin.GetX()){
        if (playerPos->GetY() <= roomMax.GetY() && playerPos->GetY() > roomMin.GetY()) {
            if (playerPos->GetZ() <= roomMax.GetZ() && playerPos->GetZ() > roomMin.GetZ()) {
                return true;
            }
        }
    }
    return false;
}
