#include "EnemyTypes.h"
#include "RoomManager.h"
#include <iostream>
#include <cmath>


// FIRST TIER ENEMIES

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
    std::cout << "Pushing projectile" << std::endl;
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
    std::cout << "Pushing 1 Projectiles" << std::endl;
    std::thread secondCooldownThread([this]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(2400));
        Projectile* proj2 = new Projectile(TwoBurstEnemy::getX(), TwoBurstEnemy::getY(), 0.05f, 0.05f, RoomManager::player);
        proj2->setSpeed(0.002f);
        RoomManager::currentRoom->getProjectiles().push_back(proj2);
        std::cout << "Pushing 2 Projectile" << std::endl;
        proj2 = nullptr;
        delete proj2;
        });
    secondCooldownThread.detach();
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

    std::cout << "Pushing projectile" << std::endl;
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

bool TankEnemy::check(float dx, float dy, Entity* entity) {
    bool overlapping = (dx - entity->getWidth() / 2.0f <= Entity::getX() + Entity::getWidth() / 2.0f &&
        dx + entity->getWidth() / 2.0f >= Entity::getX() - Entity::getWidth() / 2.0f &&
        dy - entity->getHeight() / 2.0f <= Entity::getY() + Entity::getHeight() / 2.0f &&
        dy + entity->getHeight() / 2.0f >= Entity::getY() - Entity::getHeight() / 2.0f);

    if (dynamic_cast<const Projectile*>(entity) && overlapping) return true;
    if ((overlapping && dynamic_cast<const Wall*>(entity) || dynamic_cast<const LWall*>(entity)) || (overlapping && RoomManager::player == entity)) {
        float overlapTop = getY() + getHeight() / 2.0f - dy + entity->getHeight() / 2.0f;
        float overlapBottom = dy + entity->getHeight() / 2.0f - getY() - getHeight() / 2.0f;
        float overlapRight = getX() + getWidth() / 2.0f - dx + entity->getWidth() / 2.0f;
        float overlapLeft = dx + entity->getWidth() / 2.0f - getX() - getWidth() / 2.0f;

        float minOverlap = std::min({ overlapTop, overlapBottom, overlapLeft, overlapRight });

        // Adjust the enemy's position based on the side of the collision
        if (minOverlap == overlapTop) {
            std::cout << "Top" << std::endl;
            Enemy::move(0.0f, -overlapTop - 0.55f); // Move up
        }
        else if (minOverlap == overlapLeft) {
            std::cout << "Left" << std::endl;
            Enemy::move(overlapLeft + 0.65f, 0.0f); // Move right
        }
        else if (minOverlap == overlapBottom) {
            std::cout << "Bottom" << std::endl;
            Enemy::move(0.0f, overlapBottom + 0.65f); // Move down
        }
        else if (minOverlap == overlapRight) {
            std::cout << "Right" << std::endl;
            Enemy::move(-overlapRight - 0.55f, 0.0f); // Move left
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
    glColor3f(1.0f, 0.5f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(Entity::x - Entity::width / 2.0f, Entity::y + Entity::height / 2.0f); // Top-left
    glVertex2f(Entity::x + Entity::width / 2.0f, Entity::y + Entity::height / 2.0f); // Top-right
    glVertex2f(Entity::x + Entity::width / 2.0f, Entity::y - Entity::height / 2.0f); // Bottom-right
    glVertex2f(Entity::x - Entity::width / 2.0f, Entity::y - Entity::height / 2.0f); // Bottom-left
    glEnd();
}