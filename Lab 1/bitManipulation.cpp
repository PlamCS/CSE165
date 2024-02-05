#include <iostream>

using namespace std;

void printBits(int n){
    int i = 31;
    while(!((n >> i)&1)){
        i--;
    }
    for(i;i >= 0; i--){
        cout<<((n >> i)&1);
    }
    cout<<endl;
}

int getBit(int n, int index){
    return ((n >> index)&1);
}

int setBit(int n, int index){
    n |= (1 << index);
    return n;
}

int clearBit(int n, int index){
    n &= ~(1 << index);
    return n;
}

int main(int argc, char const *argv[])
{
    int n;
    int index;

    cin>>n;
    cin>>index;

    cout<<"Binary representation of "<< n << " is ";
    printBits(n);
    cout<<"Get bit at index " << index <<": "<<getBit(n,index)<<endl;
    cout<<"Binary number after setting bit at index "<< index <<": ";
    printBits(setBit(n, index));
    cout<<"Binary number after clearing bit at index " << index <<": ";
    printBits(clearBit(n, index));
    
    return 0;
}
