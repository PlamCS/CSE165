#include "RoomManager.h"
#include "Trap.h"
#include "Rooms.h"
#include "EnemyTypes.h"
#include "Items.h"
#include <random>
#include <iostream>

std::string getTypeName(const Entity* entity) {
    if (dynamic_cast<const Trap*>(entity)) {
        return "Trap";
    }
    else if (dynamic_cast<const Wall*>(entity) || dynamic_cast<const LWall*>(entity)) {
        return "Wall";
    }
    else {
        return "Unknown";
    }
}

Room* RoomManager::currentRoom = nullptr;
Player* RoomManager::player = nullptr;
float RoomManager::score = 0;
float RoomManager::playerMS = 1.0f;

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
    Room::projectiles = {};
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

void Room::draw()
{


    glColor3f(0.329f, 0.329f, 0.329f);
    for (auto& object : Room::objects) {
		object->draw();
	}

    for (auto& projectile : Room::projectiles) {
        if (!projectile->Friendly()) glColor3f(0.0f, 0.5f, 1.0f);
        else glColor3f(1.0f, 0.5f, 0.0f);
        projectile->draw();
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

bool Room::check(float newX, float newY, Entity* entity)
{
    if (RoomManager::player->getHealth() == 0) {
        return true;
    }

    bool collides = false;
    for (int index = 0; index < Room::objects.size(); ++index) {
        Entity* object = Room::objects[index];
        if (object->check(newX, newY, entity) && !dynamic_cast<const Item*>(object)) collides = true;
        if (object->check(newX, newY, entity) && dynamic_cast<const Item*>(object)) object->Mark();
        if (object->isMarked()) {
            delete object;
            Room::objects.erase(Room::objects.begin() + index);
            index--;
            break;
        }
    }


    for (int index = 0; index < Room::projectiles.size(); ++index) {
        Projectile* projectile = Room::projectiles[index];
        bool marked = false;
        
        for (auto& enemy : Room::enemies) {
            if (enemy->check(projectile->getX(), projectile->getY(), projectile) && !projectile->Friendly()) {
                std::cout << "Enemy is Hit" << std::endl;
                RoomManager::score += 10;
                projectile->Mark();
                enemy->Mark();
                break;
            }
        }

        for (auto& object : RoomManager::currentRoom->getObjects()) {
            if (getTypeName(object) == "Trap") continue;
            else if (object->check(projectile->getX(), projectile->getY(), projectile)) {
                projectile->Mark();
                break;
            }
        }
       
        if (RoomManager::player->check(projectile->getX(), projectile->getY(), projectile) && projectile->Friendly()) {
            delete projectile;
            Room::projectiles.erase(Room::projectiles.begin() + index);
            RoomManager::player->decreaseHealth();
            break;
        }

        
        if (!projectile->isMarked()) {
            projectile->move(projectile->getSpeed() * projectile->getDx(), projectile->getSpeed() * projectile->getDy());
        } 
        else {
            delete projectile;
            Room::projectiles.erase(Room::projectiles.begin() + index);
            index--;
        }
    }
    
    for (int index = 0; index < Room::enemies.size(); ++index) {
        Enemy* enemy = Room::enemies[index];

        if (enemy->check(newX, newY, entity)) collides = true;
        
        for (auto& object : objects) {
            if (enemy->check(object->getX(), object->getY(), object)) return true;
        }
        //std::random_device rd;
        //std::mt19937 gen(rd());
        //float min_val = -0.01f * RoomManager::playerMS;
        //float max_val = 0.01f * RoomManager::playerMS;
        //std::uniform_real_distribution<float> dis(min_val, max_val);
        
        //if (!enemy->isMarked()) enemy->move(dis(gen), dis(gen));


        float dx = (RoomManager::player->getX() > enemy->getX()) ? 0.008f : (RoomManager::player->getX() < enemy->getX()) ? -0.008f : 0.0f;
        float dy = (RoomManager::player->getY() > enemy->getY()) ? 0.008f : (RoomManager::player->getY() < enemy->getY()) ? -0.008f : 0.0f;

        if (!enemy->isMarked()) enemy->move(dx, dy);
        if (!enemy->isReady()) {
            enemy->shoot();
        }
        if (enemy->isMarked()) {
            delete enemy;
            Room::enemies.erase(Room::enemies.begin() + index);
            index--;
            break;
        }

        
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
                    else if (doorTier > 50) { roomNumber = generateRandomNumber(4, 7); RoomManager::score += 25; }
                    else { roomNumber = generateRandomNumber(0, 3); RoomManager::score += 10; }

                    std::vector<Room*> rooms = {new LWallRoom(), new InvertedLWallRoom(), new WallRoom(), new InvertWallRoom(), 
                                                new CenterPillarRoom(), new FourPillarRoom(), new TrapRoom(), new InvertedTrapRoom() };

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

                collides = true;
            }
        }
    }

    return collides;
}

void Room::removeRoomMemory(Room* room) {
    for (auto it = roomMemory.begin(); it != roomMemory.end(); ++it) {
        if (it->second == room) {
            roomMemory.erase(it);
            break;
        }
    }
}

Room::~Room()
{
    if (!objects.empty()) {
        for (auto& object : objects) {
            delete object;
        }
    }
    objects.clear();
    if (!doors.empty()) {
        for (auto& door : doors) {
            delete door;
        }
    }
    doors.clear();
    if (!projectiles.empty()) {
        for (auto& projectile : projectiles) {
            delete projectile;
        }
    }
    projectiles.clear();
    if (!roomMemory.empty()) {
        for (auto& room : roomMemory) {
            room.second->removeRoomMemory(this);
            delete room.second;
        }
    }
    roomMemory.clear();
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

    Room::objects.push_back(new Wall(-0.5f, -0.5f, 0.075f, 0.075f));
    Room::objects.push_back(new Wall(0.5f, -0.5f, 0.075f, 0.075f));
    Room::objects.push_back(new Wall(-0.5f, 0.5f, 0.075f, 0.075f));
    Room::objects.push_back(new Wall(0.5f, 0.5f, 0.075f, 0.075f));


    //Enemy* enemyTest = new InvincibleEnemy(0.4f, 0.4f, 0.1f, 0.1f);
    //enemyTest->setSpeed(1.0f);
    //Room::enemies.push_back(enemyTest);
    Entity* itemTest = new SpeedBuff(-0.4f, -0.4f, 0.01f, 0.01f);
    Room::objects.push_back(itemTest);

}

CenterPillarRoom::CenterPillarRoom() {
    Room::objects.push_back(new Wall(0.0f, 0.0f, 0.8f, 0.8f));
    Room::objects.push_back(new StatusTrap(0.7f, 0.0f, 0.4f, 0.4f, std::chrono::milliseconds(1000), 0.5f));
    Room::objects.push_back(new StatusTrap(-0.7f, 0.0f, 0.4f, 0.4f, std::chrono::milliseconds(1000), 0.5f));
    Room::objects.push_back(new StatusTrap(0.0f, 0.7f, 0.4f, 0.4f, std::chrono::milliseconds(1000), 0.5f));
    Room::objects.push_back(new StatusTrap(0.0f, -0.7f, 0.4f, 0.4f, std::chrono::milliseconds(1000), 0.5f));
}

FourPillarRoom::FourPillarRoom() {
    Room::objects.push_back(new Wall(-0.5f, -0.5f, 0.3f, 0.3f));
    Room::objects.push_back(new Wall(0.5f, -0.5f, 0.3f, 0.3f));
    Room::objects.push_back(new Wall(-0.5f, 0.5f, 0.3f, 0.3f));
    Room::objects.push_back(new Wall(0.5f, 0.5f, 0.3f, 0.3f));
    Room::objects.push_back(new StatusTrap(0.6f, 0.0f, 0.4f, 0.4f, std::chrono::milliseconds(1000), 3.0f));
    Room::objects.push_back(new StatusTrap(-0.6f, 0.0f, 0.4f, 0.4f, std::chrono::milliseconds(1000), 3.0f));
    Room::objects.push_back(new StatusTrap(0.0f, 0.6f, 0.4f, 0.4f, std::chrono::milliseconds(1000), 3.0f));
    Room::objects.push_back(new StatusTrap(0.0f, -0.6f, 0.4f, 0.4f, std::chrono::milliseconds(1000), 3.0f));
}

TrapRoom::TrapRoom()
{
    float width = 0.7f;
    float height = 0.05f;
    Room::objects.push_back(new Wall(0.6f, 0.6f, width, height));
    Room::objects.push_back(new Wall(-0.6f, 0.6f, width, height));
    Room::objects.push_back(new Wall(0.6f, -0.6f, width, height));
    Room::objects.push_back(new Wall(-0.6f, -0.6f, width, height));
    Room::objects.push_back(new Wall(0.0f, 0.0f, width, height));

    Room::objects.push_back(new DartTrap(0.0f, 0.0f, height, width, std::chrono::milliseconds(1000), 0));
    Room::objects.push_back(new DartTrap(0.0f, 0.0f, height, width, std::chrono::milliseconds(1000), 180));
    Room::objects.push_back(new DartTrap(-1.0f, 0.8f, 0.1f, 0.3f, std::chrono::milliseconds(1000), 0));
    Room::objects.push_back(new DartTrap(1.0f, 0.8f, 0.1f, 0.3f, std::chrono::milliseconds(1000), 180));
    Room::objects.push_back(new DartTrap(-1.0f, -0.8f, 0.1f, 0.3f, std::chrono::milliseconds(1000), 0));
    Room::objects.push_back(new DartTrap(1.0f, -0.8f, 0.1f, 0.3f, std::chrono::milliseconds(1000), 180));
}

InvertedTrapRoom::InvertedTrapRoom()
{
    float width = 0.7f;
    float height = 0.05f;
    Room::objects.push_back(new Wall(0.6f, 0.6f, height, width));
    Room::objects.push_back(new Wall(-0.6f, 0.6f, height, width));
    Room::objects.push_back(new Wall(0.6f, -0.6f, height, width));
    Room::objects.push_back(new Wall(-0.6f, -0.6f, height, width));
    Room::objects.push_back(new Wall(0.0f, 0.0f, height, width));

    Room::objects.push_back(new DartTrap(0.0f, 0.0f, height, width, std::chrono::milliseconds(1000), 0));
    Room::objects.push_back(new DartTrap(0.0f, 0.0f, height, width, std::chrono::milliseconds(1000), 180));
    Room::objects.push_back(new DartTrap(-0.8f, 1.0f, 0.1f, 0.3f, std::chrono::milliseconds(1000), 270));
    Room::objects.push_back(new DartTrap(0.8f, 1.0f, 0.1f, 0.3f, std::chrono::milliseconds(1000), 270));
    Room::objects.push_back(new DartTrap(-0.8f, -1.0f, 0.1f, 0.3f, std::chrono::milliseconds(1000), 90));
    Room::objects.push_back(new DartTrap(0.8f, -1.0f, 0.1f, 0.3f, std::chrono::milliseconds(1000), 90));
}
