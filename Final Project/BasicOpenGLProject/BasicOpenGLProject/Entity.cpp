#include "Entity.h"
#include<iostream>

//Entity::Entity(float x, float y, float size)
//{
//    Entity::x = x;
//    Entity::y = y;
//    Entity::size = size;
//}

void Entity::draw()
{
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(Entity::x - Entity::size / 2.0f, Entity::y - Entity::size / 2.0f); // Bottom-left
    glVertex2f(Entity::x + Entity::size / 2.0f, Entity::y - Entity::size / 2.0f); // Bottom-right
    glVertex2f(Entity::x + Entity::size / 2.0f, Entity::y + Entity::size / 2.0f); // Top-right
    glVertex2f(Entity::x - Entity::size / 2.0f, Entity::y + Entity::size / 2.0f); // Top-left
    glEnd();

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(Entity::x, -1.0f);
    glVertex2f(Entity::x, 1.0f);
    glEnd();
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex2f(-1.0f, Entity::y);
    glVertex2f(1.0f, Entity::y);
    glEnd();

}