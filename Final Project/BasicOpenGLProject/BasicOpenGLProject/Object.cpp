#include "Object.h"
#include<iostream>

//Object::Object(float width, float height)
//{
//    Object::width = width;
//    Object::height = height;
//}

void Object::draw() {
    glColor3f(0.310f, 0.294f, 0.294f);
    glBegin(GL_QUADS);
    glVertex2f(Object::getX() - Object::width / 2.0f, Object::getY() - Object::height / 2.0f); // Bottom-left
    glVertex2f(Object::getX() + Object::width / 2.0f, Object::getY() - Object::height / 2.0f); // Bottom-right
    glVertex2f(Object::getX() + Object::width / 2.0f, Object::getY() + Object::height / 2.0f); // Top-right
    glVertex2f(Object::getX() - Object::width / 2.0f, Object::getY() + Object::height / 2.0f); // Top-left
    glEnd();
}