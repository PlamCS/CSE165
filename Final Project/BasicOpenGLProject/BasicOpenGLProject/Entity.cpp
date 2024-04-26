#include "RoomManager.h"
#include "Entity.h"
#include "Trap.h"
#include <iostream>
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

bool Entity::check(float dx, float dy, Entity* object)
{
    return (dx - object->getWidth() / 2.0f <= Entity::getX() + Entity::getWidth() / 2.0f &&
            dx + object->getWidth() / 2.0f >= Entity::getX() - Entity::getWidth() / 2.0f &&
            dy - object->getHeight() / 2.0f <= Entity::getY() + Entity::getHeight() / 2.0f &&
            dy + object->getHeight() / 2.0f >= Entity::getY() - Entity::getHeight() / 2.0f);
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

LWall::LWall(float x, float y, float width, float height, int rotation) : Entity(x, y, width, height)
{
    // Pivot Point is Top Right Default
    LWall::rotation = rotation;

    float posX = x + (width - height) / 2.0f;
    float posY = y + (width - height) / 2.0f;

    if (LWall::rotation == 90) {
        // Pivot Point is Bottom Right Default
        posX = x + (width - height) / 2.0f;
        posY = y - (width - height) / 2.0f;
    } else if (LWall::rotation == 180) {
        // Pivot Point is Bottom Left Default
        posX = x - (width - height) / 2.0f;
        posY = y - (width - height) / 2.0f;
    } else if (LWall::rotation == 270) {
        // Pivot Point is Top Left Default
        posX = x - (width - height) / 2.0f;
        posY = y + (width - height) / 2.0f;
    }

    LWall::Wall1 = new Wall(posX, y, height, width);
    LWall::Wall2 = new Wall(x, posY, width, height);
}

void LWall::draw() {
    glColor3f(0.329f, 0.329f, 0.329f);
    LWall::Wall1->draw();
    LWall::Wall2->draw();
}

bool LWall::check(float x, float y, Entity* object)
{
    return (LWall::Wall1->check(x, y, object) || LWall::Wall2->check(x, y, object));
}

bool Enemy::check(float pointX, float pointY, Entity* object)
{
    Enemy::counter++;
    if (Enemy::counter == 150) {
        Projectile* proj = new Projectile(Enemy::getX(), Enemy::getY(), 0.05f, 0.05f, 0.002f, object);
        proj->setSpeed(0.001f);
        std::cout << "Pushing Projectile" << std::endl;
        RoomManager::currentRoom->getProjectiles().push_back(proj);
        Enemy::counter = 0;
    }
    return false;
}

void Player::shoot()
{
    std::cout << "Shot" << std::endl;
    Projectile* projectile = new Projectile(Entity::getX(), Entity::getY(), 0.05f, 0.05f, 0.002f, RoomManager::currentRoom->getObjects()[2]);
    projectile->setFriendly();
    RoomManager::currentRoom->getProjectiles().push_back(projectile);
}
