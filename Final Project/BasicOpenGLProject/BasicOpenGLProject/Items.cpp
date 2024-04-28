#include "Items.h"
#include "RoomManager.h"
#include <iostream>

// SHIELD SHAPE 

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
void drawSquare(float x, float y, float width, float height) {
	glBegin(GL_QUADS);
	glVertex2f(x - width / 2.0f, y + height / 2.0f); // Top-left
	glVertex2f(x + width / 2.0f, y + height / 2.0f); // Top-right
	glVertex2f(x + width / 2.0f, y - height / 2.0f); // Bottom-right
	glVertex2f(x - width / 2.0f, y - height / 2.0f); // Bottom-left
	glEnd();
}



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