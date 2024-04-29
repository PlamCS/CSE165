#include "Entity.h"
#include "RoomManager.h"
#include <chrono>
#include <thread>
#include <random>

void Entity::draw()
{
    glBegin(GL_QUADS);
    glVertex2f(Entity::x - Entity::width / 2.0f, Entity::y - Entity::height / 2.0f); // Bottom-left
    glVertex2f(Entity::x + Entity::width / 2.0f, Entity::y - Entity::height / 2.0f); // Bottom-right
    glVertex2f(Entity::x + Entity::width / 2.0f, Entity::y + Entity::height / 2.0f); // Top-right
    glVertex2f(Entity::x - Entity::width / 2.0f, Entity::y + Entity::height / 2.0f); // Top-left
    glEnd();
}

bool Entity::check(float dx, float dy, Entity* entity)
{
    return (dx - entity->getWidth() / 2.0f <= Entity::getX() + Entity::getWidth() / 2.0f &&
            dx + entity->getWidth() / 2.0f >= Entity::getX() - Entity::getWidth() / 2.0f &&
            dy - entity->getHeight() / 2.0f <= Entity::getY() + Entity::getHeight() / 2.0f &&
            dy + entity->getHeight() / 2.0f >= Entity::getY() - Entity::getHeight() / 2.0f);
}

void Entity::move(float dx, float dy)
{
    float newX = Entity::getX() + dx * speed;
    float newY = Entity::getY() + dy * speed;

    bool collisionX = false;
    bool collisionY = false;

    if (!RoomManager::currentRoom->check(newX, Entity::getY(), this)) Entity::setX(newX);
    else collisionX = true;

    if (!RoomManager::currentRoom->check(Entity::getX(), newY, this)) Entity::setY(newY);
    else collisionY = true;

    if (collisionX && collisionY) {
        Entity::setX(Entity::getX());
        Entity::setY(Entity::getY());
    }
}

Projectile::Projectile(float x, float y, float width, float height, float dx, float dy) : Entity(x, y, width, height), enemy(true) {
    Projectile::setSpeed(speed);

    Projectile::initial_dx = dx;
    Projectile::initial_dy = dy;

    if (dx != 0.0f && dy != 0.0f) {
        Projectile::initial_dx *= 0.7071f; 
        Projectile::initial_dy *= 0.7071f;
    }
    
}

Projectile::Projectile(float x, float y, float width, float height, Entity* target) : Entity(x, y, width, height), enemy(true) {
    Projectile::setSpeed(speed);
    float dx = target->getX() - x;
    float dy = target->getY() - y;
    float magnitude = std::sqrt(dx * dx + dy * dy);
    if (magnitude != 0) {
        Projectile::initial_dx = dx / magnitude;
        Projectile::initial_dy = dy / magnitude;
    }
    else {
        Projectile::initial_dx = 0;
        Projectile::initial_dy = 1;
    }
}

void Projectile::move(float dx, float dy) {
    Projectile::setX(Projectile::getX() + dx);
    Projectile::setY(Projectile::getY() + dy);
}

