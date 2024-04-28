#pragma once
#include "Entity.h"


class Heart : public Item {
public:
	Heart(float x, float y, float width, float height) : Item(x, y, width, height) {};
	void draw() override;
	bool check(float dx, float dy, Entity* object) override;
};

class SpeedBuff : public Item {
private:
	static const int maxIncreases = 5;
public:
	SpeedBuff(float x, float y, float width, float height) : Item(x, y, width, height) {};
	void draw() override;
	bool check(float dx, float dy, Entity* object) override;
};

class AntiInvincible : public Item {
private:
	static const int maxIncreases = 5;
public:
	AntiInvincible(float x, float y, float width, float height) : Item(x, y, width, height) {};
	void draw() override;
	bool check(float dx, float dy, Entity* object) override;
};
