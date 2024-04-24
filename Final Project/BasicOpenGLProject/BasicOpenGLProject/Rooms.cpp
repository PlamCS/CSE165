#include "Rooms.h"
#include "RoomManager.h"
#include <iostream>
#include <random>
#include <chrono>
#include <thread>

void Room::addRoomMemory(int door, Room* room)
{
    Room::roomMemory[(int)door] = room;
}

Room* Room::getRoomMemory(int door)
{
    if (Room::roomMemory.find(door) != Room::roomMemory.end())
        return Room::roomMemory.at(door);
    return nullptr;
}

void Room::draw()
{
	glColor3f(0.329f, 0.329f, 0.329f);
	for (auto& object : objects) {
		object->draw();
	}
	for (auto& door : doors) {
        glColor3f(0.5f, 0.25f, 0.0f);
        if (Room::getRoomMemory(Room::getDoor(door))) glColor3f(0.231f, 0.231f, 0.231f);
        door->draw();
	}
}

int Room::getOppositeDoor(Door* door) {
    int returnDoor = -1;
    if (door == Room::doors[0]) returnDoor = 1;
    else if (door == Room::doors[1]) returnDoor = 0;
    else if (door == Room::doors[2]) returnDoor = 3;
    else if (door == Room::doors[3]) returnDoor = 2;
    return returnDoor;
}

int Room::getDoor(Door * door) {
    int enteringDoor = -1;
    if (door == Room::doors[0]) enteringDoor = 0;
    else if (door == Room::doors[1]) enteringDoor = 1;
    else if (door == Room::doors[2]) enteringDoor = 2;
    else if (door == Room::doors[3]) enteringDoor = 3;
    return enteringDoor;
}

static int generateRandomNumber(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

Room* RoomManager::currentRoom = nullptr;
float RoomManager::score = 0;
int RoomManager::enemies = 0;
bool Room::check(float newX, float newY, Entity& Player)
{
    for (auto& object : objects) {
        if (object->check(newX, newY, &Player)) return true;
    }

    for (auto& door : doors) {
        if (!RoomManager::enemies &&
            newX - Player.getWidth() / 2.0f <= door->getX() + door->getWidth() / 2.0f &&
            newX + Player.getWidth() / 2.0f >= door->getX() - door->getWidth() / 2.0f &&
            newY - Player.getHeight() / 2.0f <= door->getY() + door->getHeight() / 2.0f &&
            newY + Player.getHeight() / 2.0f >= door->getY() - door->getHeight() / 2.0f) {

            int returnDoor = Room::getOppositeDoor(door);
            int enteringDoor = Room::getDoor(door);

            if (!RoomManager::currentRoom->getRoomMemory(enteringDoor)) {
                std::cout << "New Room" << std::endl;
                RoomManager::score+=10;
                
                
                int randomNumber = generateRandomNumber(0,20);

                // Handles New Room Creation
                Room* nextRoom = new TierOneShortWallRoom();
                //if( randomNumber < 10 ) nextRoom = new TierOneLongWallRoom();
                //else if ( randomNumber >= 10 ) nextRoom = new TierOneShortWallRoom();
                //RoomManager::enemies = generateRandomNumber(2, 5);

                RoomManager::currentRoom->addRoomMemory(enteringDoor, nextRoom);
                nextRoom->addRoomMemory(returnDoor, RoomManager::currentRoom);
                RoomManager::currentRoom = nextRoom;
            }
            else {
                std::cout << "Going Back a Room" << std::endl;
                RoomManager::currentRoom = Room::getRoomMemory(enteringDoor);
            }

            Player.setX(-Player.getX());
            Player.setY(-Player.getY());

            return true;
        }
    }

    return false;
}

TierOneShortWallRoom::TierOneShortWallRoom() {
    float width = 0.3f;
    float height = 0.05f;

    int randomNumber = generateRandomNumber(0, 1);
    if (randomNumber == 1) {
        objects.push_back(new SpikeTrap(0.3f, 0.3f, 0.3f, 0.3f, std::chrono::milliseconds(generateRandomNumber(300, 600))));
        objects.push_back(new SpikeTrap(0.3f, -0.3f, 0.3f, 0.3f, std::chrono::milliseconds(generateRandomNumber(300, 600))));
        objects.push_back(new SpikeTrap(-0.3f, -0.3f, 0.3f, 0.3f, std::chrono::milliseconds(generateRandomNumber(300, 600))));
        objects.push_back(new SpikeTrap(-0.3f, 0.3f, 0.3f, 0.3f, std::chrono::milliseconds(generateRandomNumber(300, 600))));
        Room::objects.push_back(new LWall(0.4f, 0.4f, width, height, 0));
        Room::objects.push_back(new LWall(0.4f, -0.4f, width, height, 90));
        Room::objects.push_back(new LWall(-0.4f, -0.4f, width, height, 180));
        Room::objects.push_back(new LWall(-0.4f, 0.4f, width, height, 270));
    }else {
        objects.push_back(new SpikeTrap(0.5f, 0.5f, 0.3f, 0.3f, std::chrono::milliseconds(generateRandomNumber(300, 600))));
        objects.push_back(new SpikeTrap(0.5f, -0.5f, 0.3f, 0.3f, std::chrono::milliseconds(generateRandomNumber(300, 600))));
        objects.push_back(new SpikeTrap(-0.5f, -0.5f, 0.3f, 0.3f, std::chrono::milliseconds(generateRandomNumber(300, 600))));
        objects.push_back(new SpikeTrap(-0.5f, 0.5f, 0.3f, 0.3f, std::chrono::milliseconds(generateRandomNumber(300, 600))));
        Room::objects.push_back(new LWall(0.4f, 0.4f, width, height, 180));
        Room::objects.push_back(new LWall(0.4f, -0.4f, width, height, 270));
        Room::objects.push_back(new LWall(-0.4f, -0.4f, width, height, 0));
        Room::objects.push_back(new LWall(-0.4f, 0.4f, width, height, 90));
    }
}

TierOneLongWallRoom::TierOneLongWallRoom() {
    
}
