#pragma once
#include "Entity.h"

class Item : public Entity {
public:
	Item(float x, float y, float width, float height) : Entity(x, y, width, height) {};
	void draw() override;
	bool check(float dx, float dy, Entity* object) override;
};

class Heart : public Item {
public:
	Heart(float x, float y, float width, float height) : Item(x, y, width, height) {};
	void draw() override;
	bool check(float dx, float dy, Entity* object) override;
};

class SpeedBuff : public Item {
public:
	SpeedBuff(float x, float y, float width, float height) : Item(x, y, width, height) {};
	void draw() override;
	bool check(float dx, float dy, Entity* object) override;
};

class AntiInvincible : public Item {
public:
	AntiInvincible(float x, float y, float width, float height) : Item(x, y, width, height) {};
	void draw() override;
	bool check(float dx, float dy, Entity* object) override;
};

class BossHeart :
	public Item {
public:
	BossHeart(float x, float y, float width, float height) : Item(x, y, width, height) {};
	void draw() override;
	bool check(float dx, float dy, Entity* entity) override;
};