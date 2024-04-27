#include "Trap.h"
#include "RoomManager.h"

void SpikeTrap::Activate()
{
    std::thread activationThread([this]() {
        std::this_thread::sleep_for(activationDelay);
        SpikeTrap::activated = true;
        });
    activationThread.detach();
}

bool SpikeTrap::check(float x, float y, Entity* object)
{
    if (object != RoomManager::player) return false;

    bool condition = (x - object->getWidth() / 2.0f <= Entity::getX() + Entity::getWidth() / 2.0f &&
        x + object->getWidth() / 2.0f >= Entity::getX() - Entity::getWidth() / 2.0f &&
        y - object->getHeight() / 2.0f <= Entity::getY() + Entity::getHeight() / 2.0f &&
        y + object->getHeight() / 2.0f >= Entity::getY() - Entity::getHeight() / 2.0f);

    if (condition && !SpikeTrap::activated) {
        Activate();
        std::thread damageThread([this]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
            if (SpikeTrap::activated) {
                RoomManager::score -= 10;
            }
            });
        damageThread.detach();
    }

    return false;
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

void StatusTrap::Activate() {
    StatusTrap::activated = true;
    std::thread activationThread([this]() {
        std::this_thread::sleep_for(StatusTrap::activationDelay + std::chrono::milliseconds(1500));
        StatusTrap::activated = false;
        RoomManager::player->setSpeed(1.0f);
        });
    activationThread.detach();
}

void StatusTrap::ApplyEffect(Entity* entity) {
    if (StatusTrap::activated) {
        RoomManager::player->setSpeed(StatusTrap::statusSpeed);
    }
}

bool StatusTrap::check(float dx, float dy, Entity* object)
{
    if (object != RoomManager::player) return false;
    bool condition = (dx - RoomManager::player->getWidth() / 2.0f <= Entity::getX() + Entity::getWidth() / 2.0f &&
                        dx + RoomManager::player->getWidth() / 2.0f >= Entity::getX() - Entity::getWidth() / 2.0f &&
                        dy - RoomManager::player->getHeight() / 2.0f <= Entity::getY() + Entity::getHeight() / 2.0f &&
                        dy + RoomManager::player->getHeight() / 2.0f >= Entity::getY() - Entity::getHeight() / 2.0f);

    if (condition && !StatusTrap::activated) {
        Activate();
    }
    else if (condition && StatusTrap::activated) {
        ApplyEffect(RoomManager::player);
    }
    return false;
}

void StatusTrap::draw()
{
    if (StatusTrap::statusSpeed < 1.0f) glColor3f(0.0f, 0.0f, 1.0f);
    else if(StatusTrap::statusSpeed > 1.0f) glColor3f(1.0f, 0.5f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(Entity::x - Entity::width / 2.0f, Entity::y - Entity::height / 2.0f); // Bottom-left
    glVertex2f(Entity::x + Entity::width / 2.0f, Entity::y - Entity::height / 2.0f); // Bottom-right
    glVertex2f(Entity::x + Entity::width / 2.0f, Entity::y + Entity::height / 2.0f); // Top-right
    glVertex2f(Entity::x - Entity::width / 2.0f, Entity::y + Entity::height / 2.0f); // Top-left
    glEnd();
}

