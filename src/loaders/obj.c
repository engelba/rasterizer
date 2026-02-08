#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "loaders/obj.h"
#include "vectors.h"
#include "model.h"

Model loader_obj(const char* filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        fprintf(stderr, "Error: Could not open %s for reading\n", filename);
        exit(1);
    }

    Model model;
    char * buffer = (char *) malloc( MAX_LENGTH );

    // Fist pass on data to allocate memory
    int vertices_count = 0;
    int faces_counts = 0;
    while ( fgets( buffer, MAX_LENGTH, fp ) ) {
        
        if ( ferror( fp ) ) {
            fprintf( stderr, "Reading error with code %d\n", errno );
            break;
        }
        if (buffer[0] == 'v' && buffer[1] == ' ')
            vertices_count += 1;
        else if (buffer[0] == 'f' && buffer[1] == ' ') {
            faces_counts += 1;
        }
    }

    printf("vertices: %d | face: %d\n", vertices_count, faces_counts);

    // Allocate vertices
    model.n_vertices = vertices_count;
    model.vertices = (float3*) malloc(vertices_count * sizeof(float3));
    if (model.vertices == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(1);
    }
    // Allocate faces
    model.n_faces = faces_counts;
    model.face_indices = (int*) malloc( 3 * faces_counts * sizeof(int) );
    if (model.face_indices == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        exit(1);
    }

    // Second pass on file to actually get the infos
    rewind(fp); // restart the stream pointer at the top of the file

    int v_count = 0;
    float vert1, vert2, vert3; // To store vertices values for vertices

    int f_count = 0;
    int v1, v2, v3; // To store vertices index for faces 

    while ( fgets( buffer, MAX_LENGTH, fp ) ) {
        if ( ferror( fp ) ) {
            fprintf( stderr, "Reading error with code %d\n", errno );
            break;
        }

        if (buffer[0] == 'v' && buffer[1] == ' ') {
            sscanf(buffer, "v %f %f %f", &vert1, &vert2, &vert3);
            model.vertices[v_count] = (float3) {vert1, vert2, vert3};
            v_count += 1;
        }
        else if (buffer[0] == 'f' && buffer[1] == ' ') {
            // Reads the first integer from then skip to next whitespace (skipping "/")
            sscanf(buffer, "f %d%*[^ ] %d%*[^ ] %d%*[^ ]", &v1, &v2, &v3);
            model.face_indices[f_count]   = v1 - 1;
            model.face_indices[f_count+1] = v2 - 1;
            model.face_indices[f_count+2] = v3 - 1;
            f_count += 3;
        }
    }

    free(buffer);
    fclose(fp);

    return model;
}

