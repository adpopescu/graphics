//
// Created by dapopesc on 17/12/15.
//

#include "Level.h"

#define LEVEL_DEBUG_MODE 0


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

    Room* tempRoom = new Room(room1_Origin, room1_size, room1_doors, 2);
    rooms.push_front(tempRoom);
    tempRoom = new Room(room2_Origin, room2_size, room2_doors, 1);
    rooms.push_front(tempRoom);
    tempRoom = new Room(room3_Origin, room3_size, room3_doors, 4);
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

bool Level::getRoomBBox(VECTOR3D *min, VECTOR3D *max, VECTOR3D objectPos) {

    for (auto it : rooms){
        if (insideRoom(it, &objectPos)){
            it->getBBox(min, max);
            if ((objectPos.GetX() > it->roomLocation.GetX()) && it->doorsInRoom[0]){
                if (LEVEL_DEBUG_MODE){
                    cout << "In right side of room" << endl;
                }

                if ((objectPos.GetZ() > (it->roomLocation.GetZ() - 1)) && (objectPos.GetZ() < (it->roomLocation.GetZ() + 1))){
                    max->SetX(max->GetX() + 2.0);
                }
            }
            else if ((objectPos.GetX() < it->roomLocation.GetX()) && it->doorsInRoom[1]){
                if (LEVEL_DEBUG_MODE){
                    cout << "In left side of room" << endl;
                }

                if ((objectPos.GetZ() > (it->roomLocation.GetZ() - 1)) && (objectPos.GetZ() < (it->roomLocation.GetZ() + 1))){
                    min->SetX(min->GetX() - 2.0);
                }
            }
            if (LEVEL_DEBUG_MODE){
                cout << "Object Pos X: {" << objectPos.GetX() << "}" << std::endl;
                cout << "Object Pos Y: {" << objectPos.GetY() << "}" << std::endl;
                cout << "Object Pos Z: {" << objectPos.GetZ() << "}" << std::endl;

                cout << "Room BBox X: {" << min->GetX() << ", " << max->GetX() << "}" << std::endl;
                cout << "Room BBox Y: {" << min->GetY() << ", " << max->GetY() << "}" << std::endl;
                cout << "Room BBox Z: {" << min->GetZ() << ", " << max->GetZ() << "}" << std::endl;
            }


            return true;
        }
    }
    return false;

}

bool Level::insideRoom(Room *room, VECTOR3D *objectPos) {
    VECTOR3D roomMin, roomMax;

    room->getBBox(&roomMin, &roomMax);

    if (objectPos->GetX() <= roomMax.GetX() && objectPos->GetX() > roomMin.GetX()){
        if (objectPos->GetY() <= roomMax.GetY() && objectPos->GetY() > roomMin.GetY()) {
            if (objectPos->GetZ() <= roomMax.GetZ() && objectPos->GetZ() > roomMin.GetZ()) {
                return true;
            }
        }
    }
    return false;
}

Room* Level::getRoom(VECTOR3D objectPos) {
    for (auto it : rooms){
        if (insideRoom(it, &objectPos)){
            return it;
        }
    }
    exit(100);
}

void Level::animateRooms(VECTOR3D playerPos) {
    cout << "Animating rooms" << endl;
    for (auto room : rooms){
        if (insideRoom(room, &playerPos)){
            for (auto rob : room->robots){
                float zDif = playerPos.GetZ() - rob->getPos().GetZ();
                float xDif = playerPos.GetX() - rob->getPos().GetX();

                cout << "X dif: " << xDif << endl;
                cout << "Z dif: " << zDif << endl;
                if (xDif > 0.0){
                    rob->angle = -atan(zDif / xDif) / (2 * PI) * 360;
                }
                else {
                    rob->angle = -atan(zDif / xDif) / (2 * PI) * 360 + 180;
                }
                if (curCount < proCount){
                    ProjectileMesh *tempProjectile = new ProjectileMesh(rob);
                    projectileList.push_front(tempProjectile);
                    curCount++;
                }
            }
        }
        else{
            int randNum = rand() % 100;
            if (randNum <= 50){
                if (robotsSoFar <= maxLevelRobots){
                    if (room->makeRobot()){
                        robotsSoFar++;
                    }
                }
            }
            VECTOR3D robMin, robMax;
            VECTOR3D roomMin, roomMax;
            for (auto rob : room->robots) {
                rob->tx += enemySpeed * cos(rob->angle * 2 * PI / 360);
                rob->tz -= enemySpeed * sin(rob->angle * 2 * PI / 360);
                rob->getBBox(&robMin, &robMax);
                room->getBBox(&roomMin, &roomMax);
                if (robMax.z > roomMax.z){
                    rob->tz = roomMax.z - 0.5 * (robMax.z - robMin.z);
                    rob->angle += 135;
                }
                else if (robMin.z < roomMin.z) {
                    rob->tz = roomMin.z + 0.5 * (robMax.z - robMin.z);
                    rob->angle += 135;
                }
                if (robMax.x > roomMax.x) {
                    rob->tx = roomMax.x - 0.5 * (robMax.x - robMin.x);
                    rob->angle += 135;
                }
                else if (robMin.x < roomMin.x) {
                    rob->tx = roomMin.x + 0.5 * (robMax.x - robMin.x);
                    rob->angle += 135;
                }
            }
        }
    }
}
