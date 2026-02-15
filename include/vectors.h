#ifndef VECTORS_H
#define VECTORS_H

#include <stdbool.h>

typedef struct {
    float x;
    float y;
    float z;
} float3;

typedef struct {
    float x;
    float y;
} float2;

typedef struct {
    int x;
    int y;
} int2;

typedef struct {
    float2 a;
    float2 b;
    float2 c;
} triangle2;

bool pointOnRightSideOfLine(float2 a, float2 b, float2 p);
bool pointInTriangle(float2 a, float2 b, float2 c, float2 p);

triangle2 getRandomTriangle(int max_x, int max_y);

float2 getRandomFloat2();
float3 getRandomFloat3();

void elementwise_sum_float2(float2 a, float2 b, float2 *out);
void elementwise_sum_float3(float3 a, float3 b, float3 *out);

void get_basis_vector(float3 * pihat, float3 * pjhat, float3 * phhat);

#endif // VECTORS_H
