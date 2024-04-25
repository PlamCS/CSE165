#include "RoomManager.h"
RoomManager::RoomManager()
{
	RoomManager::score = 0;
    RoomManager::currentRoom = new BeginningRoom();
	RoomManager::player = new Player(0.0f, 0.0f, 0.1f, 0.1f);
}
