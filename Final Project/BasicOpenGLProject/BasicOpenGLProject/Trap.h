#pragma once
#include "Entity.h"
#include <chrono>

class Trap :
	public Entity {
protected:
	bool activated;
	std::chrono::milliseconds activationDelay;
public:
	Trap(float x, float y, float width, float height, std::chrono::milliseconds delay) : Entity(x, y, width, height), activationDelay(delay), activated(false) {}
	virtual void Activate() = 0;
};

class StatusTrap :
	public Trap {
protected:
	float statusSpeed;
public:
	StatusTrap(float x, float y, float width, float height, std::chrono::milliseconds delay, float speed) : Trap(x, y, width, height, delay), statusSpeed(speed) {};
	void Activate();
	void ApplyEffect(Entity* entity);
	bool check(float pointX, float pointY, Entity* object) override;
	void draw() override;
};

class SpikeTrap :
	public Trap {
public:
	SpikeTrap(float x, float y, float width, float height, std::chrono::milliseconds delay) : Trap(x, y, width, height, delay) {};
	bool check(float pointX, float pointY, Entity* object) override;
	void Activate();
	void draw() override;
};

class DartTrap :
	public Trap {
	int direction;
public:
	DartTrap(float x, float y, float width, float height, std::chrono::milliseconds delay, int direction) : Trap(x, y, width, height, delay), direction(direction){};
	void Activate();
	bool check(float pointX, float pointY, Entity* object) override;
	void draw() override;
};