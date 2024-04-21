#pragma once
#include <GL/freeglut.h>

using namespace std;

class Entity
{
protected:
	float size;
	float x;
	float y;
public:
	Entity(float x, float y, float size) : x(x), y(y), size(size) {};

	float getX() const { return Entity::x; };
	float getY() const { return Entity::y; };
	void setX(float value) { Entity::x = value; };
	void setY(float value) { Entity::y = value; };

	void draw();

};