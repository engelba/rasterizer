#ifndef MODEL_H
#define MODEL_H

#include "vectors.h"

typedef struct {
    float3* vertices;
    int n_vertices;

    int* face_indices;
    int n_faces;
} Model;

void free_model(Model model);

#endif /* MODEL_H */
