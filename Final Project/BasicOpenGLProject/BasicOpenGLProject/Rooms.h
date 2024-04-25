#pragma once
#include "Entity.h"
#include <vector>
#include <map>

class Room {
protected:
	//Entity* NorthWall = new Entity(0.0f, 1.0f, 2.0f, 0.2f);
	//Entity* SouthWall = new Entity(0.0f, -1.0f, 2.0f, 0.2f);
	//Entity* EastWall = new Entity(1.0f, 0.0f, 0.2f, 2.0f);
	//Entity* WestWall = new Entity(-1.0f, 0.0f, 0.2f, 2.0f);
	std::vector<Entity*> objects = {new Wall(0.0f, 1.05f, 2.0f, 0.2f),
									new Wall(0.0f, -1.05f, 2.0f, 0.2f),
									new Wall(1.05f, 0.0f, 0.2f, 2.0f),
									new Wall(-1.05f, 0.0f, 0.2f, 2.0f) };
	//Door* NorthDoor = new Door(0.0f, 0.98f, 0.2f, 0.2f, false);
	//Door* SouthDoor = new Door(0.0f, -0.98f, 0.2f, 0.2f, false);
	//Door* EastDoor = new Door(0.98f, 0.0f, 0.2f, 0.2f, false);
	//Door* WestDoor = new Door(-0.98f, 0.0f, 0.2f, 0.2f, false);
	std::vector<Door*> doors = {new Door(0.0f, 1.03f, 0.2f, 0.2f, false),
								new Door(0.0f, -1.03f, 0.2f, 0.2f, false),
								new Door(1.03f, 0.0f, 0.2f, 0.2f, false),
								new Door(-1.03f, 0.0f, 0.2f, 0.2f, false) };
	std::map<int, Room*> roomMemory;

public:
	std::vector<Door*> getDoors() { return Room::doors; };
	int getOppositeDoor(Door* door);
	int getDoor(Door* door);

	void addRoomMemory(int door, Room* room);
	Room* getRoomMemory(int door);

	void draw();
	bool check(float newX, float newY, Entity& Player);

	virtual ~Room();
};

class LWallRoom :
	public Room 
{
public:
	LWallRoom();
};

class InvertedLWallRoom :
	public Room 
{
public:
	InvertedLWallRoom();

};

class WallRoom :
	public Room
{
public:
	WallRoom();
};

class InvertWallRoom :
	public Room
{
public:
	InvertWallRoom();
};