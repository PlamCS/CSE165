#pragma once

class Object{
public:
    int x;
    static inline int count;
    
    Object();
    Object(int val);
    Object(Object &old);
    Object& operator=(Object &old);
};