#include <iostream>
#include "createArray.h"

using namespace std;

int main()
{
    int* array1 = nullptr;
    cout << "Creating array1..." << endl;
    createArray( 5, array1 );
    cout << "Printing array1..." << endl;
    printArray( 5, array1 );

    int* array2 = nullptr;
    cout << "Creating array2..." << endl;
    createArray( 0, array2 );
    cout << "Printing array2..." << endl;
    printArray( 5, array2 );
    
    int* array3 = nullptr;
    cout << "Creating array3..." << endl;
    createArray( 15, array3 );
    cout << "Printing array3..." << endl;
    printArray( 15, array3 );

    return 0;
}