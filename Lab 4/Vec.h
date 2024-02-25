#pragma once
#include <iostream>

using namespace std;

class Vec{
public:
    float x;
    float y;
    Vec();
    Vec(float x, float y);
    void set(float x, float y);
    void add(Vec& u);
    void print();

};