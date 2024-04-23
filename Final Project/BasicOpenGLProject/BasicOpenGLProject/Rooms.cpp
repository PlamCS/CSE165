#include "Rooms.h"
#include "RoomManager.h"
#include <iostream>
#include <random>

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
		object.draw();
	}
    glColor3f(0.5f, 0.25f, 0.0f);
	for (auto& door : doors) {
        door->draw();
	}
}

Room* RoomManager::currentRoom = nullptr;
int RoomManager::score = 0;
bool Room::check(float newX, float newY, Entity& Player)
{
    for (auto& object : objects) {
        if (newX - Player.getWidth() / 2.0f <= object.getX() + object.getWidth() / 2.0f &&
            newX + Player.getWidth() / 2.0f >= object.getX() - object.getWidth() / 2.0f &&
            newY - Player.getHeight() / 2.0f <= object.getY() + object.getHeight() / 2.0f &&
            newY + Player.getHeight() / 2.0f >= object.getY() - object.getHeight() / 2.0f) {
            return true;
        }
    }

    for (auto& door : doors) {
        if (newX - Player.getWidth() / 2.0f <= door->getX() + door->getWidth() / 2.0f &&
            newX + Player.getWidth() / 2.0f >= door->getX() - door->getWidth() / 2.0f &&
            newY - Player.getHeight() / 2.0f <= door->getY() + door->getHeight() / 2.0f &&
            newY + Player.getHeight() / 2.0f >= door->getY() - door->getHeight() / 2.0f) {

            int returnDoor = -1;
            if (door == Room::doors[0]) returnDoor = 1;
            else if (door == Room::doors[1]) returnDoor = 0;
            else if (door == Room::doors[2]) returnDoor = 3;
            else if (door == Room::doors[3]) returnDoor = 2;

            int enteringDoor = -1;
            if (door == Room::doors[0]) enteringDoor = 0;
            else if (door == Room::doors[1]) enteringDoor = 1;
            else if (door == Room::doors[2]) enteringDoor = 2;
            else if (door == Room::doors[3]) enteringDoor = 3;

            if (!RoomManager::currentRoom->getRoomMemory(enteringDoor)) {
                std::cout << "New Room" << std::endl;
                RoomManager::score+=10;
                
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<int> dist(0, 20);

                // Handles New Room Creation
                Room* nextRoom = new Room();
                int randomNumber = dist(gen);
                std::cout << randomNumber << std::endl;
                if( randomNumber < 10 ) nextRoom = new TierOneLongWallRoom();
                else if ( randomNumber >= 10 ) nextRoom = new TierOneShortWallRoom();
                

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
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(-.6f, .6f);
    Room::objects.push_back(Wall(dist(gen), dist(gen), 0.2f, 0.1f));
}

TierOneLongWallRoom::TierOneLongWallRoom() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(-.6f, .6f);
    Room::objects.push_back(Wall(dist(gen), dist(gen), 0.4f, 0.1f));
}
