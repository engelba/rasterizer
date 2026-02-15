#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<math.h>
#include"vectors.h"
#include"image.h"
#include"model.h"

void writeImageToFile(int width, int height, float3* image, const char* filename) {
    FILE* f = fopen(filename, "wb");
    if (!f) return;

    // BMP Header needs to account for padding (rows must be multiples of 4 bytes)
    int filesize = 54 + (3 * width + (4 - (width * 3) % 4) % 4) * height;

    unsigned char bmpFileHeader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
    unsigned char bmpInfoHeader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};

    // Fill in file size
    bmpFileHeader[2] = (unsigned char)(filesize);
    bmpFileHeader[3] = (unsigned char)(filesize >> 8);
    bmpFileHeader[4] = (unsigned char)(filesize >> 16);
    bmpFileHeader[5] = (unsigned char)(filesize >> 24);

    // Fill in width and height
    bmpInfoHeader[4] = (unsigned char)(width);
    bmpInfoHeader[5] = (unsigned char)(width >> 8);
    bmpInfoHeader[6] = (unsigned char)(width >> 16);
    bmpInfoHeader[7] = (unsigned char)(width >> 24);
    bmpInfoHeader[8] = (unsigned char)(height);
    bmpInfoHeader[9] = (unsigned char)(height >> 8);
    bmpInfoHeader[10] = (unsigned char)(height >> 16);
    bmpInfoHeader[11] = (unsigned char)(height >> 24);

    fwrite(bmpFileHeader, 1, 14, f);
    fwrite(bmpInfoHeader, 1, 40, f);

    unsigned char padding[3] = {0, 0, 0};
    int paddingSize = (4 - (width * 3) % 4) % 4;

    // BMP stores images bottom-to-top
    for (int i = height - 1; i >= 0; i--) {
        for (int j = 0; j < width; j++) {
            // Clamp and convert float (0.0-1.0) to byte (0-255)
            // BMP uses BGR order
            unsigned char b = (unsigned char)(image[i*width + j].z * 255.0f);
            unsigned char g = (unsigned char)(image[i*width+j].y * 255.0f);
            unsigned char r = (unsigned char)(image[i*width+j].x * 255.0f);

            fwrite(&b, 1, 1, f);
            fwrite(&g, 1, 1, f);
            fwrite(&r, 1, 1, f);
        }
        fwrite(padding, 1, paddingSize, f);
    }

    fclose(f);
}

void createTestImage() {
    int width = 64;
    int height = 64;

    float3* image;
    image = (float3*)malloc(width * height * sizeof(float3)); 

    float2 a = {0.2f * height, 0.2f * width};
    float2 b = {0.4f * height, 0.7f * width};
    float2 c = {0.8f * height, 0.4f * width};

    for(int x = 0; x <width; x++) {
        for(int y = 0; y < height; y++) {
            float2 p = (float2) {(float)x, (float)y};
            bool inside = pointInTriangle(a, b, c, p) ;
            if (inside) image[y*width +x] = (float3) {0.0f, 0.0f, 1.0f};
            else image[y*width+x] = (float3){0.0f, 0.0f, 0.0f}; // 2. Explicit Black Background
            
        }
    }

        writeImageToFile(width, height, image, "out.bmp");
}


void createTrianglesSoupImage() {
    int width = 512;
    int height = 512;
    char filename[64];

    float3* image;
    image = (float3*)malloc(width * height * sizeof(float3)); 

    int n_triangles = 10;
    float max_triangle_speed = 10.0;

    triangle2 triangles[n_triangles];
    float3 triangle_colors[n_triangles];
    float2 triangles_velocities[n_triangles];
    
    for(int i = 0; i < n_triangles; ++i) {
        triangles[i] = getRandomTriangle((float)width, (float)height);
        triangle_colors[i] = getRandomFloat3();
        triangles_velocities[i] = getRandomFloat2();
        triangles_velocities[i].x = (triangles_velocities[i].x * 2 * max_triangle_speed - max_triangle_speed);
        triangles_velocities[i].y = (triangles_velocities[i].y * 2 * max_triangle_speed - max_triangle_speed);
    }

    for (int i = 0; i < 20; ++i){
        // Clear image
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                image[y*width +x] = (float3){0.0f, 0.0f, 0.0f}; // Black bg
            }
        }

        // Draw Triangles
        for (int j = 0; j < n_triangles; ++j) {
            for(int x = 0; x <width; x++) {
                for(int y = 0; y < height; y++) {
                    float2 p = (float2) {(float)x, (float)y};
                    bool inside = pointInTriangle(triangles[j].a, triangles[j].b, triangles[j].c, p) ;
                    if (inside) image[y*width + x] = triangle_colors[j];
                }
            }

            // Move triangle by velocity
            triangles[j].a.x += triangles_velocities[j].x;
            triangles[j].a.y += triangles_velocities[j].y;

            triangles[j].b.x += triangles_velocities[j].x;
            triangles[j].b.y += triangles_velocities[j].y;

            triangles[j].c.x += triangles_velocities[j].x;
            triangles[j].c.y += triangles_velocities[j].y;
            
        }
        snprintf(filename, sizeof(filename), "out/out_%02d.bmp", i);
        writeImageToFile(width, height, image, filename);
    }
}


float2 worldToScreen(float3 point, int2 canvas_size) {
    // Center of the screen is (float2){canvas_size // 2, canvas_size // 2}
    float screenheight_world = 5.0f;
    int pixel_per_world_unit = canvas_size.y / screenheight_world;

    float2 pixel_offset = (float2) {point.x * pixel_per_world_unit + canvas_size.x / 2, point.y * pixel_per_world_unit + canvas_size.y / 2 };

    return pixel_offset;
}

void display_model(Model model) {
    int width = 512;
    int height = 512;

    float3* image = (float3*)malloc(width * height * sizeof(float3)); 
    int n_triangles = model.n_faces;

    float3* triangles_colors;
    triangles_colors = (float3*) malloc(n_triangles * sizeof(float3));
    // Init random triangle colors
   for(int i = 0; i < n_triangles; ++i) {
        triangles_colors[i] = getRandomFloat3();
    } 

    // Clear image
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            image[y *width+x] = (float3){0.0f, 0.0f, 0.0f}; // Black bg
        }
    }

    triangle2 triangle;

    // Draw Triangles
    for (int j = 0; j < n_triangles; ++j) {

        // Get the triangle values in orthographic projection
        triangle.a = worldToScreen( 
            (float3) { 
                model.vertices[model.face_indices[j*3]].x,  
                model.vertices[model.face_indices[j*3]].y,  
                model.vertices[model.face_indices[j*3]].z
                }, 
            (int2) {width, height} ) ;
    triangle.b =  worldToScreen( 
            (float3) { 
                model.vertices[model.face_indices[j*3+1]].x,  
                model.vertices[model.face_indices[j*3+1]].y,  
                model.vertices[model.face_indices[j*3+1]].z
                }, 
            (int2) {width, height}) ;
    triangle.c =  worldToScreen( 
            (float3) { 
                model.vertices[model.face_indices[j*3+2]].x, 
                model.vertices[model.face_indices[j*3+2]].y,  
                model.vertices[model.face_indices[j*3+2]].z
            }, 
            (int2) {width, height}) ;

        printf("A: x %f y %f\n" , triangle.a.x, triangle.a.y);
        printf("B: x %f y %f\n" , triangle.b.x, triangle.b.y);
        printf("C: x %f y %f\n" , triangle.c.x, triangle.c.y);

        // Perform culling
        float area = (triangle.b.x - triangle.a.x) * (triangle.c.y - triangle.a.y) - 
                    (triangle.b.y - triangle.a.y) * (triangle.c.x - triangle.a.x);
        if (fabs(area) < 0.1f) continue;

        // Find triangle bouding box
        int min_x = (int)fmin(triangle.a.x, fmin(triangle.b.x, triangle.c.x));
        int max_x = (int)fmax(triangle.a.x, fmax(triangle.b.x, triangle.c.x));
        int min_y = (int)fmin(triangle.a.y, fmin(triangle.b.y, triangle.c.y));
        int max_y = (int)fmax(triangle.a.y, fmax(triangle.b.y, triangle.c.y));

        // Clamp the bounding boxes
        min_x = (min_x < 0) ? 0 : min_x;
        max_x = (max_x > width - 1) ? width - 1 : max_x;
        min_y = (min_y < 0) ? 0 : min_y;
        max_y = (max_y > height - 1) ? height - 1 : max_y;

        for(int x = min_x; x <max_x; x++) {
            for(int y = min_y; y < max_y; y++) {
                float2 p = (float2) {(float)x, (float)y};
                bool inside = pointInTriangle(triangle.a, triangle.b, triangle.c, p);
                if (inside) image[y*width +x] = triangles_colors[j];
            }

        }
    }
    writeImageToFile(width, height, image,  "out/out.bmp");
    
    free(triangles_colors);
    free(image);

}
