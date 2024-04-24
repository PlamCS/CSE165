#include <GL/freeglut.h>
#include <iostream>
#include "Entity.h"
#include "Rooms.h"
#include "RoomManager.h"
#include <vector>
#include <sstream>

//=================================================================================================
// CALLBACKS
//=================================================================================================

//-----------------------------------------------------------------------------
// CALLBACK DOCUMENTATION
// https://www.opengl.org/resources/libraries/glut/spec3/node45.html
// http://freeglut.sourceforge.net/docs/api.php#WindowCallback
//-----------------------------------------------------------------------------

extern const float MOVESPEED = 0.02f;
const int UPDATEINTERVAL = 16;
const float PLAYERSIZE = 0.1f;
int SCREENWIDTH = 900;
int SCREENHEIGHT = 900;

bool* keyStates = new bool[256];

void keyboardDown( unsigned char key, int x, int y )
{
	if (key == '\x1B') {
		exit(EXIT_SUCCESS);
	}
	keyStates[key] = true;
	glutPostRedisplay();
}

void keyboardUp(unsigned char key, int x, int y) {
	keyStates[key] = false;
}


//=================================================================================================
// RENDERING
//=================================================================================================

Player player = Player(0.0f, 0.0f, PLAYERSIZE, PLAYERSIZE);
RoomManager map = RoomManager();

void display_func( void )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	map.draw();

	glColor3f(0.0f, 1.0f, 0.0f);
	player.draw();

	// Convert score to a string using stringstream
	std::stringstream ss;
	ss << "Score: " << RoomManager::score;
	std::string scoreString = ss.str();

	// Set the text color (white for example)
	glColor3f(1.0f, 1.0f, 1.0f);
	// Set the projection matrix for text rendering (assuming orthographic projection)
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f); // Match the window coordinate system

	// Switch to modelview matrix for text positioning
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Move the text to the top right corner
	glRasterPos2f(0.8f, 0.96f);

	// Render the score string using OpenGL functions
	for (const char& character : scoreString) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, character); // Use a built-in font
	}

	glutSwapBuffers();
}

void update(int value) {

	float dx = 0.0f;
	float dy = 0.0f;

	if (keyStates['w']) dy += MOVESPEED;
	if (keyStates['s']) dy -= MOVESPEED;
	if (keyStates['a']) dx -= MOVESPEED;
	if (keyStates['d']) dx += MOVESPEED;
	
	// Normalize diagonal movement
	if (dx != 0.0f && dy != 0.0f) {
		dx *= 0.7071f;  // sqrt(2)/2 to make diagonal speed same as cardinal directions
		dy *= 0.7071f;
	}
	
	float newX = player.getX() + dx;
	float newY = player.getY() + dy;

	bool collisionX = false;
	bool collisionY = false;

	if (!map.check(newX, player.getY(), player)) player.setX(newX);
	else collisionX = true;

	if (!map.check(player.getX(), newY, player)) player.setY(newY);
	else collisionY = true;

	if (collisionX && collisionY) {
		player.setX(player.getX());
		player.setY(player.getY());
	}

	glutTimerFunc(UPDATEINTERVAL, update, 0);
	glutPostRedisplay();

}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
}

//=================================================================================================
// INIT
//=================================================================================================

void init( void )
{
	// Print some info
	std::cout << "Vendor:         " << glGetString( GL_VENDOR   ) << "\n";
	std::cout << "Renderer:       " << glGetString( GL_RENDERER ) << "\n";
	std::cout << "OpenGL Version: " << glGetString( GL_VERSION  ) << "\n\n";

	// Set the background color
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

	std::cout << "Finished initializing...\n\n";
}

//=================================================================================================
// MAIN
//=================================================================================================

int main( int argc, char** argv )
{
	glutInit( &argc, argv );

	glutInitWindowPosition( 100, 100 );
	glutInitWindowSize(SCREENWIDTH, SCREENHEIGHT);
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );

	glutCreateWindow( "Basic OpenGL Example" );

	glutDisplayFunc( display_func );
	glutReshapeFunc(reshape);
	glutKeyboardFunc( keyboardDown );
	glutKeyboardUpFunc(keyboardUp);
	glutTimerFunc(0, update, 0);
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	init();

	glutMainLoop();

	return EXIT_SUCCESS;
}
