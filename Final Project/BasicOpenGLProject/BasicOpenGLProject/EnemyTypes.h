#pragma once
#include "Entity.h"

// FIRST TIER ENEMIES

class CrossEnemy : public Enemy {
public:
	CrossEnemy(float x, float y, float width, float height) : Enemy(x, y, width, height) {
		setSpeed(0.0f);
		setHealth(20);
	};
	void draw() override;
	void shoot() override; 

};

class turretEnemy : public Enemy {
public:
	turretEnemy(float x, float y, float width, float height) : Enemy(x, y, width, height) {};
};

class RunnerEnemy : public Enemy {
public:
	RunnerEnemy(float x, float y, float width, float height) : Enemy(x, y, width, height) {
		setSpeed(1.50f);
		setHealth(20);
	};
	void draw() override;
};

// SECOND TIER ENEMIES

class TwoBurstEnemy : public Enemy {
public:
	TwoBurstEnemy(float x, float y, float width, float height) : Enemy(x, y, width, height) {
		setSpeed(1.0f);
		setHealth(30);
	};
	void shoot() override;
	void draw() override;
};

class RoundShotEnemy : public Enemy {
public:
	RoundShotEnemy(float x, float y, float width, float height) : Enemy(x, y, width, height) {\
		setSpeed(0.0f);
		setHealth(40);
	};
	void draw() override; 
	void shoot() override;
};

class TankEnemy : public Enemy {
public:
	TankEnemy(float x, float y, float width, float height) : Enemy(x, y, width, height) {
		setSpeed(0.35f);
		setHealth(40);
	};
	void draw() override;
	bool check(float dx, float dy, Entity* entity) override;
};

// MINIBOSS - BOSS TIER ENEMIES

class ShotgunEnemy : public Enemy {
public:
	ShotgunEnemy(float x, float y, float width, float height) : Enemy(x, y, width, height) {
		setSpeed(1.0f);
		setHealth(60);
	};
	void draw() override;
	void shoot() override;
};

class InvincibleEnemy : public Enemy {
public:
	InvincibleEnemy(float x, float y, float width, float height) : Enemy(x, y, width, height) {
		setHealth(100);
	};
	void draw() override;
	bool check(float dx, float dy, Entity* entity) override;
	void shoot() override;
};

class Boss : public Enemy {
protected:
	float angle;
	int beamCooldown;
	float colorRed;
public:
	Boss(float x, float y, float width, float height) : Enemy(x, y, width, height), angle(0.0f), beamCooldown(0), colorRed(0.0){
		setSpeed(0.0f);
		setHealth(600);
	};
	void draw() override;
	void shoot() override;
	void phase1();
	void phase2();
	void phase3();
};