#include "random.h"
#include "vectors.h"
#include <stdbool.h>

float dot_float2(float2 a, float2 b) {
    return a.x * b.x + a.y * b.y; 
}

float2 perpendicular_float2(float2 vec) {
    float2 new_vec = {vec.y, -vec.x};
    return new_vec;
}

bool pointOnRightSideOfLine(float2 a, float2 b, float2 p) {
    float2 ap = {p.x-a.x, p.y-a.y};
    float2 ab_perp = perpendicular_float2((float2) {b.x-a.x, b.y-a.y});
    
    return  dot_float2(ap, ab_perp) >= 0.0f;
}; 
    
bool pointInTriangle(float2 a, float2 b, float2 c, float2 p) {  
    bool sideAB = pointOnRightSideOfLine(a, b, p);
    bool sideBC = pointOnRightSideOfLine(b, c, p);
    bool sideCA = pointOnRightSideOfLine(c, a, p);
    
    return sideAB  && sideBC && sideCA;
}


triangle2 getRandomTriangle(int max_x, int max_y) {
    triangle2 triangle;

    triangle.a = (float2) {getRandomFloatRange((float)0, (float)max_x), getRandomFloatRange((float)0, (float)max_x)};
    triangle.b = (float2) {getRandomFloatRange((float)0, (float)max_x), getRandomFloatRange((float)0, (float)max_y)};
    triangle.c = (float2) {getRandomFloatRange((float)0, (float)max_x), getRandomFloatRange((float)0, (float)max_y)};

    return triangle;
}

float2 getRandomFloat2() {
    float2 out;

    out = (float2) {getRandomFloatRange(0.0, 1.0), getRandomFloatRange(0.0, 1.0)};
    return out;
}

float3 getRandomFloat3() {
    float3 out;

    out = (float3) {getRandomFloatRange(0.0, 1.0), getRandomFloatRange(0.0, 1.0), getRandomFloatRange(0.0, 1.0)};
    return out;
}

void elementwise_sum_float2(float2 a, float2 b, float2 *out) {
    out->x = a.x + b.x;
    out->y = a.y + b.y;
}

void elementwise_sum_float3(float3 a, float3 b, float3 *out) {
    out->x = a.x + b.x;
    out->y = a.y + b.y;
    out->z = a.z + b.z;
}

