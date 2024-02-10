#pragma once

struct Stack {
	struct Link {
		void* data;
		Link* next;
		
		void initialize( void* dat, Link* nxt );
	}* head;
	void (*deletecb)(void* pt);

	Stack();
	Stack(int n);
	void setDeleteCallback( void (*delcb)(void* pt) );
	void initialize();
	void cleanup();
	void push( void* dat );
	void* peek();
	void* pop();
	~Stack();
};
