#include <iostream>
#include "AddressBook.h"

using namespace std;

int main()
{
	int n;
	cin >> n;

	AddressBook* myAddressBook = new AddressBook;

	string name, lastname, email;

	for( int i = 0; i < n; ++i )
	{
		cin >> name;
		cin >> lastname;
		cin >> email;

		Entry* e = new Entry;
		e->setName( name );
		e->setLastName( lastname );
		e->setEmail( email );

		myAddressBook->add( e );
	}

	myAddressBook->print();

	return 0;
}
