#ifndef IMAGE_H
#define IMAGE_H

#include "vectors.h"

void writeImageToFile(int width, int height, float3 image[height][width], const char* filename);
void createTestImage();
void createTrianglesSoupImage();

#endif // !IMAGE_H
