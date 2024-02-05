#include <iostream>
#include <math.h>

using namespace std;

int fake_main( int argc, char *argv[] )
{
	double a, b, c;

	cin >> a;
	cin >> b;
	cin >> c;

	cout << (-b + (sqrt((b * b) - (4 * a * c)))) / (2 * a) << endl;
	cout << (-b - (sqrt((b*b) - (4 * a * c)))) / (2 * a) << endl;
	
	return 0;
}