#include <string.h> // memset
#include <proto/exec.h> // AllocMem, FreeMem
#include "linedraw.h"
#include "../system/logger.h"
#include "3d.h"

#define FPMUL(x,y) ((((x)>>6)*((y)>>6))>>4)  // Using 16:16 fixed point. Largest number w/o overflow is 6:10
#define FPDIV(x,y) ((((x)<<6)/((y)>>6))>>4)
#define FPP 16

#define SCREEN_W 320
#define SCREEN_H 176
#define SCREEN_HALF_W 160
#define SCREEN_HALF_H 88

UBYTE    *back_3d = NULL;

// Cube
#define NUM_VERT 8
#define NUM_FACES 6

struct Vertices
{
    int x[NUM_VERT];
    int y[NUM_VERT];
    int z[NUM_VERT];
};

struct Vertices v;

struct Faces
{
    int vi[4];
};

struct Faces f[NUM_FACES];

void init_3d()
{
    UBYTE i;

    v.x[0] = 10<<FPP; v.y[0] = 72<<FPP; v.z[0] = 2<<FPP;
    v.x[1] = 10<<FPP; v.y[1] = 60<<FPP; v.z[1] = 2<<FPP;
    v.x[2] = 20<<FPP; v.y[2] = 80<<FPP; v.z[2] = 20<<FPP;
    v.x[3] = 20<<FPP; v.y[3] = 42<<FPP; v.z[3] = 2<<FPP;
    v.x[4] = 30<<FPP; v.y[4] = 20<<FPP; v.z[4] = 10<<FPP;
    v.x[5] = 60<<FPP; v.y[5] = 40<<FPP; v.z[5] = 60<<FPP;
    v.x[6] = 32<<FPP; v.y[6] = 40<<FPP; v.z[6] = 30<<FPP;
    v.x[7] = 72<<FPP; v.y[7] = 20<<FPP; v.z[7] = 30<<FPP;

    f[0].vi[0] = 1; f[0].vi[1] = 2; f[0].vi[2] = 3; f[0].vi[3] = 4;
    f[1].vi[0] = 8; f[1].vi[1] = 7; f[1].vi[2] = 6; f[1].vi[3] = 5;
    f[2].vi[0] = 4; f[2].vi[1] = 3; f[2].vi[2] = 7; f[2].vi[3] = 8;
    f[3].vi[0] = 5; f[3].vi[1] = 1; f[3].vi[2] = 4; f[3].vi[3] = 8;
    f[4].vi[0] = 5; f[4].vi[1] = 6; f[4].vi[2] = 2; f[4].vi[3] = 1;
    f[5].vi[0] = 2; f[5].vi[1] = 6; f[5].vi[2] = 7; f[5].vi[3] = 3;

    back_3d = AllocMem(320 * 176, MEMF_FAST);
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
    UBYTE i;
    int resx[NUM_VERT];
    int resy[NUM_VERT];
    int resz[NUM_VERT];
    int scrx[NUM_VERT];
    int scry[NUM_VERT];
    int mat[4][4]; // The rotation matrix
    int sx,sy,sz,cx,cy,cz;

    rotx += 1;
    roty += 2;
    rotz += 1;

    sx = sin256[rotx]<<8;
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
    //memcpy(screen, back_3d, 320*256); // draw background

    // Rotate and Perspective
    for(i = 0; i < NUM_VERT; i++)
    {
        resx[i] = FPMUL(v.x[i],mat[0][0]) + FPMUL(v.y[i],mat[1][0]) + FPMUL(v.z[i],mat[2][0]);
        resy[i] = FPMUL(v.x[i],mat[0][1]) + FPMUL(v.y[i],mat[1][1]) + FPMUL(v.z[i],mat[2][1]);
        resz[i] = FPMUL(v.x[i],mat[0][2]) + FPMUL(v.y[i],mat[1][2]) + FPMUL(v.z[i],mat[2][2]) + (256<<FPP);

        //MAKE PERSPECTIVE 
        scrx[i] = FPDIV(resx[i], resz[i]); // Mult with 1/x table (32k) instead of div
        scry[i] = FPDIV(resy[i], resz[i]);

        // Convert to 32:0, Scale and Place
        scrx[i] = SCREEN_HALF_W + (scrx[i]);    
        scry[i] = SCREEN_HALF_H + (scry[i]);
    }

    // DRAW OBJECT
    for(i = 0; i < NUM_FACES; i++)
    {
        char vi0 = f[i].vi[0] - 1;
        char vi1 = f[i].vi[1] - 1;
        char vi2 = f[i].vi[2] - 1;
        char vi3 = f[i].vi[3] - 1;

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
