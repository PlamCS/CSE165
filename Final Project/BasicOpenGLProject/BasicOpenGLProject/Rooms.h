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
	std::vector<Wall> objects = {Wall(0.0f, 1.0f, 2.0f, 0.2f),
									Wall(0.0f, -1.0f, 2.0f, 0.2f),
									Wall(1.0f, 0.0f, 0.2f, 2.0f),
									Wall(-1.0f, 0.0f, 0.2f, 2.0f) };
	//Door* NorthDoor = new Door(0.0f, 0.98f, 0.2f, 0.2f, false);
	//Door* SouthDoor = new Door(0.0f, -0.98f, 0.2f, 0.2f, false);
	//Door* EastDoor = new Door(0.98f, 0.0f, 0.2f, 0.2f, false);
	//Door* WestDoor = new Door(-0.98f, 0.0f, 0.2f, 0.2f, false);
	std::vector<Door> doors = {Door(0.0f, 0.98f, 0.2f, 0.2f, false),
								Door(0.0f, -0.98f, 0.2f, 0.2f, false),
								Door(0.98f, 0.0f, 0.2f, 0.2f, false),
								Door(-0.98f, 0.0f, 0.2f, 0.2f, false) };
	std::map<Door*, Room*> roomMemory;

public:
	std::vector<Door> getDoors() { return Room::doors; };

	void addRoomMemory(Door* door, Room* room);
	Room* getRoomMemory(Door* door);

	virtual void draw();
	virtual bool check(float newX, float newY, Entity& Player);
};

