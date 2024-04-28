#pragma once
#include <GL/freeglut.h>
#include <thread>
#include <vector>
#include <cmath>
#include <iostream>

class Entity
{
protected:
	bool marked;
	float x, y, width, height, speed;
public:
	Entity(float x, float y, float width, float height) : x(x), y(y), width(width), height(height), speed(0), marked(false){};

	float getSpeed() const { return Entity::speed; };
	float getX() const { return Entity::x; };
	float getY() const { return Entity::y; };
	float getWidth() const { return Entity::width; };
	float getHeight() const { return Entity::height; };
	bool isMarked() const { return Entity::marked; };

	void setSpeed(float value) { Entity::speed = value; };
	void setX(float value) { Entity::x = value; };
	void setY(float value) { Entity::y = value; };
	void setWidth(float value) { Entity::width = value; };
	void setHeight(float value) { Entity::height = value; };
	void Mark() { Entity::marked = true; };

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
	~LWall();
};

class Projectile :
	public Entity
{
protected:
	float initial_dx;
	float initial_dy;
	bool enemy;
public:
	Projectile(float x, float y, float width, float height, float dx, float dy);
	Projectile(float x, float y, float width, float height, Entity* target);

	float getDx() const { return Projectile::initial_dx; };
	float getDy() const { return Projectile::initial_dy; };

	bool Friendly() const { return Projectile::enemy; };
	void setFriendly() { Projectile::enemy = false; };

	void move(float dx, float dy) override;
};

class Player :
	public Entity
{
protected:
	std::chrono::milliseconds cooldown;
	std::chrono::milliseconds immunityframes;
	std::chrono::steady_clock::time_point lastHitTime;
	bool isOnCooldown;

	int health;
public:
	Player(float x, float y, float width, float height) :
		Entity(x, y, width, height), 
		lastHitTime(std::chrono::steady_clock::now()),
		cooldown(std::chrono::milliseconds(2000)), 
		immunityframes(std::chrono::milliseconds(1500)),
		isOnCooldown(false), health(50) {}

	int getHealth() const { return Player::health; };
	void increaseHealth() { Player::health += 10; };
	void decreaseHealth() {
		if (!isImmune()) {
			health -= 10;
			lastHitTime = std::chrono::steady_clock::now();
		}
	};
	void changeCooldown(std::chrono::milliseconds cooldown) { Player::cooldown = cooldown; };
	void changeImmunity(std::chrono::milliseconds time) { Player::immunityframes = time; };

	bool isReady() const { return Player::isOnCooldown; };
	bool isImmune() const {
		std::cout << "is Immune Currently" << std::endl;
		auto now = std::chrono::steady_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(now - Player::lastHitTime) < Player::immunityframes;
	}

	virtual void shoot();
};

class Enemy : public Player {
public:
	Enemy(float x, float y, float width, float height) : Player(x, y, width, height) { Enemy::changeImmunity(std::chrono::milliseconds(0)); };
	bool check(float dx, float dy, Entity* entity) override;
	void move(float dx, float dy) override;
	void shoot() override;
};