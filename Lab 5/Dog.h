#pragma once
#include "Animal.h"
#include <iostream>

using namespace std;

class Dog: public Animal{
    string name;
    int age;

public:
    Dog() {
		cout << "Creating Dog" << endl;
		name = "Dog Animal";
		age = 0;
	}
	Dog(string name, int age){
        cout<< "Creating Dog"<<endl;
        this->name = name;
        this->age = age;
    }

	~Dog() {
		cout << "Deleting Dog" << endl;
	}

	string getName() {
		return name;
	}

	void setName(string name) {
		this->name = name;
	}

	int getAge() {
		return age;
	}

	void setAge(int age) {
		this->age = age;
	}

	void feed() {
		cout << "Dog food, please!" << endl;
	}

};