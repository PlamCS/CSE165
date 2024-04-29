#include "EnemyTypes.h"
#include "Trap.h"
#include "RoomManager.h"
#include "Items.h"
#include <random>
#include <iostream>

// FIRST TIER ENEMIES

Entity* Player::dropItem()
{
    if (Player::item == "Item") {
        return new Item(Player::getX(), Player::getY(), 0.01f, 0.01f);
    }
    else if (Player::item == "Heart") {
        return new Heart(Player::getX(), Player::getY(), 0.01f, 0.01f);
    }
    else if (Player::item == "Boss Heart") {
        return new BossHeart(Player::getX(), Player::getY(), 0.15f, 0.15f);
    }
    return nullptr;
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
    Enemy::setSpeed(1.0f);
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

// CROSS ENEMY
void CrossEnemy::draw() {
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(Entity::x - Entity::width / 2.0f, Entity::y + Entity::height / 2.0f); // Top-left
    glVertex2f(Entity::x + Entity::width / 2.0f, Entity::y + Entity::height / 2.0f); // Top-right
    glVertex2f(Entity::x + Entity::width / 2.0f, Entity::y - Entity::height / 2.0f); // Bottom-right
    glVertex2f(Entity::x - Entity::width / 2.0f, Entity::y - Entity::height / 2.0f); // Bottom-left
    glEnd();

    // Draw the cross
    glBegin(GL_LINES);
    // Horizontal line
    glVertex2f(Entity::x - Entity::width, Entity::y);
    glVertex2f(Entity::x + Entity::width, Entity::y);
    // Vertical line
    glVertex2f(Entity::x, Entity::y - Entity::height);
    glVertex2f(Entity::x, Entity::y + Entity::height);
    glEnd();
}

void CrossEnemy::shoot() 
{
    isOnCooldown = true;
    std::thread cooldownThread([this]() {
        std::this_thread::sleep_for(cooldown);
        isOnCooldown = false;
        });
    cooldownThread.detach();
    Projectile* proj = new Projectile(CrossEnemy::getX(), CrossEnemy::getY(), 0.05f, 0.05f, 0.0f, 1.0f);
    proj->setSpeed(0.003f);
    RoomManager::currentRoom->getProjectiles().push_back(proj);
    proj = new Projectile(CrossEnemy::getX(), CrossEnemy::getY(), 0.05f, 0.05f, 0.0f, -1.0f);
    proj->setSpeed(0.003f);
    RoomManager::currentRoom->getProjectiles().push_back(proj);
    proj = new Projectile(CrossEnemy::getX(), CrossEnemy::getY(), 0.05f, 0.05f, -1.0f, 0.0f);
    proj->setSpeed(0.003f);
    RoomManager::currentRoom->getProjectiles().push_back(proj);
    proj = new Projectile(CrossEnemy::getX(), CrossEnemy::getY(), 0.05f, 0.05f, 1.0f, 0.0f);
    proj->setSpeed(0.003f);
    RoomManager::currentRoom->getProjectiles().push_back(proj);
    proj = nullptr;
    delete proj;
}

 // RUNNER ENEMY
void RunnerEnemy::draw() {
    glColor3f(0.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(Entity::x - Entity::width / 2.0f, Entity::y - Entity::height / 2.0f); // Bottom-left
    glVertex2f(Entity::x + Entity::width / 2.0f, Entity::y - Entity::height / 2.0f); // Bottom-right
    glVertex2f(Entity::x + Entity::width / 2.0f, Entity::y + Entity::height / 2.0f); // Top-right
    glVertex2f(Entity::x - Entity::width / 2.0f, Entity::y + Entity::height / 2.0f); // Top-left
    glEnd();
}

// TIER TWO ENEMIES

// TWO-BURST ENEMY
void TwoBurstEnemy::draw() {
    glColor3f(1.0f, 0.7f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(Entity::x - Entity::width / 2.0f, Entity::y + Entity::height / 2.0f); // Top-left
    glVertex2f(Entity::x + Entity::width / 2.0f, Entity::y + Entity::height / 2.0f); // Top-right
    glVertex2f(Entity::x + Entity::width / 2.0f, Entity::y - Entity::height / 2.0f); // Bottom-right
    glVertex2f(Entity::x - Entity::width / 2.0f, Entity::y - Entity::height / 2.0f); // Bottom-left
    glEnd();
}

void TwoBurstEnemy::shoot() {
    std::thread cooldownThread([this]() {
        std::this_thread::sleep_for(cooldown);
        isOnCooldown = false;
        });
    cooldownThread.detach();
    Projectile* proj = new Projectile(TwoBurstEnemy::getX(), TwoBurstEnemy::getY(), 0.05f, 0.05f,  RoomManager::player);
    proj->setSpeed(0.002f);
    RoomManager::currentRoom->getProjectiles().push_back(proj);
    proj = new Projectile(TwoBurstEnemy::getX(), TwoBurstEnemy::getY(), 0.05f, 0.05f, RoomManager::player);
    proj->setSpeed(0.005f);
    RoomManager::currentRoom->getProjectiles().push_back(proj);

    proj = nullptr;
    delete proj;
    isOnCooldown = true;
}

// ROUND-SHOT ENEMY
void RoundShotEnemy::draw() {
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(Entity::x - Entity::width / 2.0f, Entity::y + Entity::height / 2.0f); // Top-left
    glVertex2f(Entity::x + Entity::width / 2.0f, Entity::y + Entity::height / 2.0f); // Top-right
    glVertex2f(Entity::x + Entity::width / 2.0f, Entity::y - Entity::height / 2.0f); // Bottom-right
    glVertex2f(Entity::x - Entity::width / 2.0f, Entity::y - Entity::height / 2.0f); // Bottom-left
    glEnd();

    // Draw the cross
    glBegin(GL_LINES);
    // Horizontal line
    glVertex2f(Entity::x - Entity::width, Entity::y);
    glVertex2f(Entity::x + Entity::width, Entity::y);
    // Vertical line
    glVertex2f(Entity::x, Entity::y - Entity::height);
    glVertex2f(Entity::x, Entity::y + Entity::height);
    // Diagonal line 1 (bottom-left to top-right)
    glVertex2f(Entity::x - Entity::width * 0.75f, Entity::y - Entity::height * 0.75f);
    glVertex2f(Entity::x + Entity::width * 0.75f, Entity::y + Entity::height * 0.75f);
    // Diagonal line 2 (bottom-right to top-left)
    glVertex2f(Entity::x + Entity::width * 0.75f, Entity::y - Entity::height * 0.75f);
    glVertex2f(Entity::x - Entity::width * 0.75f, Entity::y + Entity::height * 0.75f);
    glEnd();
}

void RoundShotEnemy::shoot() {
    isOnCooldown = true;
    std::thread cooldownThread([this]() {
        std::this_thread::sleep_for(cooldown);
        isOnCooldown = false;
        });
    cooldownThread.detach();
    Projectile* proj = new Projectile(RoundShotEnemy::getX(), RoundShotEnemy::getY(), 0.05f, 0.05f, 0.0f, 1.0f);
    proj->setSpeed(0.003f);
    RoomManager::currentRoom->getProjectiles().push_back(proj);
    proj = new Projectile(RoundShotEnemy::getX(), RoundShotEnemy::getY(), 0.05f, 0.05f, 0.0f, -1.0f);
    proj->setSpeed(0.003f);
    RoomManager::currentRoom->getProjectiles().push_back(proj);
    proj = new Projectile(RoundShotEnemy::getX(), RoundShotEnemy::getY(), 0.05f, 0.05f, -1.0f, 0.0f);
    proj->setSpeed(0.003f);
    RoomManager::currentRoom->getProjectiles().push_back(proj);
    proj = new Projectile(RoundShotEnemy::getX(), RoundShotEnemy::getY(), 0.05f, 0.05f, 1.0f, 0.0f);
    proj->setSpeed(0.003f);
    RoomManager::currentRoom->getProjectiles().push_back(proj);
    proj = new Projectile(RoundShotEnemy::getX(), RoundShotEnemy::getY(), 0.05f, 0.05f, 1.0f, -1.0f);
    proj->setSpeed(0.003f);
    RoomManager::currentRoom->getProjectiles().push_back(proj);
    proj = new Projectile(RoundShotEnemy::getX(), RoundShotEnemy::getY(), 0.05f, 0.05f, -1.0f, 1.0f);
    proj->setSpeed(0.003f);
    RoomManager::currentRoom->getProjectiles().push_back(proj);
    proj = new Projectile(RoundShotEnemy::getX(), RoundShotEnemy::getY(), 0.05f, 0.05f, -1.0f, -1.0f);
    proj->setSpeed(0.003f);
    RoomManager::currentRoom->getProjectiles().push_back(proj);
    proj = new Projectile(RoundShotEnemy::getX(), RoundShotEnemy::getY(), 0.05f, 0.05f, 1.0f, 1.0f);
    proj->setSpeed(0.003f);
    RoomManager::currentRoom->getProjectiles().push_back(proj);

    proj = nullptr;
    delete proj;
}

// TANK ENEMY
void TankEnemy::draw() {
    glColor3f(0.35f, 0.1f, 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(Entity::x - Entity::width / 2.0f, Entity::y + Entity::height / 2.0f); // Top-left
    glVertex2f(Entity::x + Entity::width / 2.0f, Entity::y + Entity::height / 2.0f); // Top-right
    glVertex2f(Entity::x + Entity::width / 2.0f, Entity::y - Entity::height / 2.0f); // Bottom-right
    glVertex2f(Entity::x - Entity::width / 2.0f, Entity::y - Entity::height / 2.0f); // Bottom-left
    glEnd();
}

bool TankEnemy::check(float dx, float dy, Entity* entity)
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
            TankEnemy::move(0.0f, -overlapTop - 0.15f); // Move up
        }
        else if (minOverlap == overlapLeft) {
            TankEnemy::move(overlapLeft + 0.25f, 0.0f); // Move right
        }
        else if (minOverlap == overlapBottom) {
            TankEnemy::move(0.0f, overlapBottom + 0.25f); // Move down
        }
        else if (minOverlap == overlapRight) {
            TankEnemy::move(-overlapRight - 0.15f, 0.0f); // Move left
        }

        return true;
    }
    return false;
}

// TIER 3 MINIBOSS BOSS ENEMIES

// SHOTGUN MINIBOSS
void ShotgunEnemy::draw(){
    glColor3f(1.0f, 0.75f, 0.25f);
    glBegin(GL_QUADS);
    glVertex2f(Entity::x - Entity::width / 2.0f, Entity::y - Entity::height / 2.0f); // Bottom-left
    glVertex2f(Entity::x + Entity::width / 2.0f, Entity::y - Entity::height / 2.0f); // Bottom-right
    glVertex2f(Entity::x + Entity::width / 2.0f, Entity::y + Entity::height / 2.0f); // Top-right
    glVertex2f(Entity::x - Entity::width / 2.0f, Entity::y + Entity::height / 2.0f); // Top-left
    glEnd();
}

void ShotgunEnemy::shoot() {
    isOnCooldown = true;
    std::thread cooldownThread([this]() {
        std::this_thread::sleep_for(cooldown);
        isOnCooldown = false;
        });
    cooldownThread.detach();
    
    Projectile* proj = new Projectile(Enemy::getX(), Enemy::getY(), 0.05f, 0.05f, RoomManager::player);
    proj->setSpeed(0.03f);
    RoomManager::currentRoom->getProjectiles().push_back(proj);

    // Define the offset for the scatter effect
    float offset = 0.1f;

    // Calculate the positions of additional projectiles
    Projectile* proj2 = new Projectile(Enemy::getX(), Enemy::getY(), 0.05f, 0.05f, proj->getDx() + offset, proj->getDy() + offset);
    proj2->setSpeed(0.04f);
    RoomManager::currentRoom->getProjectiles().push_back(proj2);

    proj2 = new Projectile(Enemy::getX(), Enemy::getY(), 0.05f, 0.05f, proj->getDx() - offset, proj->getDy() - offset);
    proj2->setSpeed(0.04f);
    RoomManager::currentRoom->getProjectiles().push_back(proj2);

    proj2 = new Projectile(Enemy::getX(), Enemy::getY(), 0.05f, 0.05f, proj->getDx() + offset, proj->getDy() - offset);
    proj2->setSpeed(0.04f);
    RoomManager::currentRoom->getProjectiles().push_back(proj2);

    proj2 = new Projectile(Enemy::getX(), Enemy::getY(), 0.05f, 0.05f, proj->getDx() - offset, proj->getDy() + offset);
    proj2->setSpeed(0.04f);
    RoomManager::currentRoom->getProjectiles().push_back(proj2);

    proj2 = new Projectile(Enemy::getX(), Enemy::getY(), 0.05f, 0.05f, proj->getDx() + offset, proj->getDy());
    proj2->setSpeed(0.04f);
    RoomManager::currentRoom->getProjectiles().push_back(proj2);

    proj2 = new Projectile(Enemy::getX(), Enemy::getY(), 0.05f, 0.05f, proj->getDx() - offset, proj->getDy());
    proj2->setSpeed(0.04f);
    RoomManager::currentRoom->getProjectiles().push_back(proj2);

    proj2 = new Projectile(Enemy::getX(), Enemy::getY(), 0.05f, 0.05f, proj->getDx(), proj->getDy() + offset);
    proj2->setSpeed(0.04f);
    RoomManager::currentRoom->getProjectiles().push_back(proj2);

    proj2 = new Projectile(Enemy::getX(), Enemy::getY(), 0.05f, 0.05f, proj->getDx(), proj->getDy() - offset);
    proj2->setSpeed(0.04f);
    RoomManager::currentRoom->getProjectiles().push_back(proj2);

    proj2 = new Projectile(Enemy::getX(), Enemy::getY(), 0.05f, 0.05f, proj->getDx(), proj->getDy());
    proj2->setSpeed(0.04f); // Central projectile (same position as the original one)
    RoomManager::currentRoom->getProjectiles().push_back(proj2);

    proj = nullptr;
    proj2 = nullptr;

    delete proj;
    delete proj2;
 
}

// INVINCIBLE MINIBOSS
void InvincibleEnemy::draw() {

    glColor3f(0.35f, 0.5f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(Entity::x - Entity::width / 2.0f, Entity::y + Entity::height / 2.0f); // Top-left
    glVertex2f(Entity::x + Entity::width / 2.0f, Entity::y + Entity::height / 2.0f); // Top-right
    glVertex2f(Entity::x + Entity::width / 2.0f, Entity::y - Entity::height / 2.0f); // Bottom-right
    glVertex2f(Entity::x - Entity::width / 2.0f, Entity::y - Entity::height / 2.0f); // Bottom-left
    glEnd();

}

bool InvincibleEnemy::check(float dx, float dy, Entity* entity)
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
            Enemy::move(0.0f, -overlapTop - 0.15f); // Move up
        }
        else if (minOverlap == overlapLeft) {
            Enemy::move(overlapLeft + 0.25f, 0.0f); // Move right
        }
        else if (minOverlap == overlapBottom) {
            Enemy::move(0.0f, overlapBottom + 0.25f); // Move down
        }
        else if (minOverlap == overlapRight) {
            Enemy::move(-overlapRight - 0.15f, 0.0f); // Move left
        }

        return true;
    }
    return false;
}

void InvincibleEnemy::shoot() {
    if (Enemy::getX() >= 1.0f || Enemy::getX() >= -1.0f || Enemy::getY() >= 1.0 || Enemy::getY() >= -1.0f) {
        Enemy::setX(0.0f);
        Enemy::setY(0.0f);
    }

    isOnCooldown = true;
    std::thread cooldownThread([this]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(2200));
        isOnCooldown = false;
        });
    cooldownThread.detach();
    InvincibleEnemy::setSpeed(4.5f);
    std::thread secondCooldownThread([this]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        InvincibleEnemy::setSpeed(0.0f);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(1, 4);
        if (dist(gen) == 1) {
            RoomManager::currentRoom->getObjects().push_back(new AntiInvincible(0.0f, 0.0f, 0.01f, 0.01f));
        }
        });
    secondCooldownThread.detach();
}

// BOSS
void Boss::draw() {

    glColor3f(0.15f, 0.0f, 0.0f);
    if (Boss::beamCooldown <= 1) {
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_LINES);
        glVertex2f(Boss::getX(), Boss::getY()); 
        glVertex2f(RoomManager::player->getX(), RoomManager::player->getY());
    }
    glBegin(GL_QUADS);
    glVertex2f(Entity::x - Entity::width / 2.0f, Entity::y + Entity::height / 2.0f); // Top-left
    glVertex2f(Entity::x + Entity::width / 2.0f, Entity::y + Entity::height / 2.0f); // Top-right
    glVertex2f(Entity::x + Entity::width / 2.0f, Entity::y - Entity::height / 2.0f); // Bottom-right
    glVertex2f(Entity::x - Entity::width / 2.0f, Entity::y - Entity::height / 2.0f); // Bottom-left
    glEnd();
    if (dynamic_cast<const FinalBossRoom*>(RoomManager::currentRoom)) {
        RoomManager::player->setWidth(0.05f);
        RoomManager::player->setHeight(0.05f);
        RoomManager::player->setSpeed(0.5f);
        RoomManager::player->setHealth(100);
    }
}

void Boss::shoot()
{
    const int numProjectiles = 16;
    const float angleIncrement = 360.0f / numProjectiles;
    const float rotationFactor = 8.0f;

    if (isOnCooldown) return;
    isOnCooldown = true;

    std::thread cooldownThread([this]() {
        std::this_thread::sleep_for(Boss::cooldown);
        isOnCooldown = false;
        });
    cooldownThread.detach();

    for (int i = 1; i < numProjectiles + 1; i++) {
        // Calculate the angle for this projectile
        float direction = (i * angleIncrement) + Boss::angle;

        // Convert the angle to radians
        float radians = direction * (3.14159265358979323846 / 180.0f);

        // Calculate the direction vector for this projectile
        float directionX = std::cos(radians);
        float directionY = std::sin(radians);

        // Create and launch the projectile
        Projectile* projectile = new Projectile(Boss::getX(), Boss::getY(), 0.025f, 0.025f, directionX, directionY);
        projectile->setSpeed(0.005f);
        RoomManager::currentRoom->getProjectiles().push_back(projectile);

    }
    Boss::angle += rotationFactor;

    if (Boss::getHealth() == 250 || Boss::getHealth() == 200 || Boss::getHealth() == 150 || Boss::getHealth() == 100 || Boss::getHealth() == 50) Boss::beamRate--;

    if (Boss::beamCooldown == 0) {
        std::cout << "Beam" << std::endl;
        float projectileSpeed = 0.01f;
        for (int i = 1; i <= 10; i++) {
            Projectile* projectile = new Projectile(Boss::getX(), Boss::getY(), 0.025f, 0.025f, RoomManager::player->getX(), RoomManager::player->getY());
            projectile->setSpeed(projectileSpeed);
            RoomManager::currentRoom->getProjectiles().push_back(projectile);
            projectileSpeed += 0.01f;
        }
        beamCooldown = Boss::beamRate;
    }
    else {
        beamCooldown--;
    }
}

