#include "Rooms.h"
#include "RoomManager.h"
#include <iostream>

void Room::addRoomMemory(Door* door, Room* room)
{
    Room::roomMemory[door] = room;
}

Room* Room::getRoomMemory(Door* door)
{
    if (Room::roomMemory.find(door) != Room::roomMemory.end()) return Room::roomMemory[door];
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
        door.draw();
	}
}

Room* RoomManager::currentRoom = nullptr;
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
        if (newX - Player.getWidth() / 2.0f <= door.getX() + door.getWidth() / 2.0f &&
            newX + Player.getWidth() / 2.0f >= door.getX() - door.getWidth() / 2.0f &&
            newY - Player.getHeight() / 2.0f <= door.getY() + door.getHeight() / 2.0f &&
            newY + Player.getHeight() / 2.0f >= door.getY() - door.getHeight() / 2.0f) {

            std::cout << "Entering Door" << std::endl;
            std::cout << !RoomManager::currentRoom->getRoomMemory(&door) << std::endl;
            if (!RoomManager::currentRoom->getRoomMemory(&door)) {
                std::cout << "New Room" << std::endl;

                Door* returnDoor = nullptr;
                if (&door == &Room::doors[0]) { returnDoor = &Room::doors[1]; }
                else if (&door == &Room::doors[1]) { returnDoor = &Room::doors[0]; }
                else if (&door == &Room::doors[2]) { returnDoor = &Room::doors[3]; }
                else if (&door == &Room::doors[3]) { returnDoor = &Room::doors[2]; }

                Room* nextRoom = new Room();
                RoomManager::currentRoom->addRoomMemory(&door, nextRoom);
                nextRoom->addRoomMemory(returnDoor, RoomManager::currentRoom);
                RoomManager::currentRoom = nextRoom;
            }
            else {
                std::cout << "Going Back a Room" << std::endl;
            }

            Player.setX(-Player.getX());
            Player.setY(-Player.getY());

            return true;
        }
    }

    return false;
}