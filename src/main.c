#include <stdlib.h>
#include <time.h>

#include "vectors.h"
#include "image.h"
#include "loaders/obj.h"

int main() {
    srand((unsigned int)time(NULL));
    // createTrianglesSoupImage();
    
    Model model = loader_obj("assets/cube.obj");

    display_model(model); 

    free_model(model);

    return 0;
}
