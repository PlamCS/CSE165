#include "Entity.h"

void Entity::draw()
{
    glPointSize(100.0f);
    glColor3f(0.0f, 1.0f, 0.0f); // Green color
    glBegin(GL_POINTS);
    glVertex2f(Entity::x, Entity::y);
    glEnd();
}

void Entity::move(float x, float y)
{
    Entity::x += x;
    Entity::y += y;
}

bool Entity::collidesWith(float otherX, float otherY){
    return (otherX >= Entity::x - 1.0f && otherX <= Entity::x + 1.0f &&
        otherY >= Entity::y - 1.0f && otherY <= Entity::y + 1.0f);
}