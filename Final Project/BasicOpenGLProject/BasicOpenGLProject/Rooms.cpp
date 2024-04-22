#include "Rooms.h"
#include <iostream>
#include <cstdlib>

void Room::draw()
{
	glColor3f(0.329f, 0.329f, 0.329f);
	for (auto& object : objects) {
		object->draw();
	}

	for (auto& door : doors) {
		if (door->getStatus()) door->draw();
	}
}

bool Room::check(float newX, float newY, Entity& Player)
{
    for (auto& object : objects) {
        if (newX - Player.getWidth() / 2.0f <= object->getX() + object->getWidth() / 2.0f &&
            newX + Player.getWidth() / 2.0f >= object->getX() - object->getWidth() / 2.0f &&
            newY - Player.getHeight() / 2.0f <= object->getY() + object->getHeight() / 2.0f &&
            newY + Player.getHeight() / 2.0f >= object->getY() - object->getHeight() / 2.0f) {
            return true;
        }
    }

    for (auto& door : doors) {
        if (door->getStatus() &&
            newX - Player.getWidth() / 2.0f <= door->getX() + door->getWidth() / 2.0f &&
            newX + Player.getWidth() / 2.0f >= door->getX() - door->getWidth() / 2.0f &&
            newY - Player.getHeight() / 2.0f <= door->getY() + door->getHeight() / 2.0f &&
            newY + Player.getHeight() / 2.0f >= door->getY() - door->getHeight() / 2.0f) {
            return true;
        }
    }

    return false;
}

BeginningRoom::BeginningRoom()
{
    Room::EastDoor->setStatus(true);
}

void BeginningRoom::draw()
{
    glColor3f(0.329f, 0.329f, 0.329f);
    for (auto& object : objects) {
        object->draw();
    }

    glColor3f(0.51f, 0.235f, 0.0f);
    for (auto& door : doors) {
        if (door->getStatus()) door->draw();
    }
}