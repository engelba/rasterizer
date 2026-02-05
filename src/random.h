#ifndef RANDOM_H
#define RANDOM_H

#include <stdlib.h>
#include <time.h>

float getRandomFloatRange(float min, float max) {
    float scale = (float)rand() / (float)RAND_MAX;
    return min + scale * (max-min);
}

#endif // !RANDOM
