#include "Items.h"
#include "RoomManager.h"
#include <iostream>

#include <atomic>   // Include atomic for atomic boolean
#include <thread>   // Include thread for sleeping
#include <chrono>   // Include chrono for time-related functions

// SHIELD SHAPE OR BOSS HEART

//glColor3f(1.0f, 0.0f, 0.0f);  // Red color
	//glBegin(GL_TRIANGLES);

	// Bottom triangle
	//glVertex2f(Entity::x, Entity::y - Entity::height / 2.0f);               // Bottom point
	//glVertex2f(Entity::x - Entity::width / 2.0f, Entity::y + Entity::height / 4.0f);  // Left point
	//glVertex2f(Entity::x + Entity::width / 2.0f, Entity::y + Entity::height / 4.0f);  // Right point

	// Left lobe
	//glVertex2f(Entity::x - Entity::width / 2.0f, Entity::y + Entity::height / 4.0f);  // Left point
	//glVertex2f(Entity::x, Entity::y + Entity::height / 2.0f);                   // Top-left curve
	//glVertex2f(Entity::x, Entity::y + Entity::height / 4.0f);                   // Bottom-left curve

	// Right lobe
	//glVertex2f(Entity::x + Entity::width / 2.0f, Entity::y + Entity::height / 4.0f);  // Right point
	//glVertex2f(Entity::x, Entity::y + Entity::height / 2.0f);                   // Top-right curve
	//glVertex2f(Entity::x, Entity::y + Entity::height / 4.0f);                   // Bottom-right curve

	//glEnd();

// Heart Item

void Heart::draw(){
	// Set the width and height of the rhombus
	float rhombusWidth = 0.1f;
	float rhombusHeight = 0.2f;

	glColor3f(0.0f, 1.0f, 0.0f); // Set color to green
	glBegin(GL_QUADS);

	// Top
	glVertex2f(Entity::x, Entity::y + rhombusHeight / 5.0f); // Top
	glVertex2f(Entity::x + rhombusWidth / 5.0f, Entity::y); // Right
	glVertex2f(Entity::x, Entity::y - rhombusHeight / 5.0f); // Bottom
	glVertex2f(Entity::x - rhombusWidth / 5.0f, Entity::y); // Left

	// Bottom
	glVertex2f(Entity::x, Entity::y - rhombusHeight / 5.0f); // Top
	glVertex2f(Entity::x + rhombusWidth / 5.0f, Entity::y); // Right
	glVertex2f(Entity::x, Entity::y + rhombusHeight / 5.0f); // Bottom
	glVertex2f(Entity::x - rhombusWidth / 5.0f, Entity::y); // Left

	glEnd();

}

bool Heart::check(float dx, float dy, Entity* object) {
	bool condition = (dx - object->getWidth() / 2.0f <= Entity::getX() + Entity::getWidth() / 2.0f &&
		dx + object->getWidth() / 2.0f >= Entity::getX() - Entity::getWidth() / 2.0f &&
		dy - object->getHeight() / 2.0f <= Entity::getY() + Entity::getHeight() / 2.0f &&
		dy + object->getHeight() / 2.0f >= Entity::getY() - Entity::getHeight() / 2.0f);

	if (condition) {
		RoomManager::player->increaseHealth();
		return true;
	}
	return false;
}

// Speed Item

void SpeedBuff::draw() {
	// Set the color to turquoise for the diamond
	glColor3f(0.25f, 0.88f, 0.82f);

	// Begin drawing filled polygon for the diamond
	glBegin(GL_POLYGON);

	// Define the points for the diamond
	glVertex2f(x, y + height / 2.0f); // Top point
	glVertex2f(x + width / 2.0f, y); // Right point
	glVertex2f(x, y - height / 2.0f); // Bottom point
	glVertex2f(x - width / 2.0f, y); // Left point

	glEnd();
}

bool SpeedBuff::check(float dx, float dy, Entity* object) {
	bool condition = (dx - object->getWidth() / 2.0f <= Entity::getX() + Entity::getWidth() / 2.0f &&
		dx + object->getWidth() / 2.0f >= Entity::getX() - Entity::getWidth() / 2.0f &&
		dy - object->getHeight() / 2.0f <= Entity::getY() + Entity::getHeight() / 2.0f &&
		dy + object->getHeight() / 2.0f >= Entity::getY() - Entity::getHeight() / 2.0f);

	if (condition) {
		static std::atomic<bool> isOnCooldown = false;

		isOnCooldown = true;
		std::thread cooldownThread([this]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(10000));
			isOnCooldown = false;
			});
		cooldownThread.detach();
		RoomManager::player->setSpeed(RoomManager::player->getSpeed() + 0.25f);
		std::cout << "Move" << std::endl;
		std::thread secondCooldownThread([this]() {
			std::this_thread::sleep_for(std::chrono::milliseconds(10000));
			RoomManager::player->setSpeed(RoomManager::player->getSpeed() - 0.25f);
			std::cout << "Stop" << std::endl;
			});
		secondCooldownThread.detach();
		return true;
	}
	return false;
}

// Invincible Item

void AntiInvincible::draw() {
	// Set the number of points for the star
	int numPoints = 10;

	// Set the color to gold for the star
	glColor3f(1.0f, 0.84f, 0.0f);

	// Begin drawing lines for the star
	glBegin(GL_LINE_LOOP);

	// Define the points for the star
	for (int i = 0; i < numPoints; ++i) {
		float angle = 2.0f * 3.1415926f * float(i) / float(numPoints);
		float outerRadius = (width + height);
		float innerRadius = outerRadius / 2.0f;

		if (i % 2 == 0) {
			glVertex2f(x + outerRadius * cosf(angle), y + outerRadius * sinf(angle));
		}
		else {
			glVertex2f(x + innerRadius * cosf(angle), y + innerRadius * sinf(angle));
		}
	}

	glEnd();
}

bool AntiInvincible::check(float dx, float dy, Entity* object) {
	bool condition = (dx - object->getWidth() / 2.0f <= Entity::getX() + Entity::getWidth() / 2.0f &&
		dx + object->getWidth() / 2.0f >= Entity::getX() - Entity::getWidth() / 2.0f &&
		dy - object->getHeight() / 2.0f <= Entity::getY() + Entity::getHeight() / 2.0f &&
		dy + object->getHeight() / 2.0f >= Entity::getY() - Entity::getHeight() / 2.0f);
	if (condition) {
		RoomManager::currentRoom->getEnemies()[0]->decreaseHealth();
		return true;
	}
	return false;
}