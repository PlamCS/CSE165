#include "Vec.h"
using namespace std;

Vec::Vec(){
    Vec::x = 0.0;
    Vec::y = 0.0;
}
Vec::Vec(float x, float y){
    Vec::x = x;
    Vec::y = y;
}
void Vec::set(float x, float y){
    Vec::x = x;
    Vec::y = y;
}
void Vec::add(Vec& u){
    Vec::x += u.x;
    Vec::y += u.y;
}
void Vec::print(){
    cout<<"("<<Vec::x<<","<<Vec::y<<")"<<endl;
}