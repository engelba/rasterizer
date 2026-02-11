#ifndef IMAGE_H
#define IMAGE_H

#include "vectors.h"
#include "model.h"

void writeImageToFile(int width, int height, float3 * image, const char* filename);
void createTestImage();
void createTrianglesSoupImage();
void display_model(Model model);

float2 worldToScreen(float3 point, int2 canvas_size);

#endif // !IMAGE_H
