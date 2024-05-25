#ifndef OUTPUT_H
#define OUTPUT_H

int num_verts = 8;
int num_faces = 6;

struct Vertex {
    int x;
    int y;
    int z;
};

struct Vertex v[] = {
    { 0, 131072, 131072 },
    { 0, 0, 131072 },
    { 131072, 0, 131072 },
    { 131072, 131072, 131072 },
    { 0, 131072, 0 },
    { 0, 0, 0 },
    { 131072, 0, 0 },
    { 131072, 131072, 0 },
};

struct Face {
    int vertexIndices[4];
};

struct Face f[] = {
    { 1, 2, 3, 4 },
    { 8, 7, 6, 5 },
    { 4, 3, 7, 8 },
    { 5, 1, 4, 8 },
    { 5, 6, 2, 1 },
    { 2, 6, 7, 3 },
};

#endif // OUTPUT_H
