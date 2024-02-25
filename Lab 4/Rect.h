#pragma once
#include "Vec.h"
#include <vector>

class Rect{
public:
    Vec v, u;
    Rect();
    Rect(float x, float y, float w, float h);
    bool contains(Vec w);
};