#include "RoomManager.h"
#include "Trap.h"

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

void DartTrap::Activate()
{
    float offset = 0.1f;
    
    float dirXOne = DartTrap::getX() + offset;
    float dirXTwo = DartTrap::getX();
    float dirXThree = DartTrap::getX() - offset;

    float dirYOne = DartTrap::getY() + offset;
    float dirYTwo = DartTrap::getY();
    float dirYThree = DartTrap::getY() - offset;

    float dirProjY = 0.0f;
    float dirProjX = 0.0f;
    if (DartTrap::direction == 0) {
        dirProjX = 1.0f;
        dirXOne = DartTrap::getX() + DartTrap::getWidth();
        dirXTwo = DartTrap::getX() + DartTrap::getWidth();
        dirXThree = DartTrap::getX() + DartTrap::getWidth();
        dirYOne = DartTrap::getY() + offset;
        dirYTwo = DartTrap::getY();
        dirYThree = DartTrap::getY() - offset;
    }
    else if (DartTrap::direction == 90) {
        dirProjY = 1.0f;
        dirXOne = DartTrap::getX() + offset;
        dirXTwo = DartTrap::getX();
        dirXThree = DartTrap::getX() - offset;
        dirYOne = DartTrap::getY() + DartTrap::getWidth();
        dirYTwo = DartTrap::getY() + DartTrap::getWidth();
        dirYThree = DartTrap::getY() + DartTrap::getWidth();

    }
    else if (DartTrap::direction == 180) {
        dirProjX = -1.0f;
        dirXOne = DartTrap::getX() - DartTrap::getWidth();
        dirXTwo = DartTrap::getX() - DartTrap::getWidth();
        dirXThree = DartTrap::getX() - DartTrap::getWidth();
        dirYOne = DartTrap::getY() + offset;
        dirYTwo = DartTrap::getY();
        dirYThree = DartTrap::getY() - offset;
        
    }
    else if (DartTrap::direction == 270) {
        dirProjY = -1.0f;
        dirXOne = DartTrap::getX() + offset;
        dirXTwo = DartTrap::getX();
        dirXThree = DartTrap::getX() - offset;
        dirYOne = DartTrap::getY() - DartTrap::getWidth();
        dirYTwo = DartTrap::getY() - DartTrap::getWidth();
        dirYThree = DartTrap::getY() - DartTrap::getWidth();
    }
    
    Projectile* proj = new Projectile(dirXOne, dirYOne, 0.03f, 0.03f, dirProjX, dirProjY);
    proj->setSpeed(0.007f);
    RoomManager::currentRoom->getProjectiles().push_back(proj);

    proj = new Projectile(dirXTwo, dirYTwo, 0.03f, 0.03f, dirProjX, dirProjY);
    proj->setSpeed(0.007f);
    RoomManager::currentRoom->getProjectiles().push_back(proj);
    
    proj = new Projectile(dirXThree, dirYThree, 0.03f, 0.03f, dirProjX, dirProjY);
    proj->setSpeed(0.007f);
    RoomManager::currentRoom->getProjectiles().push_back(proj);

    proj = nullptr;
    delete proj;

    DartTrap::activated = true;
}

bool DartTrap::check(float dx, float dy, Entity* object)
{
    bool directionOne = dx - RoomManager::player->getWidth() / 2.0f <= Entity::getX() + Entity::getWidth() / 2.0f;
    bool directionTwo = dx + RoomManager::player->getWidth() / 2.0f >= Entity::getX() - Entity::getWidth() / 2.0f;
    bool directionThree = dy - RoomManager::player->getHeight() / 2.0f <= Entity::getY() + Entity::getHeight() / 2.0f;
    bool directionFour = dy + RoomManager::player->getHeight() / 2.0f >= Entity::getY() - Entity::getHeight() / 2.0f;

    if (DartTrap::direction == 0) directionOne = dx - RoomManager::player->getWidth() / 2.0f <= Entity::getX() + Entity::getWidth() + 4.0f / 2.0f;
    else if (DartTrap::direction == 90) directionThree = dy - RoomManager::player->getHeight() / 2.0f <= Entity::getY() + Entity::getHeight() + 4.0f / 2.0f;
    else if (DartTrap::direction == 180) directionTwo = dx + RoomManager::player->getWidth() / 2.0f >= Entity::getX() - Entity::getWidth() - 4.0f / 2.0f;
    else if (DartTrap::direction == 270) directionFour = dy + RoomManager::player->getHeight() / 2.0f >= Entity::getY() - Entity::getHeight() - 4.0f / 2.0f;

    bool condition = (directionOne && directionTwo && directionThree && directionFour);

    if (condition && !DartTrap::activated) {
        Activate();
        std::thread damageThread([this]() {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            DartTrap::activated = false;
            });
        damageThread.detach();
    }

    return false;
}

void DartTrap::draw()
{
    float w = DartTrap::getWidth();
    float h = DartTrap::getHeight();
    
    if (DartTrap::direction == 90 || DartTrap::direction == 270) {
        w = DartTrap::getHeight();
        h = DartTrap::getWidth();
    }
        
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
    glVertex2f(Entity::x - w / 2.0f, Entity::y - h / 2.0f); // Bottom-left
    glVertex2f(Entity::x + w / 2.0f, Entity::y - h / 2.0f); // Bottom-right
    glVertex2f(Entity::x + w / 2.0f, Entity::y + h / 2.0f); // Top-right
    glVertex2f(Entity::x - w / 2.0f, Entity::y + h / 2.0f); // Top-left
    glEnd();
}
