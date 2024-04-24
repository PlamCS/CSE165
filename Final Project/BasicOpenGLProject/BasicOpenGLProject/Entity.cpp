#include "Entity.h"
#include "RoomManager.h"
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

bool Entity::check(float x, float y, Entity* object)
{
    return (x - object->getWidth() / 2.0f <= Entity::getX() + Entity::getWidth() / 2.0f &&
            x + object->getWidth() / 2.0f >= Entity::getX() - Entity::getWidth() / 2.0f &&
            y - object->getHeight() / 2.0f <= Entity::getY() + Entity::getHeight() / 2.0f &&
            y + object->getHeight() / 2.0f >= Entity::getY() - Entity::getHeight() / 2.0f);
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
    bool wall1Collision = false;
    bool wall2Collision = false;
    if (LWall::Wall1->check(x, y, object)) wall1Collision = true;
    if (LWall::Wall2->check(x, y, object)) wall2Collision = true;
    return (wall1Collision || wall2Collision);
}

LWall::~LWall()
{
    delete LWall::Wall1;
    delete LWall::Wall2;
    delete this;
}

bool SpikeTrap::check(float x, float y, Entity* object)
{
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
