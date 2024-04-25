#include "Entity.h"
#include "RoomManager.h"
#include <iostream>
#include <random>

std::string getTypeName(const Entity* entity) {
    if (dynamic_cast<const Wall*>(entity)) {
        return "Wall";
    }
    else if (dynamic_cast<const SpikeTrap*>(entity)) {
        return "SpikeTrap";
    }
    else if (dynamic_cast<const LWall*>(entity)) {
        return "LWall";
    }
    else {
        return "Unknown";
    }
}

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
    float newX = Entity::getX() + dx;
    float newY = Entity::getY() + dy;

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
    //bool wall1Collision = false;
    //bool wall2Collision = false;
    //if (LWall::Wall1->check(x, y, object)) wall1Collision = true;
    //if (LWall::Wall2->check(x, y, object)) wall2Collision = true;
    return (LWall::Wall1->check(x, y, object) || LWall::Wall2->check(x, y, object));
}

bool SpikeTrap::check(float x, float y, Entity* object)
{
    if (object != RoomManager::player) return false;
    bool condition = (x - object->getWidth() / 2.0f <= Entity::getX() + Entity::getWidth() / 2.0f &&
        x + object->getWidth() / 2.0f >= Entity::getX() - Entity::getWidth() / 2.0f &&
        y - object->getHeight() / 2.0f <= Entity::getY() + Entity::getHeight() / 2.0f &&
        y + object->getHeight() / 2.0f >= Entity::getY() - Entity::getHeight() / 2.0f);
    
    if (condition && !SpikeTrap::activated) {
        SpikeTrap::Activate();

        std::thread activationThread([this]() {
            std::this_thread::sleep_for(activationDelay + std::chrono::milliseconds(3000));
            SpikeTrap::activated = false;
            std::cout << "Trap deactivated!" << std::endl;
            });
        activationThread.detach();

    }
    else if (condition && SpikeTrap::activated) {
        std::cout << "Player Takes Damage" << std::endl;
        RoomManager::score -= 10;
    }
    return false;
}

void SpikeTrap::Activate()
{  
    std::thread activationThread([this]() {
        std::this_thread::sleep_for(activationDelay);
        SpikeTrap::activated = true;
        std::cout << "Trap activated!" << std::endl;
        });
    activationThread.detach();
}

void SpikeTrap::draw() {
    if (!SpikeTrap::activated) glColor3f(0.561f, 0.525f, 0.373f);
    else  glColor3f(0.188f, 0.188f, 0.188f);
    glBegin(GL_QUADS);
    glVertex2f(Entity::x - Entity::width / 2.0f, Entity::y - Entity::height / 2.0f); // Bottom-left
    glVertex2f(Entity::x + Entity::width / 2.0f, Entity::y - Entity::height / 2.0f); // Bottom-right
    glVertex2f(Entity::x + Entity::width / 2.0f, Entity::y + Entity::height / 2.0f); // Top-right
    glVertex2f(Entity::x - Entity::width / 2.0f, Entity::y + Entity::height / 2.0f); // Top-left
    glEnd();

}

void Enemy::draw() {
    glBegin(GL_QUADS);
    glVertex2f(Entity::x - Entity::width / 2.0f, Entity::y - Entity::height / 2.0f); // Bottom-left
    glVertex2f(Entity::x + Entity::width / 2.0f, Entity::y - Entity::height / 2.0f); // Bottom-right
    glVertex2f(Entity::x + Entity::width / 2.0f, Entity::y + Entity::height / 2.0f); // Top-right
    glVertex2f(Entity::x - Entity::width / 2.0f, Entity::y + Entity::height / 2.0f); // Top-left
    glEnd();
    for (int index = 0; index < Enemy::projectiles.size(); ++index) {
        Projectile* projectile = Enemy::projectiles[index];
        projectile->draw();
        for (auto& object : RoomManager::currentRoom->getObjects()) {
            if (getTypeName(object) == "SpikeTrap") continue;
            else if (object->check(projectile->getX(), projectile->getY(), projectile) || RoomManager::player->check(projectile->getX(), projectile->getY(), projectile)) {
                delete projectile;
                Enemy::projectiles.erase(Enemy::projectiles.begin() + index);
                --index;
                break;
            }
            projectile->move();
        }
    }
}

bool Enemy::check(float pointX, float pointY, Entity* object)
{
    Enemy::counter++;
    //std::cout << "Projectile test. The counter is: " << Enemy::counter << std::endl;
    if (Enemy::counter == 150) {
        std::cout << "Spawning projectile" << std::endl;
        Enemy::projectiles.push_back(new Projectile(Enemy::getX(), Enemy::getY(), 0.05f, 0.05f, 0.001f, object));
        Enemy::counter = 0;
    }
    return false;
}

