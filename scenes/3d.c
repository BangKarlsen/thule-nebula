#include <string.h> // memset
#include <proto/exec.h> // AllocMem, FreeMem
#include "linedraw.h"
#include "../system/logger.h"
#include "../data/cube2.obj.h"
#include "3d.h"

#define FPMUL(x,y) ((((x)>>6)*((y)>>6))>>4)  // Using 16:16 fixed point. Largest number w/o overflow is 6:10
#define FPDIV(x,y) ((((x)<<6)/((y)>>6))>>4)
#define FPP 16

#define SCREEN_W 320
#define SCREEN_H 176
#define SCREEN_HALF_W 160
#define SCREEN_HALF_H 88

UBYTE *back_3d = NULL;

void init_3d()
{
    back_3d = AllocMem(SCREEN_W * SCREEN_H, MEMF_FAST);
    if (!back_3d)
    {
        SERIAL_LOG("Could not allocate back_3d  >:|")
    }
}

unsigned char rotx = 0;
unsigned char roty = 0;
unsigned char rotz = 0;

void tick_3d(char *screen)
{
    int scrx[num_verts];
    int scry[num_verts];
    int mat[4][4]; // The rotation matrix
    int sx,sy,sz,cx,cy,cz;

    rotx += 1;
    roty += 2;
    rotz += 1;

    sx = sin256[rotx]<<8; // Yhm, why was this again?? Why not << 16? Maybe twas already scaled <<8 once or something?
    sy = sin256[roty]<<8;
    sz = sin256[rotz]<<8;
    cx = cos256[rotx]<<8;
    cy = cos256[roty]<<8;
    cz = cos256[rotz]<<8;

    mat[0][0] =  FPMUL(cx,cz) + FPMUL(FPMUL(sx,sy),sz);
    mat[1][0] = -FPMUL(cx,sz) + FPMUL(FPMUL(cx,sx),sy);    
    mat[2][0] =  FPMUL(cy,sx);
    mat[0][1] =  FPMUL(cy,sz);                
    mat[1][1] =  FPMUL(cy,cz);                
    mat[2][1] = -sy;
    mat[0][2] = -FPMUL(cz,sx) + FPMUL(FPMUL(cx,sy),sz);
    mat[1][2] =  FPMUL(sx,sz) + FPMUL(FPMUL(cx,cz),sy);    
    mat[2][2] =  FPMUL(cx,cy);

    memset(screen, 0, SCREEN_W * SCREEN_H); // clear screen
    //memcpy(screen, back_3d, SCREEN_W * SCREEN_H);     // draw background

    const int z_offset = 10;
    // Rotate and Perspective
    for(int i = 0; i < num_verts; i++)
    {
        int resx = FPMUL(v[i].x,mat[0][0]) + FPMUL(v[i].y,mat[1][0]) + FPMUL(v[i].z,mat[2][0]);
        int resy = FPMUL(v[i].x,mat[0][1]) + FPMUL(v[i].y,mat[1][1]) + FPMUL(v[i].z,mat[2][1]);
        int resz = FPMUL(v[i].x,mat[0][2]) + FPMUL(v[i].y,mat[1][2]) + FPMUL(v[i].z,mat[2][2]) + (z_offset<<FPP);

        //MAKE PERSPECTIVE 
        scrx[i] = FPDIV(resx, resz); // Mult with 1/x table (32k) instead of div
        scry[i] = FPDIV(resy, resz);

        // Convert to 32:0, Scale and Place
        scrx[i] = SCREEN_HALF_W + (scrx[i]);    
        scry[i] = SCREEN_HALF_H + (scry[i]);
    }

    // DRAW OBJECT
    for(int i = 0; i < num_faces; i++)
    {
        char vi0 = f[i].vertexIndices[0] - 1;
        char vi1 = f[i].vertexIndices[1] - 1;
        char vi2 = f[i].vertexIndices[2] - 1;
        char vi3 = f[i].vertexIndices[3] - 1;

        linedraw(scrx[vi0], scry[vi0], scrx[vi1], scry[vi1], 55, screen);
        linedraw(scrx[vi1], scry[vi1], scrx[vi2], scry[vi2], 55, screen);
        linedraw(scrx[vi2], scry[vi2], scrx[vi3], scry[vi3], 55, screen);
        linedraw(scrx[vi3], scry[vi3], scrx[vi0], scry[vi0], 55, screen);
    }
}    

void deinit_3d()
{
    if(back_3d)
    {
        SERIAL_LOG("deinit_3d\n")
        FreeMem(back_3d, SCREEN_W * SCREEN_H);
    }
}
