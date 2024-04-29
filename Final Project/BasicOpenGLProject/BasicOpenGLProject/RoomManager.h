#pragma once
#include "Entity.h"
#include "Rooms.h"

class RoomManager {
public:
	static float score;
	static Room* currentRoom;
	static Player* player;
	static float playerMS;

	RoomManager();
	void draw() { RoomManager::currentRoom->draw(); };
	~RoomManager();
}; 
