#ifndef VECTORS_H
#define VECTORS_H

#include <stdbool.h>

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

bool pointOnRightSideOfLine(struct float2 a, struct float2 b, struct float2 p);
bool pointInTriangle(struct float2 a, struct float2 b, struct float2 c, struct float2 p);
struct triangle2 getRandomTriangle(int max_x, int max_y);

#endif // VECTORS_H
