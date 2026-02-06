#include <stdlib.h>
#include <time.h>
#include "random.h"

float getRandomFloatRange(float min, float max) {
    float scale = (float)rand() / (float)RAND_MAX;
    return min + scale * (max-min);
}

