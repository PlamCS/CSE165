#pragma once
#include <GL/freeglut.h>
#include <thread>
#include <vector>
#include <cmath>

class Entity
{
protected:
	float x, y, width, height, speed;
public:
	Entity(float x, float y, float width, float height) : x(x), y(y), width(width), height(height), speed(0) {};

	float getSpeed() const { return Entity::speed; };
	float getX() const { return Entity::x; };
	float getY() const { return Entity::y; };
	float getWidth() const { return Entity::width; };
	float getHeight() const { return Entity::height; };
	
	void setSpeed(float value) { Entity::speed = value; };
	void setX(float value) { Entity::x = value; };
	void setY(float value) { Entity::y = value; };
	void setWidth(float value) { Entity::width = value; };
	void setHeight(float value) { Entity::height = value; };

	virtual void draw();
	virtual bool check(float dx, float dy, Entity* object);
	virtual void move(float dx, float dy);
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
	int Tier() const { return Door::tier; };
	void setTier(int value) { Door::tier = value; };
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
};

class Player :
	public Entity
{
protected:
	float speed;
	std::chrono::milliseconds cooldown;
	bool isOnCooldown;
public:
	Player(float x, float y, float width, float height, float speed) : 
		Entity(x, y, width, height), speed(speed), 
		cooldown(std::chrono::milliseconds(2000)), isOnCooldown(false) {}
	bool isReady() const { return Player::isOnCooldown; };
	void shoot();
};

class Projectile :
	public Entity
{
protected:
	float projectileSpeed;
	float initial_dx;
	float initial_dy;
	bool enemy;
	bool marked;
public:
	Projectile(float x, float y, float width, float height, float speed, Entity* target) : Entity(x, y, width, height) , projectileSpeed(speed), enemy(true), marked(false){
		float dx = target->getX() - x;
		float dy = target->getY() - y;

		float magnitude = std::sqrt(dx * dx + dy * dy);

		if (magnitude != 0) {
			Projectile::initial_dx = dx / magnitude;
			Projectile::initial_dy = dy / magnitude;
		}
		else {
			Projectile::initial_dx = 0;
			Projectile::initial_dy = 1;
		}
	}

	bool Friendly() const { return Projectile::enemy; };
	
	void setFriendly() { Projectile::enemy = false; };

	void move() {
		Projectile::setX(Projectile::getX() + Projectile::projectileSpeed * Projectile::initial_dx);
		Projectile::setY(Projectile::getY() + Projectile::projectileSpeed * Projectile::initial_dy);
	}
};

class Enemy : public Player {
public:
	Enemy(float x, float y, float width, float height) : Player(x, y, width, height, 0.02f) {};
	void shoot();
};