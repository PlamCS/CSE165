#pragma once
#include <GL/freeglut.h>

using namespace std;

class Entity
{
public:
	float x;
	float y;
	void draw();
	void move(float x, float y);
	bool collidesWith(float otherX, float otherY);


};