#include "EnemyTypes.h"
#include "RoomManager.h"
#include <iostream>
#include "Items.h"
#include <cmath>
#include "Trap.h"
#include <random>
#include <math.h>

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


    if(Boss::colorRed < 0.9f) glColor3f(0.15f + Boss::colorRed, 0.0f, 0.0f);
    else {
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
}

void Boss::shoot()
{
    if (Boss::getHealth() > 400) Boss::phase1();
    else if (Boss::getHealth() > 200);
    else if (Boss::getHealth() > 20);
        
}

void Boss::phase1() {
    Boss::colorRed += 0.15f;

    if (Boss::colorRed >= 1.0f) {
        Boss::colorRed = 0.0f;
    }
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

    if (beamCooldown == 6) {
        float projectileSpeed = 0.01f;
        for (int i = 1; i <= 10; i++) {
            Projectile* projectile = new Projectile(Boss::getX(), Boss::getY(), 0.025f, 0.025f, RoomManager::player->getX(), RoomManager::player->getY());
            projectile->setSpeed(projectileSpeed);
            RoomManager::currentRoom->getProjectiles().push_back(projectile);
            projectileSpeed += 0.01f;
        }
        beamCooldown = 0;
    }
    else {
        beamCooldown++;
    }
}

void Boss::phase2() {

}

void Boss::phase3() {

}
