#pragma once
#include "Entity.h"
#include "Objects.h"
#include <vector>
#include <stack>

class Room {

protected:
	Entity* NorthWall = new Entity(0.0f, 1.0f, 2.0f, 0.2f);
	Entity* SouthWall = new Entity(0.0f, -1.0f, 2.0f, 0.2f);
	Entity* EastWall = new Entity(1.0f, 0.0f, 0.2f, 2.0f);
	Entity* WestWall = new Entity(-1.0f, 0.0f, 0.2f, 2.0f);

	Door* NorthDoor = new Door(0.0f, 0.98f, 0.2f, 0.2f, false);
	Door* SouthDoor = new Door(0.0f, -0.98f, 0.2f, 0.2f, false);
	Door* EastDoor = new Door(0.98f, 0.0f, 0.2f, 0.2f, false);
	Door* WestDoor = new Door(-0.98f, 0.0f, 0.2f, 0.2f, false);

	std::vector<Entity*> objects = { NorthWall, SouthWall, EastWall, WestWall };
	std::vector<Door*> doors = { NorthDoor, SouthDoor, EastDoor, WestDoor };

public:
	virtual void draw();
	bool check(float newX, float newY, Entity& Player);
};

class BeginningRoom : public Room {

public:
	BeginningRoom();
	void draw() override;

};