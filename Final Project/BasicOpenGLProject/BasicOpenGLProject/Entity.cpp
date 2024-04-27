#include "RoomManager.h"
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

void Enemy::shoot()
{
    isOnCooldown = true;
    std::thread cooldownThread([this]() {
        std::this_thread::sleep_for(cooldown);
        isOnCooldown = false;
        });
    cooldownThread.detach();
    Projectile* proj = new Projectile(Enemy::getX(), Enemy::getY(), 0.05f, 0.05f, RoomManager::player);
    proj->setSpeed(0.003f);
    RoomManager::currentRoom->getProjectiles().push_back(proj);
}

bool Enemy::check(float dx, float dy, Entity* entity)
{
    bool overlapping = (dx - entity->getWidth() / 2.0f <= Entity::getX() + Entity::getWidth() / 2.0f &&
                        dx + entity->getWidth() / 2.0f >= Entity::getX() - Entity::getWidth() / 2.0f &&
                        dy - entity->getHeight() / 2.0f <= Entity::getY() + Entity::getHeight() / 2.0f &&
                        dy + entity->getHeight() / 2.0f >= Entity::getY() - Entity::getHeight() / 2.0f);
    
    if (dynamic_cast<const Projectile*>(entity) && overlapping) return true;
    if (overlapping && dynamic_cast<const Wall*>(entity) || dynamic_cast<const LWall*>(entity)) {
        float overlapTop = getY() + getHeight() / 2.0f - dy + entity->getHeight() / 2.0f;
        float overlapBottom = dy + entity->getHeight() / 2.0f - getY() - getHeight() / 2.0f;
        float overlapRight = getX() + getWidth() / 2.0f - dx + entity->getWidth() / 2.0f;
        float overlapLeft = dx + entity->getWidth() / 2.0f - getX() - getWidth() / 2.0f;

        float minOverlap = std::min({ overlapTop, overlapBottom, overlapLeft, overlapRight });

        // Adjust the enemy's position based on the side of the collision
        if (minOverlap == overlapTop) {
            std::cout << "Top" << std::endl;
            Enemy::move(0.0f, -overlapTop - 0.05f); // Move up
        }
        else if (minOverlap == overlapLeft) {
            std::cout << "Left" << std::endl;
            Enemy::move(overlapLeft + 0.15f, 0.0f); // Move right
        }
        else if (minOverlap == overlapBottom) {
            std::cout << "Bottom" << std::endl;
            Enemy::move(0.0f, overlapBottom + 0.15f); // Move down
        }
        else if (minOverlap == overlapRight) {
            std::cout << "Right" << std::endl;
            Enemy::move(-overlapRight - 0.05f, 0.0f); // Move left
        }

        return true;
    }
    else if (overlapping && RoomManager::player == entity) {
        std::cout << "Player Health Decrease" << std::endl;

        return true;
    }
    
    return false;
}

void Enemy::move(float dx, float dy)
{
    float newX = Enemy::getX() + dx * Enemy::speed;
    float newY = Enemy::getY() + dy * Enemy::speed;

    bool collisionX = false;
    bool collisionY = false;

    if (!check(newX, Enemy::getY(), this)) Enemy::setX(newX);
    else collisionX = true;
    if (!check(Enemy::getX(), newY, this)) Enemy::setY(newY);
    else collisionY = true;

    if (collisionX && collisionY) {
        Enemy::setX(newX);
        Enemy::setY(newY);
    }
}

void Player::shoot()
{
    if (isOnCooldown) return;
    isOnCooldown = true;

    Entity* nearestEnemy = nullptr;
    float minDistance = std::numeric_limits<float>::max();

    for (auto& enemy : RoomManager::currentRoom->getEnemies()) {
        float distance = std::sqrt(std::pow(Entity::getX() - enemy->getX(), 2) + std::pow(Entity::getY() - enemy->getY(), 2));
        if (distance < minDistance) {
            minDistance = distance;
            nearestEnemy = enemy;
        }
    }
    if (!nearestEnemy) return;
    std::thread cooldownThread([this]() {
        std::this_thread::sleep_for(Player::cooldown);
        isOnCooldown = false;
        });
    cooldownThread.detach();

    Projectile* projectile = new Projectile(Entity::getX(), Entity::getY(), 0.05f, 0.05f, nearestEnemy);
    projectile->setSpeed(0.005f);
    projectile->setFriendly();
    RoomManager::currentRoom->getProjectiles().push_back(projectile);
}
