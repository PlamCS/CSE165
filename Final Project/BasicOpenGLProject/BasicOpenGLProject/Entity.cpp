#include "RoomManager.h"
#include "Entity.h"
#include "Trap.h"
#include <iostream>
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

void Enemy::shoot()
{
    isOnCooldown = true;
    std::thread cooldownThread([this]() {
        std::this_thread::sleep_for(cooldown);
        isOnCooldown = false;
        });
    cooldownThread.detach();
    Projectile* proj = new Projectile(Enemy::getX(), Enemy::getY(), 0.05f, 0.05f, 0.002f, RoomManager::player);
    proj->setSpeed(0.001f);
    std::cout << "Pushing Projectile" << std::endl;
    RoomManager::currentRoom->getProjectiles().push_back(proj);
}

void Player::shoot()
{
    if (isOnCooldown) {
        return; // Exit the function early if shooting is on cooldown
    }

    // Set the cooldown flag to true
    isOnCooldown = true;

    Entity* nearestEnemy = nullptr;
    float minDistance = std::numeric_limits<float>::max(); // Initialize minDistance to a large value

    for (auto& enemy : RoomManager::currentRoom->getEnemies()) {
        // Calculate distance between player and enemy using Euclidean distance formula
        float distance = std::sqrt(std::pow(Entity::getX() - enemy->getX(), 2) + std::pow(Entity::getY() - enemy->getY(), 2));

        // Check if the current enemy is closer than the previous closest enemy
        if (distance < minDistance) {
            minDistance = distance;
            nearestEnemy = enemy;
        }
    }

    // If no enemy is found, exit the function
    if (!nearestEnemy) {
        return;
    }

    // Create a thread to delay the next shot
    std::thread cooldownThread([this]() {
        std::this_thread::sleep_for(Player::cooldown);

        // Reset the cooldown flag after the cooldown period is over
        isOnCooldown = false;
        });
    cooldownThread.detach();

    Projectile* projectile = new Projectile(Entity::getX(), Entity::getY(), 0.05f, 0.05f, 0.005f, nearestEnemy);
    projectile->setFriendly();
    RoomManager::currentRoom->getProjectiles().push_back(projectile);
}
