#pragma once
#include "Entity.h"

// FIRST TIER ENEMIES

class CrossEnemy : public Enemy {
public:
	CrossEnemy(float x, float y, float width, float height) : Enemy(x, y, width, height) {};
	void draw() override;
	void shoot() override; 

};

class turretEnemy : public Enemy {
public:
	turretEnemy(float x, float y, float width, float height) : Enemy(x, y, width, height) {};
};

class RunnerEnemy : public Enemy {
public:
	RunnerEnemy(float x, float y, float width, float height) : Enemy(x, y, width, height) {};
	void draw() override;
};

// SECOND TIER ENEMIES

class TwoBurstEnemy : public Enemy {
public:
	TwoBurstEnemy(float x, float y, float width, float height) : Enemy(x, y, width, height) {};
	void shoot() override;
	void draw() override;
};

class RoundShotEnemy : public Enemy {
public:
	RoundShotEnemy(float x, float y, float width, float height) : Enemy(x, y, width, height) {};
	void draw() override; 
	void shoot() override;
};

class TankEnemy : public Enemy {
public:
	TankEnemy(float x, float y, float width, float height) : Enemy(x, y, width, height) {};
	void draw() override;
	bool check(float dx, float dy, Entity* entity) override;
};

// MINIBOSS - BOSS TIER ENEMIES

class ShotgunEnemy : public Enemy {
public:
	ShotgunEnemy(float x, float y, float width, float height) : Enemy(x, y, width, height) {};
	void draw() override;
	void shoot() override;
};

class InvincibleEnemy : public Enemy {
public:
	InvincibleEnemy(float x, float y, float width, float height) : Enemy(x, y, width, height) {};
	void draw() override; 
};

class Boss : public Enemy {
	Boss(float x, float y, float width, float height) : Enemy(x, y, width, height) {};
};