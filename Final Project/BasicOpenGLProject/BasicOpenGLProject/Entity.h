#pragma once
#include <GL/freeglut.h>

using namespace std;

class Entity
{
	float size;
public:
	float x;
	float y;
	Entity(float x, float y, float size);
	
	float getX();
	float getY();
	void setX(float value);
	void setY(float value);

	void draw();

};