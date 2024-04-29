#pragma once
#include "Entity.h"
#include "Items.h"
// FIRST TIER ENEMIES
class Player :
	public Entity
{
protected:
	std::chrono::milliseconds cooldown;
	std::chrono::milliseconds immunityframes;
	std::chrono::steady_clock::time_point lastHitTime;
	bool isOnCooldown;
	std::string item;
	int health;
	bool winner;
public:
	Player(float x, float y, float width, float height) :
		Entity(x, y, width, height),
		lastHitTime(std::chrono::steady_clock::now()),
		cooldown(std::chrono::milliseconds(1000)),
		immunityframes(std::chrono::milliseconds(1500)),
		isOnCooldown(false), health(100), item("Item"), winner(false) {};

	void setWinCondition() { Player::winner = true; };
	bool isWinner() const { return Player::winner; };

	void giveItem(std::string item) { Player::item = item; };
	Entity* dropItem();

	int getHealth() const { return Player::health; };
	void increaseHealth() { Player::health += 10; };
	void decreaseHealth() {
		if (!isImmune()) {
			health -= 10;
			lastHitTime = std::chrono::steady_clock::now();
		}
	};
	void setHealth(int value) { Player::health = value; };

	void changeCooldown(std::chrono::milliseconds cooldown) { Player::cooldown = cooldown; };
	void changeImmunity(std::chrono::milliseconds time) { Player::immunityframes = time; };

	bool isReady() const { return Player::isOnCooldown; };
	bool isImmune() const {
		auto now = std::chrono::steady_clock::now();
		return std::chrono::duration_cast<std::chrono::milliseconds>(now - Player::lastHitTime) < Player::immunityframes;
	}

	virtual void shoot();
};

class Enemy : public Player {
public:
	Enemy(float x, float y, float width, float height);
	bool checkCollision(Enemy* enemy);
	bool check(float dx, float dy, Entity* entity) override;
	void move(float dx, float dy) override;
	void shoot() override;
};

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
	int beamRate;
public:
	Boss(float x, float y, float width, float height) : Enemy(x, y, width, height), angle(0.0f), beamCooldown(10), beamRate(10){
		setSpeed(0.0f);
		setHealth(600);
		Boss::giveItem("Boss Heart");
	};
	void draw() override;
	void shoot() override;
};