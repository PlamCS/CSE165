#include "Rect.h"
#include "Vec.h"

using namespace std;

Rect::Rect(){
    Rect::v;
    Rect::u;
}
Rect::Rect(float x, float y, float w, float h){
    Rect::v.set(x, y);
    Rect::u.set(w, h);
}
bool Rect::contains(Vec w){
    return ((w.x >= Rect::v.x && w.y <= Rect::v.y) && (w.x <= Rect::v.x + Rect::u.x && w.y >= Rect::v.y - Rect::u.y));
}