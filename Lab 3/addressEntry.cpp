#include <iostream>
#include "Entry.h"

using namespace std;

int main()
{
	string name, lastname, email;

	cin >> name;
	cin >> lastname;
	cin >> email;

	Entry myEntry;

	myEntry.setName( name );
	myEntry.setLastName( lastname );
	myEntry.setEmail( email );

	myEntry.print();

	return 0;
}