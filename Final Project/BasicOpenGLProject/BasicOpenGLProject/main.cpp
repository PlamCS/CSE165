#include <GL/freeglut.h>
#include <iostream>
#include "Object.h"
#include "Entity.h"
#include <vector>

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

bool* keyStates = new bool[256];
Entity Player = Entity(0.0f, 0.0f, PLAYERSIZE);
std::vector<Object> objects;


void keyboard_func( unsigned char key, int x, int y )
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

void display_func( void )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	Player.draw();
	
	Object TopWall = Object(0.0f, 1.0f, PLAYERSIZE, 2.0f, 0.2f);
	Object BottomWall = Object(0.0f, -1.0f, PLAYERSIZE, 2.0f, 0.2f);
	Object LeftWall = Object(-1.0f, 0.0f, PLAYERSIZE, 0.2f, 2.0f);
	Object RightWall = Object(1.0f, 0.0f, PLAYERSIZE, 0.2f, 2.0f);
	objects.push_back(TopWall);
	objects.push_back(BottomWall);
	objects.push_back(LeftWall);
	objects.push_back(RightWall);

	for (auto& object : objects) {
		object.draw();
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
	
	float newX = Player.getX() + dx;
	float newY = Player.getY() + dy;
	bool collisionX = false;
	bool collisionY = false;


	for (const auto& object : objects) {
		float objectLeft = object.getX() - object.getWidth() / 2.0f;
		float objectRight = object.getX() + object.getWidth() / 2.0f;
		float objectTop = object.getY() + object.getHeight() / 2.0f;
		float objectBottom = object.getY() - object.getHeight() / 2.0f;

		if (newX + PLAYERSIZE / 2.0f >= objectLeft && newX - PLAYERSIZE / 2.0f <= objectRight &&
			Player.getY() + PLAYERSIZE / 2.0f >= objectBottom && Player.getY() - PLAYERSIZE / 2.0f <= objectTop) {
			// Collision detected along X-axis
			collisionX = true;
		}

		if (newY + PLAYERSIZE / 2.0f >= objectBottom && newY - PLAYERSIZE / 2.0f <= objectTop &&
			Player.getX() + PLAYERSIZE / 2.0f >= objectLeft && Player.getX() - PLAYERSIZE / 2.0f <= objectRight) {
			// Collision detected along Y-axis
			collisionY = true;
		}
	}

	if (!collisionX && newX - PLAYERSIZE / 2.0f >= -1.0f && newX + PLAYERSIZE / 2.0f <= 1.0f)
		Player.setX(newX);
	if (!collisionY && newY - PLAYERSIZE / 2.0f >= -1.0f && newY + PLAYERSIZE / 2.0f <= 1.0f)
		Player.setY(newY);

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
	glutInitWindowSize( 800, 800);
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );

	glutCreateWindow( "Basic OpenGL Example" );

	glutDisplayFunc( display_func );
	glutReshapeFunc(reshape);
	glutKeyboardFunc( keyboard_func );
	glutKeyboardUpFunc(keyboardUp);
	glutTimerFunc(0, update, 0);
	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
	init();

	glutMainLoop();

	return EXIT_SUCCESS;
}
