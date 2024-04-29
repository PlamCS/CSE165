#pragma once
#include "EnemyTypes.h"
#include "Entity.h"
#include <vector>
#include <map>

class Room {
protected:
	std::vector<Enemy*> enemies;
	std::vector<Entity*> objects;
	std::vector<Door*> doors;
	std::vector<Projectile*> projectiles;
	std::map<int, Room*> roomMemory;
public:
	Room();
	std::vector<Door*>& getDoors() { return Room::doors; }
	std::vector<Entity*>& getObjects() { return Room::objects; }
	std::vector<Projectile*>& getProjectiles() { return Room::projectiles; }
	std::vector<Enemy*>& getEnemies() { return Room::enemies; }
	
	int getOppositeDoor(Door* door);
	int getDoor(Door* door);

	void addRoomMemory(int door, Room* room);
	Room* getRoomMemory(int door);
	void removeRoomMemory(Room* room);

	void draw();
	virtual bool check(float newX, float newY, Entity* Player);

	virtual ~Room();
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

class CenterPillarRoom :
	public Room
{
public:
	CenterPillarRoom();
};

class FourPillarRoom :
	public Room
{
public:
	FourPillarRoom();
};

class TrapRoom:
	public Room
{
public:
	TrapRoom();
};

class InvertedTrapRoom :
	public Room
{
public:
	InvertedTrapRoom();
};

class ShotgunRoom :
	public Room {
public:
	ShotgunRoom();
};

class InvincibleRoom :
	public Room {
public:
	InvincibleRoom();
};

class FinalBossRoom :
	public Room {
public:
	FinalBossRoom();
};