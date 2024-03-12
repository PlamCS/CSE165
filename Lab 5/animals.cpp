#include <iostream>
#include "Dog.h"

using namespace std;

int main(int argc, const char * argv[])
{
	Dog myDog = Dog("Snoopy", 4);

	cout << myDog.getName() << " is " << myDog.getAge() << " years old.\n";

	myDog.feed();

	return 0;
}