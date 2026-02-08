#include <stdio.h>
#include <stdbool.h>
#include "vectors.h"
#include "image.h"

void writeImageToFile(int width, int height, float3 image[height][width], const char* filename) {
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
            unsigned char b = (unsigned char)(image[i][j].z * 255.0f);
            unsigned char g = (unsigned char)(image[i][j].y * 255.0f);
            unsigned char r = (unsigned char)(image[i][j].x * 255.0f);

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

     float3 image[height][width];

    float2 a = {0.2f * height, 0.2f * width};
    float2 b = {0.4f * height, 0.7f * width};
    float2 c = {0.8f * height, 0.4f * width};

    for(int x = 0; x <width; x++) {
        for(int y = 0; y < height; y++) {
            float2 p = (float2) {(float)x, (float)y};
            bool inside = pointInTriangle(a, b, c, p) ;
            if (inside) image[y][x] = (float3) {0.0f, 0.0f, 1.0f};
            else image[y][x] = (float3){0.0f, 0.0f, 0.0f}; // 2. Explicit Black Background
            
        }
    }

        writeImageToFile(width, height, image, "out.bmp");
}


void createTrianglesSoupImage() {
    int width = 512;
    int height = 512;
    char filename[64];

    float3 image[height][width];
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
                image[y][x] = (float3){0.0f, 0.0f, 0.0f}; // Black bg
            }
        }

        // Draw Triangles
        for (int j = 0; j < n_triangles; ++j) {
            for(int x = 0; x <width; x++) {
                for(int y = 0; y < height; y++) {
                    float2 p = (float2) {(float)x, (float)y};
                    bool inside = pointInTriangle(triangles[j].a, triangles[j].b, triangles[j].c, p) ;
                    if (inside) image[y][x] = triangle_colors[j];
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
