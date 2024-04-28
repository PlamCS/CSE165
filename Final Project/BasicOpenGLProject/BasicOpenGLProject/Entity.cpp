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

Enemy::Enemy(float x, float y, float width, float height) : Player(x, y, width, height) { 
    Enemy::setHealth(20);
    Enemy::changeImmunity(std::chrono::milliseconds(0)); 
}

bool Enemy::checkCollision(Enemy* enemy) {
    // Calculate the distances between the centers of the two enemies
    float dx = enemy->getX() - Enemy::getX();
    float dy = enemy->getY() - Enemy::getY();

    // Calculate the sum of the radii of the two enemies
    float combinedRadius = (Enemy::getWidth() + enemy->getWidth()) / 2.0f;

    // Check if the distance between the centers is less than the sum of the radii
    bool colliding = (dx * dx + dy * dy) < (combinedRadius * combinedRadius);

    return colliding;
}


bool Enemy::check(float dx, float dy, Entity* entity)
{
    bool overlapping = (dx - entity->getWidth() / 2.0f <= Entity::getX() + Entity::getWidth() / 2.0f &&
        dx + entity->getWidth() / 2.0f >= Entity::getX() - Entity::getWidth() / 2.0f &&
        dy - entity->getHeight() / 2.0f <= Entity::getY() + Entity::getHeight() / 2.0f &&
        dy + entity->getHeight() / 2.0f >= Entity::getY() - Entity::getHeight() / 2.0f);

    if ((dynamic_cast<const Projectile*>(entity)) && overlapping) return true;
    if (dynamic_cast<const Trap*>(entity)) return false;
    if (overlapping && (RoomManager::player == entity || dynamic_cast<const Wall*>(entity))) {
        if (RoomManager::player == entity) RoomManager::player->decreaseHealth();
        float overlapTop = getY() + getHeight() / 2.0f - dy + entity->getHeight() / 2.0f;
        float overlapBottom = dy + entity->getHeight() / 2.0f - getY() - getHeight() / 2.0f;
        float overlapRight = getX() + getWidth() / 2.0f - dx + entity->getWidth() / 2.0f;
        float overlapLeft = dx + entity->getWidth() / 2.0f - getX() - getWidth() / 2.0f;

        float minOverlap = std::min({ overlapTop, overlapBottom, overlapLeft, overlapRight });

        // Adjust the enemy's position based on the side of the collision
        if (minOverlap == overlapTop) {
            Enemy::move(0.0f, -overlapTop - 0.05f); // Move up
        }
        else if (minOverlap == overlapLeft) {
            Enemy::move(overlapLeft + 0.15f, 0.0f); // Move right
        }
        else if (minOverlap == overlapBottom) {
            Enemy::move(0.0f, overlapBottom + 0.15f); // Move down
        }
        else if (minOverlap == overlapRight) {
            Enemy::move(-overlapRight - 0.05f, 0.0f); // Move left
        }

        return true;
    }
    return false;
}

void Enemy::move(float dx, float dy)
{
    float newX = Enemy::getX() + (dx * Enemy::speed) / 3;
    float newY = Enemy::getY() + (dy * Enemy::speed) / 3;

    bool collisionX = false;
    bool collisionY = false;

    for (auto& enemy : RoomManager::currentRoom->getEnemies()) {
        if (enemy != this && Enemy::check(newX, Enemy::getY(), enemy)) {
            collisionX = true;
        }
        if (enemy != this && Enemy::check(Enemy::getX(), newY, enemy)) {
            collisionY = true;
        }
    }

    for (auto& enemy : RoomManager::currentRoom->getEnemies()) {
        if (enemy != this && Enemy::checkCollision(enemy)) {
            // Calculate the direction from this enemy to the other enemy
            float directionX = enemy->getX() - Enemy::getX();
            float directionY = enemy->getY() - Enemy::getY();

            // Normalize the direction
            float length = sqrt(directionX * directionX + directionY * directionY);
            directionX /= length;
            directionY /= length;

            // Move slightly away from the other enemy
            newX = Enemy::getX() - directionX * 0.05f;
            newY = Enemy::getY() - directionY * 0.05f;

            collisionX = false; // Reset collision flags to allow movement
            collisionY = false;
            break; // Exit loop since we only want to handle one collision
        }
    }


    if (!collisionX) {
        Enemy::setX(newX);
    }
    if (!collisionY) {
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
        double distance = std::sqrt(std::pow(Entity::getX() - enemy->getX(), 2) + std::pow(Entity::getY() - enemy->getY(), 2));
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

    float projectileSize = 0.05f;
    if (dynamic_cast<FinalBossRoom*>(RoomManager::currentRoom)) projectileSize = 0.025;
    Projectile* projectile = new Projectile(Entity::getX(), Entity::getY(), projectileSize, projectileSize, nearestEnemy);
    projectile->setSpeed(0.005f);
    projectile->setFriendly();
    RoomManager::currentRoom->getProjectiles().push_back(projectile);
}

void Item::draw() {
    // Set the number of segments for the circle
    int numSegments = 100;

    // Calculate radius based on width and height
    float radius = (width + height);

    // Set color to red
    glColor3f(0.75f, 0.75f, 0.75f);

    // Begin drawing triangle fan for the filled circle
    glBegin(GL_TRIANGLE_FAN);

    // Center of the circle
    glVertex2f(x, y);

    for (int i = 0; i <= numSegments; ++i) {
        // Calculate angle for each segment
        float theta = 2.0f * 3.1415926f * float(i) / float(numSegments);

        // Calculate x and y coordinates for the current segment
        float xCoord = x + radius * cosf(theta);
        float yCoord = y + radius * sinf(theta);

        // Set vertex
        glVertex2f(xCoord, yCoord);
    }

    // End drawing
    glEnd();

}

bool Item::check(float dx, float dy, Entity* object) {
    bool condition = (dx - object->getWidth() / 2.0f <= Entity::getX() + Entity::getWidth() / 2.0f &&
        dx + object->getWidth() / 2.0f >= Entity::getX() - Entity::getWidth() / 2.0f &&
        dy - object->getHeight() / 2.0f <= Entity::getY() + Entity::getHeight() / 2.0f &&
        dy + object->getHeight() / 2.0f >= Entity::getY() - Entity::getHeight() / 2.0f);

    if (condition) {
        RoomManager::score += 15;
        return true;
    }
    return false;
}

bool Beam::check(float dx, float dy, Entity* entity)
{


    return false;
}
