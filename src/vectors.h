#ifndef VECTORS_H
#define VECTORS_H

#include <stdbool.h>
#include "random.h"

struct float3 {
    float x;
    float y;
    float z;
};

struct float2 {
    float x;
    float y;
};

struct triangle2 {
    struct float2 a;
    struct float2 b;
    struct float2 c;
};

float dot_float2(struct float2 a, struct float2 b) {
    return a.x * b.x + a.y * b.y; 
}

struct float2 perpendicular_float2(struct float2 vec) {
    struct float2 new_vec = {vec.y, -vec.x};
    return new_vec;
}

bool pointOnRightSideOfLine(struct float2 a, struct float2 b, struct float2 p) {
    struct float2 ap = {p.x-a.x, p.y-a.y};
    struct float2 ab_perp = perpendicular_float2((struct float2) {b.x-a.x, b.y-a.y});
    
    return  dot_float2(ap, ab_perp) >= 0.0f;
}; 
    
bool pointInTriangle(struct float2 a, struct float2 b, struct float2 c, struct float2 p) {  
    bool sideAB = pointOnRightSideOfLine(a, b, p);
    bool sideBC = pointOnRightSideOfLine(b, c, p);
    bool sideCA = pointOnRightSideOfLine(c, a, p);
    
    return sideAB ==sideBC && sideBC == sideCA;
}


struct triangle2 getRandomTriangle(int max_x, int max_y) {
    struct triangle2 triangle;

    triangle.a = (struct float2) {getRandomFloatRange((float)0, (float)max_x), getRandomFloatRange((float)0, (float)max_x)};
    triangle.b = (struct float2) {getRandomFloatRange((float)0, (float)max_x), getRandomFloatRange((float)0, (float)max_y)};
    triangle.c = (struct float2) {getRandomFloatRange((float)0, (float)max_x), getRandomFloatRange((float)0, (float)max_y)};

    return triangle;
}
#endif
