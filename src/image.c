#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<math.h>
#include"vectors.h"
#include"image.h"
#include"model.h"

void writeImageToFile(const Config* config, float3 * image) {
    FILE* f = fopen(config->output_filename, "wb");
    if (!f) return;

    // BMP Header needs to account for padding (rows must be multiples of 4 bytes)
    int filesize = 54 + (3 * config->width + (4 - (config->width * 3) % 4) % 4) * config->height;

    unsigned char bmpFileHeader[14] = {'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0};
    unsigned char bmpInfoHeader[40] = {40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0};

    // Fill in file size
    bmpFileHeader[2] = (unsigned char)(filesize);
    bmpFileHeader[3] = (unsigned char)(filesize >> 8);
    bmpFileHeader[4] = (unsigned char)(filesize >> 16);
    bmpFileHeader[5] = (unsigned char)(filesize >> 24);

    // Fill in width and height
    bmpInfoHeader[4] = (unsigned char)(config->width);
    bmpInfoHeader[5] = (unsigned char)(config->width >> 8);
    bmpInfoHeader[6] = (unsigned char)(config->width >> 16);
    bmpInfoHeader[7] = (unsigned char)(config->width >> 24);
    bmpInfoHeader[8] = (unsigned char)(config->height);
    bmpInfoHeader[9] = (unsigned char)(config->height >> 8);
    bmpInfoHeader[10] = (unsigned char)(config->height >> 16);
    bmpInfoHeader[11] = (unsigned char)(config->height >> 24);

    fwrite(bmpFileHeader, 1, 14, f);
    fwrite(bmpInfoHeader, 1, 40, f);

    unsigned char padding[3] = {0, 0, 0};
    int paddingSize = (4 - (config->width * 3) % 4) % 4;

    // BMP stores images bottom-to-top
    for (int i = config->height - 1; i >= 0; i--) {
        for (int j = 0; j < config->width; j++) {
            // Clamp and convert float (0.0-1.0) to byte (0-255)
            // BMP uses BGR order
            unsigned char b = (unsigned char)(image[i*config->width + j].z * 255.0f);
            unsigned char g = (unsigned char)(image[i*config->width+j].y * 255.0f);
            unsigned char r = (unsigned char)(image[i*config->width+j].x * 255.0f);

            fwrite(&b, 1, 1, f);
            fwrite(&g, 1, 1, f);
            fwrite(&r, 1, 1, f);
        }
        fwrite(padding, 1, paddingSize, f);
    }

    fclose(f);
}


float2 worldToScreen(const Config* config, float3 point) {
    // Center of the screen is (float2){canvas_size // 2, canvas_size // 2}
    float screenheight_world = 3.0f;
    int pixel_per_world_unit = config->height / screenheight_world;

    float2 pixel_offset = (float2) {
        point.x * pixel_per_world_unit + config->width / 2, 
        point.y * pixel_per_world_unit + config->height / 2 
    };

    return pixel_offset;
}

void display_model(const Config* config, Model model) {

    float3* image = (float3*)malloc(config->width * config->height * sizeof(float3)); 
    int n_triangles = model.n_faces;

    float3* triangles_colors;
    triangles_colors = (float3*) malloc(n_triangles * sizeof(float3));
    // Init random triangle colors
   for(int i = 0; i < n_triangles; ++i) {
        triangles_colors[i] = getRandomFloat3();
    } 

    // Clear image
    for (int x = 0; x < config->width; x++) {
        for (int y = 0; y < config->height; y++) {
            image[y *config->width+x] = (float3){0.0f, 0.0f, 0.0f}; // Black bg
        }
    }

    triangle2 triangle;

    float3 rotation_angles = (float3) {0.5f, 0.5f, 0.0f};

    // Draw Triangles
    for (int j = 0; j < n_triangles; ++j) {

        // Get the triangle values in orthographic projection
        float3 vA = (float3) { 
                model.vertices[model.face_indices[j*3]].x,  
                model.vertices[model.face_indices[j*3]].y,  
                model.vertices[model.face_indices[j*3]].z
                };
        float3 vB = (float3) { 
                model.vertices[model.face_indices[j*3+1]].x,  
                model.vertices[model.face_indices[j*3+1]].y,  
                model.vertices[model.face_indices[j*3+1]].z
                };
        float3 vC = (float3) {  
                model.vertices[model.face_indices[j*3+2]].x, 
                model.vertices[model.face_indices[j*3+2]].y,  
                model.vertices[model.face_indices[j*3+2]].z
            };
        // Rotate triangle
        vA = rotate_vertex(vA, rotation_angles);
        vB = rotate_vertex(vB, rotation_angles);
        vC = rotate_vertex(vC, rotation_angles);

        // Project to screen coordinates
        triangle.a = worldToScreen( config,  vA  );
        triangle.b = worldToScreen( config,  vB  ) ;
        triangle.c = worldToScreen( config,  vC) ;

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
        max_x = (max_x > config->width - 1) ? config->width - 1 : max_x;
        min_y = (min_y < 0) ? 0 : min_y;
        max_y = (max_y > config->height - 1) ? config->height - 1 : max_y;

        for(int x = min_x; x <max_x; x++) {
            for(int y = min_y; y < max_y; y++) {
                float2 p = (float2) {(float)x, (float)y};
                bool inside = pointInTriangle(triangle.a, triangle.b, triangle.c, p);
                if (inside) image[y*config->width +x] = triangles_colors[j];
            }

        }
    }
    writeImageToFile(config, image);
    
    free(triangles_colors);
    free(image);
}
