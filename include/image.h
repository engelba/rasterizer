#ifndef IMAGE_H
#define IMAGE_H

#include "vectors.h"
#include "model.h"
#include "config.h"

void writeImageToFile(const Config* config, float3 * image);

void display_model(const Config* config, Model model);
float2 worldToScreen(const Config* config, float3 point);

#endif // !IMAGE_H
