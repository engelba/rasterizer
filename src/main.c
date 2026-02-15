#include <stdlib.h>
#include <time.h>

#include "vectors.h"
#include "image.h"
#include "loaders/obj.h"
#include "config.h"

int main() {
    srand((unsigned int)time(NULL));
    // createTrianglesSoupImage();
    
    Model model = loader_obj("assets/cube.obj");
    Config config = {
        .width = 100,
        .height = 600,
        .output_filename = "out/render.bmp"
    };

    display_model(&config, model); 

    free_model(model);

    return 0;
}
