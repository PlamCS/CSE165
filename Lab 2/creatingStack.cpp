#include <iostream>
#include "Stack.h"

using namespace std;

int main(){
    Stack* s = new Stack;
    int n;
    cin>>n;
    double value;
    for(int i = 0; i < n; i++){
        cin>>value;
        s->push(new double(value));
    }

    //Pop displaying or peek display after a pop will work perfectly fine too.
    for(int i = 0; i < n; i++){
        cout<<*(double*)s->pop()<<endl;
    }
    s->cleanup();
    
    return 0;
}
