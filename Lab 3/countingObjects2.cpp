#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Object.h"

using namespace std;

int main()
{
    Object a;
    Object b( 5 );
    a = b;
    {
        Object c = b;
    }
    
    cout << "Sum of a.x and b.x: " << a.x + b.x << endl;

    srand(time(NULL));
    int n = rand() % 10 + 1; //random number from 1 to 10
    cout << "Generating " << n << " more object(s)..." << endl;
    Object* p = new Object[n];

    cout << "Final object count: " << Object::count << endl;
    
    delete[] p;
    
    return 0;
}
