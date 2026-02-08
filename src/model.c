#include "model.h"
#include <stdlib.h>

void free_model(Model model) {
    if (model.vertices != NULL) {
        free(model.vertices);
    }
    
    if (model.face_indices != NULL) {
        free(model.face_indices);
    }
}

