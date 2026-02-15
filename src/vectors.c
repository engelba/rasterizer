#include "random.h"
#include "vectors.h"
#include <stdbool.h>
#include <math.h>

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

float3 rotate_vertex(float3 v, float3 angle) {
    float3 rotated = v;

    // X axis rotation
    float cos_x = cosf(angle.x);
    float sin_x = sinf(angle.x);
    float y_temp = rotated.y * cos_x - rotated.z * sin_x;
    float z_temp = rotated.y * sin_x + rotated.z * cos_x;
    rotated.y = y_temp;
    rotated.z = z_temp;

    // y axis rotation
    float cos_y = cosf(angle.y);
    float sin_y = sinf(angle.y);
    float x_temp = rotated.x * cos_y + rotated.z * sin_y;
    z_temp = -rotated.x * sin_y + rotated.z * cos_y;
    rotated.x = x_temp;
    rotated.z = z_temp;

    // z axis rotation
    float cos_z = cosf(angle.z);
    float sin_z = sinf(angle.z);
    x_temp = rotated.x * cos_z - rotated.y * sin_z;
    y_temp = rotated.x * sin_z + rotated.y * cos_z;
    rotated.x = x_temp;
    rotated.y = y_temp;

    return rotated;
}
