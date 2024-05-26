#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_NUM_VERTS 800     // that'll trip someone up at some point..
#define MAX_NUM_FACES 600     // ditto

int num_verts = 0;
int num_faces = 0;

struct Vertex {
    int x[MAX_NUM_VERTS];
    int y[MAX_NUM_VERTS];
    int z[MAX_NUM_VERTS];
};

struct Vertex v;

struct Face {
    int vertexIndices[4]; 
};

struct Face f[MAX_NUM_FACES];

// Function to parse .obj file
int load_obj(char* filename)
{
    FILE* file = fopen(filename, "r");
    if(file == NULL){
        printf("Failed to open file [%s]!\n", filename);
        return -1;
    }

    char lineheader[128];
    int res;
    while(1){
        res = fscanf(file, "%s", lineheader);
        if(res == EOF){
            printf("done parsing file: %s\n", filename);
            break;
        }
        // Process the line based on lineheader
        if(strcmp(lineheader, "v") == 0){
            // Store vertex data
            float x, y, z;
            fscanf(file, "%f %f %f\n", &x, &y, &z);
            v.x[num_verts] = round(x * 65536);
            v.y[num_verts] = round(y * 65536);
            v.z[num_verts] = round(z * 65536);
            num_verts++;
        } else if(strcmp(lineheader, "f") == 0){
            int indices[4];
            fscanf(file, "%d %d %d %d\n", &indices[0], &indices[1], &indices[2], &indices[3]);
            f[num_faces].vertexIndices[0] = indices[0];
            f[num_faces].vertexIndices[1] = indices[1];
            f[num_faces].vertexIndices[2] = indices[2];
            f[num_faces].vertexIndices[3] = indices[3];
            num_faces++;
        }
    }

    fclose(file);
    return 0;
}

// Function to output C header file
int write_h(char* outfilename) {
    FILE* headerFile = fopen(outfilename, "w");
    if(headerFile == NULL){
        printf("Failed to open output file [%s]!\n", outfilename);
        return -1;
    }

    fprintf(headerFile, "#ifndef OUTPUT_H\n");
    fprintf(headerFile, "#define OUTPUT_H\n");
    fprintf(headerFile, "\n");
    fprintf(headerFile, "int num_verts = %d;\n", num_verts);
    fprintf(headerFile, "int num_faces = %d;\n", num_faces);
    fprintf(headerFile, "\n");

    fprintf(headerFile, "struct Vertex {\n");
    fprintf(headerFile, "    int x;\n");
    fprintf(headerFile, "    int y;\n");
    fprintf(headerFile, "    int z;\n");
    fprintf(headerFile, "};\n");
    fprintf(headerFile, "\n");

    fprintf(headerFile, "struct Vertex v[] = {\n");
    for(int i = 0; i < num_verts; i++)
    {
        fprintf(headerFile, "    { %d, %d, %d },\n", v.x[i], v.y[i], v.z[i]);
    }
    fprintf(headerFile, "};\n");
    fprintf(headerFile, "\n");
    
    fprintf(headerFile, "struct Face {\n");
    fprintf(headerFile, "    int vertexIndices[4];\n");
    fprintf(headerFile, "};\n");
    fprintf(headerFile, "\n");

    fprintf(headerFile, "struct Face f[] = {\n");
    for(int i = 0; i < num_faces; i++){
        fprintf(headerFile, "    { %d, %d, %d, %d },\n", f[i].vertexIndices[0], f[i].vertexIndices[1], f[i].vertexIndices[2], f[i].vertexIndices[3]);
    }
    fprintf(headerFile, "};\n");
    fprintf(headerFile, "\n");

    fprintf(headerFile, "#endif // OUTPUT_H\n");

    fclose(headerFile);
    return 0;
}


// TODO:
// Handle negative numbers
// Handle eating obj straight outta blender 

int main(int argc, char *argv[]) {
    if(argc < 2) {
        printf("ERROR! objeater needs somthing to eat! Add obj file as input argument 💃\n");
        return -1;
    }

    if(load_obj(argv[1])){
        return -1;
    }

    char out_file[255];
    sprintf(out_file, "%s.h", argv[1]);
    printf("Writing to: %s\n", out_file);

    if(write_h(out_file)){
        return -1;
    }

    return 0;
}
