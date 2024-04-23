#pragma once
#include <GL/freeglut.h>

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
	
	void setX(float value) { Entity::x = value; };
	void setY(float value) { Entity::y = value; };
	void setWidth(float value) { Entity::width = value; };
	void setHeight(float value) { Entity::height = value; };

	void draw();
};

class Door :
	public Entity
{
protected:
	bool visited;
public:
	Door(float x, float y, float width, float height, bool status) : Entity(x, y, width, height), visited(status) {}
	void Visited() { Door::visited = true; };
	void Unvisited() { Door::visited = false; };
	bool isVisited() const { return Door::visited; };
};

class Wall :
	public Entity
{
public:
	Wall(float x, float y, float width, float height) : Entity(x, y, width, height) {}
};

class Player :
	public Entity
{
public:
	Player(float x, float y, float width, float height) : Entity(x, y, width, height) {}
};