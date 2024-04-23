#pragma once
#include "Rooms.h"

class RoomManager {
public:
	static Room* currentRoom;
	RoomManager();
	void draw() { RoomManager::currentRoom->draw(); };
	bool check(float x, float y, Entity& Player) { return RoomManager::currentRoom->check(x, y, Player); };
}; 
