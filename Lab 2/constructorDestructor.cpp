#include <iostream>
#include "Stack.h"

using namespace std;

void delete_func( void * pt )
{
    double* dpt = (double*)pt;
    cout << "Deleting: " << *dpt << "\n";
    delete dpt;
}

int main( int argc, const char* argv[] )
{
	cout << "s1\n";
	{
		Stack s1;
		s1.setDeleteCallback( delete_func );
		
		for( int i = 1; i < 5; i++ )
			s1.push( new double( double(i)/2.0 ) );
	}

	cout << endl;

	cout << "s2\n";
	{
		Stack s2(4);
		s2.setDeleteCallback( delete_func );
	}

	cout << "\nend\n";

	return 0;
}