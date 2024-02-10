#include <iostream>
#include "Stack.h"

using namespace std;

Stack::Stack(){
    Stack::initialize();
}

Stack::Stack(int n){
    Stack::initialize();
    double counter = 0;
    for(int i = 0; i < n; i++){
        Stack::push(new double(1+counter));
        counter+=0.1;
    }
}

void Stack::Link::initialize(void* dat, Link* nxt){
    Stack::Link::data = dat;
    Stack::Link::next = nxt;
}

void Stack::initialize(){
    Stack::head = nullptr;
}

void Stack::setDeleteCallback( void (*delcb)(void* pt) ){
    Stack::deletecb = delcb;
}

void* Stack::peek(){
    return Stack::head->data;
}

void Stack::push( void* dat ){  
    Stack::Link* temp = new Stack::Link();
    temp->initialize(dat, Stack::head);
    Stack::head = temp;
}

void* Stack::pop(){
    Stack::Link* temp = Stack::head;
    Stack::head = Stack::head->next;
    void* val = temp->data;
    temp->data = nullptr;
    temp->next = nullptr;
    return val;
}

void Stack::cleanup(){
    while(Stack::head->next != nullptr){
        Stack::deletecb(Stack::pop());
    }
    Stack::deletecb(Stack::pop());
}

Stack::~Stack(){
    Stack::cleanup();
}