#pragma once
#include <GL/freeglut.h>

using namespace std;

class Entity
{
protected:
	float x, y, width, height;

public:
	Entity(float x, float y, float width, float height) : x(x), y(y), width(width), height(height){};

	float getX() const { return Entity::x; };
	float getY() const { return Entity::y; };
	float getWidth() const { return Entity::width; };
	float getHeight() const { return Entity::height; };
	/*float getR() const { return Entity::r; };
	float getG() const { return Entity::g; };
	float getB() const { return Entity::b; };*/

	void setX(float value) { Entity::x = value; };
	void setY(float value) { Entity::y = value; };
	void setWidth(float value) { Entity::width = width; };
	void setHeight(float value) { Entity::height = height; };
	/*void setR(float value) { Entity::r = value; };
	void setG(float value) { Entity::g = value; };
	void setB(float value) { Entity::b = value; };*/

	//void changeRGB(float r, float g, float b) { Entity::r = r; Entity::g = g; Entity::b = b; };

	void draw();

};