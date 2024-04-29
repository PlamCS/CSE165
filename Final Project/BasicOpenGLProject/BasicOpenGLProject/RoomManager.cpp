#include "RoomManager.h"

RoomManager::RoomManager()
{
	RoomManager::score = 0;
	RoomManager::playerMS = 1.0f;
    RoomManager::currentRoom = new BeginningRoom();
	RoomManager::player = new Player(0.0f, 0.0f, 0.1f, 0.1f);
	player->setSpeed(RoomManager::playerMS);
}

RoomManager::~RoomManager()
{
	delete RoomManager::player;
	delete RoomManager::currentRoom;
}
