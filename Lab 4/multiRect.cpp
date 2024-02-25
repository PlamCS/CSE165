#include <iostream>
#include "Rect.h"
#include "Vec.h"

using namespace std;

int main(int argc, char const *argv[])
{
    /*
    -3 3 6 6 
    -2 2 4 4
    -1 1 2 2
    -1 -1 -1 -1
    */
    vector<Rect> container;
    float x, y, w, h;
    cout<<"Type 4 floats to define a rectangle: ";
    while(true){
        cin>>x;
        cin>>y;
        cin>>w;
        cin>>h;
        Rect r(x, y, w, h);
        if(x < 0 && y < 0 && w < 0 && h < 0){
            break;
        }
        container.push_back(r);
        cout<<"Type 4 floats to define the next rectangle: ";    
    }
    cout<< "Type 2 floats to define a point: ";
    while(true){
        cin>>x;
        cin>>y;
        if(x == -99.0f && y == -99.0f){
            break;
        }
        Vec w(x,y);
        for(int i = 0; i < container.size(); i++){
            
            if(container[i].contains(w)){
                cout<<"The point is INSIDE of rectangle "<< i <<endl;
            }else{
                cout<<"The point is OUTSIDE of rectangle "<< i <<endl;
            }
        }
        cout<<"Type 2 floats to define the next point: ";
    }
    cout<<"Finished!"<<endl;

    return 0;
}
