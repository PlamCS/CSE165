#pragma once
#include <GL/freeglut.h>
#include <chrono>
#include <thread>
#include <vector>
#include <cmath>

class Entity
{
protected:
	float x, y, width, height;
public:
	Entity(float x, float y, float width, float height) : x(x), y(y), width(width), height(height) {};
	
	float getX() const { return Entity::x; };
	float getY() const { return Entity::y; };
	float getWidth() const { return Entity::width; };
	float getHeight() const { return Entity::height; };
	
	void setX(float value) { Entity::x = value; };
	void setY(float value) { Entity::y = value; };
	void setWidth(float value) { Entity::width = value; };
	void setHeight(float value) { Entity::height = value; };

	virtual void draw();
	virtual bool check(float pointX, float pointY, Entity* object);
};

class Door :
	public Entity
{
protected:
	int tier;
	bool visited;
public:
	Door(float x, float y, float width, float height, bool status) : Entity(x, y, width, height), visited(status), tier(0) {};
	
	void Visited() { Door::visited = true; };
	void Unvisited() { Door::visited = false; };
	bool isVisited() const { return Door::visited; };
	int Tier() { return Door::tier; };
	void setTier(int value) { Door::tier = value; };
};

class SpikeTrap : public Entity {
protected:
	bool activated = false;
	std::chrono::milliseconds activationDelay;
public:
	SpikeTrap(float x, float y, float width, float height, std::chrono::milliseconds delay) : Entity(x, y, width, height), activationDelay(delay), activated(false) {};
	bool check(float pointX, float pointY, Entity* object) override;
	void Activate();
	void draw() override;
};

class Wall : 
	public Entity{
public:
	Wall(float x, float y, float width, float height) : Entity(x, y, width, height) {};
};

class LWall : 
	public Entity {
protected:
	Entity* Wall1;
	Entity* Wall2;
	int rotation;
public:
	LWall(float x, float y, float width, float height, int rotation);
	void draw() override;
	bool check(float pointX, float pointY, Entity* object) override;
	~LWall();
};

class Player :
	public Entity
{
public:
	Player(float x, float y, float width, float height) : Entity(x, y, width, height) {}
};

class Projectile :
	public Entity
{
protected:
	float speed;
	float initial_dx;
	float initial_dy;
	
public:
	Projectile(float x, float y, float width, float height, float speed, Entity* target) : Entity(x, y, width, height), speed(speed){
		float dx = target->getX() - x;
		float dy = target->getY() - y;
		
		float magnitude = std::sqrt(dx * dx + dy * dy);

		if (magnitude != 0) { 
			initial_dx = dx / magnitude;
			initial_dy = dy / magnitude;
		}
		else {
			initial_dx = 0;
			initial_dy = 1;
		}
	
	}
	void move() {
		setX(getX() + speed * initial_dx);
		setY(getY() + speed * initial_dy);
	}
};

class Enemy : public Entity{
protected:
	std::vector<Projectile*> projectiles;
	int counter;
public:
	Enemy(float x, float y, float width, float height) : Entity(x, y, width, height), counter(0){}
	void draw() override;
	bool check(float pointX, float pointY, Entity* object) override;
	bool checkProjectile();
	//Projectile* shoot(const Player& player) {
	//	return new Projectile(getX(), getY(), 0.05f, 0.05f, 0.01f, player);
	//}
};