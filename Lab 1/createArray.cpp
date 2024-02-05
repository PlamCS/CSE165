#include <iostream>
#include "createArray.h"

using namespace std;

int fibVal(int n){
    int x = 0;
    int y = 1;
    int z = 0;
    if (n == 0)
        return x;
    for(int i = 2; i <= n; i++){
        z = x + y;
        x = y;
        y = z;
    }
    return y;
}

void createArray(int size, int*& array){
    if(size < 1){
        cout<<"ERROR: size of array < 1"<<endl;
        return;
    }

    array = new int[size];
    for(int i = 0; i < size; i++){
        array[i] = fibVal(i);
    }

}

void printArray(int size, int* array){
    if(array == nullptr){
        cout<<"Invalid array."<<endl;
        return;
    }
    
    for(int i = 0; i < size; i++){
        cout<<array[i]<<" ";
    }
    cout<<endl;
}
