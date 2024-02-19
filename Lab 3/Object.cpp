#include <iostream>
#include "Object.h"

using namespace std;

Object::Object(){
    Object::x = 0;
    Object::count++;
}

Object::Object(int val){
    Object::x = val;
    Object::count++;
}

Object::Object(Object &old){
    Object::x = old.x;
    Object::count++;
}

Object& Object::operator=(Object &old){
    Object::x = old.x;
    return *this;
}