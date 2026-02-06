#include "image.h"
#include <stdlib.h>
#include <time.h>
#include "vectors.h"

int main() {
    srand((unsigned int)time(NULL));
    createTrianglesSoupImage();
    
    return 0;
}
