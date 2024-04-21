#pragma once
#include<GL/freeglut.h>
#include "Entity.h"

class Object: public Entity
{
	float width;
	float height;
public:
	Object(float x, float y, float size, float width, float height) : Entity(x, y, size), width(width), height(height) {};
	
	float getWidth() const { return Object::width; };
	float getHeight() const { return Object::height; };
	
	void draw();

};