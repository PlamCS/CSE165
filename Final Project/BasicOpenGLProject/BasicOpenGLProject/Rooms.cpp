#include "Rooms.h"
#include "RoomManager.h"
#include <iostream>
#include <random>
#include <chrono>
#include <thread>

Room* RoomManager::currentRoom = nullptr;
Player* RoomManager::player = nullptr;
float RoomManager::score = 0;
static int generateRandomNumber(int min, int max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}

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
    for (auto& object : Room::objects) {
		object->draw();
	}

    glColor3f(1.0f, 0.0f, 0.0f);
    for (auto& enemy : Room::enemies) {
        enemy->draw();
    }

	for (auto& door : Room::doors) {
        
        int value = door->Tier();
        if (!door->Tier()) {
            if (RoomManager::score >= 500) value = generateRandomNumber(0, 100);
            else if (RoomManager::score >= 200) value = generateRandomNumber(0, 97);
            else if (RoomManager::score >= 50) value = generateRandomNumber(0, 80);
            else value = generateRandomNumber(0, 50);
            door->setTier(value);
        }

        if (Room::getRoomMemory(Room::getDoor(door))) glColor3f(0.231f, 0.231f, 0.231f);
        else if (value > 97) glColor3f(0.859f, 0.0f, 0.0f);
        else if (value > 80 ) glColor3f(0.502f, 0.0f, 1.0f);
        else if (value > 50 ) glColor3f(1.0f, 0.463f, 0.0f);
        else glColor3f(0.5f, 0.25f, 0.0f);
        door->draw();
	}

    glColor3f(0.0f, 1.0f, 0.0f);
    RoomManager::player->draw();
}

Room::Room()
{
    Room::objects = {
        new Wall(0.0f, 1.05f, 2.0f, 0.2f),
        new Wall(0.0f, -1.05f, 2.0f, 0.2f),
        new Wall(1.05f, 0.0f, 0.2f, 2.0f),
        new Wall(-1.05f, 0.0f, 0.2f, 2.0f)
    };
    Room::doors = {
        new Door(0.0f, 1.03f, 0.2f, 0.2f, false),
        new Door(0.0f, -1.03f, 0.2f, 0.2f, false),
        new Door(1.03f, 0.0f, 0.2f, 0.2f, false),
        new Door(-1.03f, 0.0f, 0.2f, 0.2f, false)
    };
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

bool Room::check(float newX, float newY, Entity* entity)
{
    
    for (auto& object : objects) {
        if (object->check(newX, newY, entity)) return true;
    }

    
    for (auto& enemy : Room::enemies) {
        enemy->check(0.0f, 0.0f, RoomManager::player);
    }

    if (RoomManager::player == entity) {
        for (auto& door : doors) {
            if (Room::enemies.empty() &&
                newX - entity->getWidth() / 2.0f <= door->getX() + door->getWidth() / 2.0f &&
                newX + entity->getWidth() / 2.0f >= door->getX() - door->getWidth() / 2.0f &&
                newY - entity->getHeight() / 2.0f <= door->getY() + door->getHeight() / 2.0f &&
                newY + entity->getHeight() / 2.0f >= door->getY() - door->getHeight() / 2.0f) {

                int returnDoor = Room::getOppositeDoor(door);
                int enteringDoor = Room::getDoor(door);

                if (!RoomManager::currentRoom->getRoomMemory(enteringDoor)) {
                    int doorTier = door->Tier();
                
                    // Handles New Room Creation (What Kind of Room)
                    int roomNumber = 0;
                    if (doorTier > 97) roomNumber = generateRandomNumber(12, 15);
                    else if (doorTier > 80) roomNumber = generateRandomNumber(8, 11);
                    else if (doorTier > 50) roomNumber = generateRandomNumber(4, 7);
                    else { roomNumber = generateRandomNumber(0, 3); RoomManager::score += 10; }

                
                    std::vector<Room*> rooms = {new LWallRoom(), new InvertedLWallRoom(), new WallRoom(), new InvertWallRoom() };

                    Room* nextRoom = rooms[roomNumber];
                    for (auto& room : rooms) {
                        if (room != nextRoom) delete room;
                    }
                    //RoomManager::enemies = generateRandomNumber(2, 5);

                    RoomManager::currentRoom->addRoomMemory(enteringDoor, nextRoom);
                    nextRoom->addRoomMemory(returnDoor, RoomManager::currentRoom);
                    RoomManager::currentRoom = nextRoom;
                }
                else {
                    RoomManager::currentRoom = Room::getRoomMemory(enteringDoor);
                }

                entity->setX(-entity->getX());
                entity->setY(-entity->getY());

                return true;
            }
        }
    }

    return false;
}

LWallRoom::LWallRoom() {
    float width = 0.3f;
    float height = 0.05f;

    Room::objects.push_back(new SpikeTrap(0.3f, 0.3f, 0.3f, 0.3f, std::chrono::milliseconds(generateRandomNumber(500, 1000))));
    Room::objects.push_back(new SpikeTrap(0.3f, -0.3f, 0.3f, 0.3f, std::chrono::milliseconds(generateRandomNumber(500, 1000))));
    Room::objects.push_back(new SpikeTrap(-0.3f, -0.3f, 0.3f, 0.3f, std::chrono::milliseconds(generateRandomNumber(500, 1000))));
    Room::objects.push_back(new SpikeTrap(-0.3f, 0.3f, 0.3f, 0.3f, std::chrono::milliseconds(generateRandomNumber(500, 1000))));
    Room::objects.push_back(new LWall(0.4f, 0.4f, width, height, 0));
    Room::objects.push_back(new LWall(0.4f, -0.4f, width, height, 90));
    Room::objects.push_back(new LWall(-0.4f, -0.4f, width, height, 180));
    Room::objects.push_back(new LWall(-0.4f, 0.4f, width, height, 270));

    

}

InvertedLWallRoom::InvertedLWallRoom()
{

    
    float width = 0.3f;
    float height = 0.05f;
    Room::objects.push_back(new SpikeTrap(0.5f, 0.5f, 0.3f, 0.3f, std::chrono::milliseconds(generateRandomNumber(500, 1000))));
    Room::objects.push_back(new SpikeTrap(0.5f, -0.5f, 0.3f, 0.3f, std::chrono::milliseconds(generateRandomNumber(500, 1000))));
    Room::objects.push_back(new SpikeTrap(-0.5f, -0.5f, 0.3f, 0.3f, std::chrono::milliseconds(generateRandomNumber(500, 1000))));
    Room::objects.push_back(new SpikeTrap(-0.5f, 0.5f, 0.3f, 0.3f, std::chrono::milliseconds(generateRandomNumber(500, 1000))));
    Room::objects.push_back(new LWall(0.4f, 0.4f, width, height, 180));
    Room::objects.push_back(new LWall(0.4f, -0.4f, width, height, 270));
    Room::objects.push_back(new LWall(-0.4f, -0.4f, width, height, 0));
    Room::objects.push_back(new LWall(-0.4f, 0.4f, width, height, 90));
}

WallRoom::WallRoom()
{
    float width = 0.7f;
    float height = 0.05f;
    Room::objects.push_back(new Wall(0.6f, 0.6f, width, height));
    Room::objects.push_back(new Wall(-0.6f, 0.6f, width, height));
    Room::objects.push_back(new Wall(0.6f, -0.6f, width, height));
    Room::objects.push_back(new Wall(-0.6f, -0.6f, width, height));
    Room::objects.push_back(new Wall(0.0f, 0.0f, width, height));

    
}

InvertWallRoom::InvertWallRoom()
{
    float width = 0.7f;
    float height = 0.05f;
    Room::objects.push_back(new Wall(0.6f, 0.6f, height, width));
    Room::objects.push_back(new Wall(-0.6f, 0.6f, height, width));
    Room::objects.push_back(new Wall(0.6f, -0.6f, height, width));
    Room::objects.push_back(new Wall(-0.6f, -0.6f, height, width));
    Room::objects.push_back(new Wall(0.0f, 0.0f, height, width));
}

BeginningRoom::BeginningRoom()
{
    Room::doors.clear();
    Room::doors.push_back(new Door(1.5f, 0.0f, 0.2f, 0.2f, false));
    Room::doors.push_back(new Door(1.5f, 0.0f, 0.2f, 0.2f, false));
    Room::doors.push_back(new Door(1.03f, 0.0f, 0.2f, 0.2f, false));
    Room::doors.push_back(new Door(1.5f, 0.0f, 0.2f, 0.2f, false));

    Room::enemies.push_back(new Enemy(-0.7f, 0.7f, 0.2f, 0.2f));
    Room::enemies.push_back(new Enemy(0.7f, -0.7f, 0.2f, 0.2f));

}
