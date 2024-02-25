#include <iostream>
#include "Vec.h"

using namespace std;

Vec nullvec( 0.0f, 0.0f );

bool equal( const Vec & u, const Vec& v )
{
	return u.x == v.x && u.y == v.y;
}

int main( int argc, const char* argv[] )
{
	Vec u;
	Vec v;
	Vec w( 1.0f, 2.0f );

	if( equal( u, nullvec ) == true ) cout << "Test 1 passed!\n";
	else cout << "error1\n";

	v.set( 1.0f, 2.0f );
	if( equal( v, w ) == true ) cout << "Test 2 passed!\n";
	else cout << "error2\n";

	for( int i = 0; i < 10; i++ )
		u.add( w );

	u.print();

	if( equal( u, Vec( 10.0f, 20.0f ) ) == true ) cout << "Test 3 passed!\n";
	else cout << "error3\n";

	cout << "Finished!\n";

	return 0;
}