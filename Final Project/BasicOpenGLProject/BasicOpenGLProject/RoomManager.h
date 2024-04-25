#pragma once
#include "Rooms.h"
#include "Entity.h"

class RoomManager {
public:
	static float score;
	static Room* currentRoom;
	static Player* player;

	RoomManager();
	void draw() { RoomManager::currentRoom->draw(); };
	//Use Check to finish game
	//bool check();
}; 
