#pragma once
#include <string>
#include <iostream>

using namespace std;

class Entry{
private:
    string first;
    string last;
    string email;
public:
    inline void setName(string first ){this->first = first;};
	inline void setLastName(string last ){this->last = last;};
	inline void setEmail(string email ){this->email = email;};
    inline string getName(){return first;};
	inline string getLastName(){return last;};
	inline string getEmail(){return email;};
    inline void print(){cout<<"First: "<< this->first <<"\nLast: "<<this->last<<"\nEmail: "<<this->email<<endl;};


};