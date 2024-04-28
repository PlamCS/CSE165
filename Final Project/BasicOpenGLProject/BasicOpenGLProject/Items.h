#pragma once
#include "Entity.h"


class Heart : public Item {
public:
	Heart(float x, float y, float width, float height) : Item(x, y, width, height) {};
	void draw() override;
};
