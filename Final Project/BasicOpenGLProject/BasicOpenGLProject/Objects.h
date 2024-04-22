#pragma once
#include<GL/freeglut.h>
#include "Entity.h"
#include <iostream>

class Door : public Entity {
protected:
	bool active;
public:
	Door(float x, float y, float width, float height, bool active) : Entity(x, y, width, height), active(active) {};
	void setStatus(bool status) { Door::active = status; };
	bool getStatus() const { return Door::active; };
};