#pragma once
#include "Entry.h"
#include <vector>

using namespace std;


class AddressBook{
private:
    vector<Entry*> book;
public:
    inline void add(Entry* e){book.push_back(e);};
    inline void print(){for(int i = 0; i < book.size(); i++){book[i]->print();}};
};