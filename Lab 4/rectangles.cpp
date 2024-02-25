#include <iostream>
#include "Rect.h"

using namespace std;

Vec nullvec( 0.0f, 0.0f );

int main( int argc, const char* argv[] )
{
	Rect a( -1.0f,  1.0f, 3.0f, 4.0f );
	Rect b( -5.0f, -5.0f, 2.0f, 2.0f );
	Rect c(  5.0f,  8.0f, 2.0f, 2.0f );

	if( a.contains( nullvec ) == true ) cout << "Test 1 passed!\n";
	else cout << "error1\n";

	if( b.contains( nullvec ) == false ) cout << "Test 2 passed!\n";
	else cout << "error2\n";

	if( c.contains( nullvec ) == false ) cout << "Test 3 passed!\n";
	else cout << "error3\n";

	if( a.contains( Vec( 0.0f, 3.0f ) ) == false ) cout << "Test 4 passed!\n";
	else cout << "error4\n";

	if( b.contains( Vec( -4.0f, -6.0f ) ) == true ) cout << "Test 5 passed!\n";
	else cout << "error5\n";

	if( c.contains( Vec( 6.0f, 9.0f ) ) == false ) cout << "Test 6 passed!\n";
	else cout << "error6\n";

	cout << "Finished!\n";

	return 0;
}