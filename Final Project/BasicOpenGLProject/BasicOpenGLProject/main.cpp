#include <GL/freeglut.h>
#include <iostream>
#include "Map.h"
#include "Entity.h"

//=================================================================================================
// CALLBACKS
//=================================================================================================

//-----------------------------------------------------------------------------
// CALLBACK DOCUMENTATION
// https://www.opengl.org/resources/libraries/glut/spec3/node45.html
// http://freeglut.sourceforge.net/docs/api.php#WindowCallback
//-----------------------------------------------------------------------------

const float moveSpeed = 0.020f;
const int updateInterval = 16;
bool* keyStates = new bool[256];
Map map = Map();
Entity Player = Entity();

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

	map.draw();
	Player.draw();

	glutSwapBuffers();
}

void update(int value) {

	float dx = 0.0f;
	float dy = 0.0f;

	if (keyStates['w']) dy += moveSpeed;
	if (keyStates['s']) dy -= moveSpeed;
	if (keyStates['a']) dx -= moveSpeed;
	if (keyStates['d']) dx += moveSpeed;
	

	// Normalize diagonal movement
	if (dx != 0.0f && dy != 0.0f) {
		dx *= 0.7071f;  // sqrt(2)/2 to make diagonal speed same as cardinal directions
		dy *= 0.7071f;
	}

	Player.move(dx, dy);

	glutTimerFunc(updateInterval, update, 0);
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
	glutInitWindowSize( 800, 600 );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );

	glutCreateWindow( "Basic OpenGL Example" );
	glutFullScreen();

	glutDisplayFunc( display_func );
	glutReshapeFunc(reshape);
	glutKeyboardFunc( keyboard_func );
	glutKeyboardUpFunc(keyboardUp);
	glutTimerFunc(0, update, 0);
	init();

	glutMainLoop();

	return EXIT_SUCCESS;
}
