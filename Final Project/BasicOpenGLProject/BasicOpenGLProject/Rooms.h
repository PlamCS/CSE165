#pragma once
#include "Entity.h"
#include <vector>
#include <map>

class Room {
protected:
	std::vector<Entity*> enemies;
	std::vector<Entity*> objects;
	std::vector<Door*> doors;
	std::map<int, Room*> roomMemory;
public:
	Room();
	std::vector<Door*> getDoors() { return Room::doors; }
	std::vector<Entity*> getObjects() { return Room::objects; }
	
	int getOppositeDoor(Door* door);
	int getDoor(Door* door);

	void addRoomMemory(int door, Room* room);
	Room* getRoomMemory(int door);

	void draw();
	virtual bool check(float newX, float newY, Entity* Player);
};

class BeginningRoom :
	public Room 
{
public:
	BeginningRoom();
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