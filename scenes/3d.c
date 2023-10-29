#include <string.h>
#include <proto/exec.h> // AllocMem, FreeMem
                        //#include <clib/graphics_protos.h>
                        //#include <clib/intuition_protos.h>
                        //#include <clib/dos_protos.h>
                        //#include <math.h>
#include "linedraw.h"
#include "../system/logger.h"
//#include "sinecosine.h"
#include "3d.h"

#define FPMUL(x,y) ((((x)>>6)*((y)>>6))>>4)  // Using 16:16 fixed point. Largest number w/o overflow is 6:10
#define FPDIV(x,y) ((((x)<<6)/((y)>>6))>>4)
#define FPP 16
#define NRPOINTS 8

#define SCREEN_W 320
#define SCREEN_H 176
#define SCREEN_HALF_W 160
#define SCREEN_HALF_H 88

UBYTE    *back_3d = NULL;
int      x[NRPOINTS],
         y[NRPOINTS],
         z[NRPOINTS];

void init_3d()
{
    UBYTE i;

    //GENERATE COORDINATES (A Box)
    for(i = 0; i < NRPOINTS; i++)
    {
        x[i] = (50-100*(((i+1)/2)%2))<<FPP;
        y[i] = (50-100*((i/2)%2))<<FPP;
        z[i] = (50-100*((i/4)%2))<<FPP;
    }

    back_3d = AllocMem(320 * 176, MEMF_FAST);
    if (!back_3d)
    {
        SERIAL_LOG("Could not allocate back_3d  >:|")
    }
}

unsigned char rotx = 0;
unsigned char roty = 0;
unsigned char rotz = 0;

void tick_3d_unroll(char *screen)
{
    int resx[NRPOINTS],
    resy[NRPOINTS],
    resz[NRPOINTS],
    scrx[NRPOINTS],
    scry[NRPOINTS];
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


    resx[0] = FPMUL(x[0],mat[0][0]) + FPMUL(y[0],mat[1][0]) + FPMUL(z[0],mat[2][0]);
    resy[0] = FPMUL(x[0],mat[0][1]) + FPMUL(y[0],mat[1][1]) + FPMUL(z[0],mat[2][1]);
    resz[0] = FPMUL(x[0],mat[0][2]) + FPMUL(y[0],mat[1][2]) + FPMUL(z[0],mat[2][2]) + (256<<FPP);
    scrx[0] = FPDIV(resx[0], resz[0])+SCREEN_HALF_W;
    scry[0] = FPDIV(resy[0], resz[0])+SCREEN_HALF_H;

    resx[1] = FPMUL(x[1],mat[0][0]) + FPMUL(y[1],mat[1][0]) + FPMUL(z[1],mat[2][0]);
    resy[1] = FPMUL(x[1],mat[0][1]) + FPMUL(y[1],mat[1][1]) + FPMUL(z[1],mat[2][1]);
    resz[1] = FPMUL(x[1],mat[0][2]) + FPMUL(y[1],mat[1][2]) + FPMUL(z[1],mat[2][2]) + (256<<FPP);
    scrx[1] = FPDIV(resx[1], resz[1])+SCREEN_HALF_W;
    scry[1] = FPDIV(resy[1], resz[1])+SCREEN_HALF_H;

    resx[2] = FPMUL(x[2],mat[0][0]) + FPMUL(y[2],mat[1][0]) + FPMUL(z[2],mat[2][0]);
    resy[2] = FPMUL(x[2],mat[0][1]) + FPMUL(y[2],mat[1][1]) + FPMUL(z[2],mat[2][1]);
    resz[2] = FPMUL(x[2],mat[0][2]) + FPMUL(y[2],mat[1][2]) + FPMUL(z[2],mat[2][2]) + (256<<FPP);
    scrx[2] = FPDIV(resx[2], resz[2])+SCREEN_HALF_W;
    scry[2] = FPDIV(resy[2], resz[2])+SCREEN_HALF_H;

    resx[3] = FPMUL(x[3],mat[0][0]) + FPMUL(y[3],mat[1][0]) + FPMUL(z[3],mat[2][0]);
    resy[3] = FPMUL(x[3],mat[0][1]) + FPMUL(y[3],mat[1][1]) + FPMUL(z[3],mat[2][1]);
    resz[3] = FPMUL(x[3],mat[0][2]) + FPMUL(y[3],mat[1][2]) + FPMUL(z[3],mat[2][2]) + (256<<FPP);
    scrx[3] = FPDIV(resx[3], resz[3])+SCREEN_HALF_W;
    scry[3] = FPDIV(resy[3], resz[3])+SCREEN_HALF_H;

    resx[4] = FPMUL(x[4],mat[0][0]) + FPMUL(y[4],mat[1][0]) + FPMUL(z[4],mat[2][0]);
    resy[4] = FPMUL(x[4],mat[0][1]) + FPMUL(y[4],mat[1][1]) + FPMUL(z[4],mat[2][1]);
    resz[4] = FPMUL(x[4],mat[0][2]) + FPMUL(y[4],mat[1][2]) + FPMUL(z[4],mat[2][2]) + (256<<FPP);
    scrx[4] = FPDIV(resx[4], resz[4])+SCREEN_HALF_W;
    scry[4] = FPDIV(resy[4], resz[4])+SCREEN_HALF_H;

    resx[5] = FPMUL(x[5],mat[0][0]) + FPMUL(y[5],mat[1][0]) + FPMUL(z[5],mat[2][0]);
    resy[5] = FPMUL(x[5],mat[0][1]) + FPMUL(y[5],mat[1][1]) + FPMUL(z[5],mat[2][1]);
    resz[5] = FPMUL(x[5],mat[0][2]) + FPMUL(y[5],mat[1][2]) + FPMUL(z[5],mat[2][2]) + (256<<FPP);
    scrx[5] = FPDIV(resx[5], resz[5])+SCREEN_HALF_W;
    scry[5] = FPDIV(resy[5], resz[5])+SCREEN_HALF_H;

    resx[6] = FPMUL(x[6],mat[0][0]) + FPMUL(y[6],mat[1][0]) + FPMUL(z[6],mat[2][0]);
    resy[6] = FPMUL(x[6],mat[0][1]) + FPMUL(y[6],mat[1][1]) + FPMUL(z[6],mat[2][1]);
    resz[6] = FPMUL(x[6],mat[0][2]) + FPMUL(y[6],mat[1][2]) + FPMUL(z[6],mat[2][2]) + (256<<FPP);
    scrx[6] = FPDIV(resx[6], resz[6])+SCREEN_HALF_W;
    scry[6] = FPDIV(resy[6], resz[6])+SCREEN_HALF_H;

    resx[7] = FPMUL(x[7],mat[0][0]) + FPMUL(y[6],mat[1][0]) + FPMUL(z[6],mat[2][0]);
    resy[7] = FPMUL(x[7],mat[0][1]) + FPMUL(y[6],mat[1][1]) + FPMUL(z[6],mat[2][1]);
    resz[7] = FPMUL(x[7],mat[0][2]) + FPMUL(y[6],mat[1][2]) + FPMUL(z[6],mat[2][2]) + (256<<FPP);
    scrx[7] = FPDIV(resx[7], resz[7])+SCREEN_HALF_W;
    scry[7] = FPDIV(resy[7], resz[7])+SCREEN_HALF_H;

    memset(screen, 0, SCREEN_W * SCREEN_H); // clear screen
    linedraw(scrx[0], scry[0], scrx[1], scry[1], 53, screen);
    linedraw(scrx[1], scry[1], scrx[2], scry[2], 53, screen);
    linedraw(scrx[2], scry[2], scrx[3], scry[3], 53, screen);
    linedraw(scrx[3], scry[3], scrx[0], scry[0], 53, screen);

    linedraw(scrx[0], scry[0], scrx[4], scry[4], 53, screen);
    linedraw(scrx[1], scry[1], scrx[5], scry[5], 53, screen);
    linedraw(scrx[2], scry[2], scrx[6], scry[6], 53, screen);
    linedraw(scrx[3], scry[3], scrx[7], scry[7], 53, screen);

    linedraw(scrx[4], scry[4], scrx[5], scry[5], 53, screen);
    linedraw(scrx[5], scry[5], scrx[6], scry[6], 53, screen);
    linedraw(scrx[6], scry[6], scrx[7], scry[7], 53, screen);
    linedraw(scrx[7], scry[7], scrx[4], scry[4], 53, screen);
}    

void tick_3d(char *screen)
{
    UBYTE i;
    int resx[NRPOINTS],
    resy[NRPOINTS],
    resz[NRPOINTS],
    scrx[NRPOINTS],
    scry[NRPOINTS];
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
    for(i = 0; i < NRPOINTS; i++)
    {
        resx[i] = FPMUL(x[i],mat[0][0]) + FPMUL(y[i],mat[1][0]) + FPMUL(z[i],mat[2][0]);
        resy[i] = FPMUL(x[i],mat[0][1]) + FPMUL(y[i],mat[1][1]) + FPMUL(z[i],mat[2][1]);
        resz[i] = FPMUL(x[i],mat[0][2]) + FPMUL(y[i],mat[1][2]) + FPMUL(z[i],mat[2][2]) + (256<<FPP);

        //MAKE PERSPECTIVE 
        scrx[i] = FPDIV(resx[i], resz[i]); // Mult with 1/x table (32k) instead of div
        scry[i] = FPDIV(resy[i], resz[i]);

        // Convert to 32:0, Scale and Place
        scrx[i] = SCREEN_HALF_W + (scrx[i]);    
        scry[i] = SCREEN_HALF_H + (scry[i]);
    }

    // DRAW OBJECT
    for(i = 0; i < NRPOINTS/2; i++)
    {
        linedraw(scrx[i], scry[i], scrx[i+4], scry[i+4], 55, screen);
        linedraw(scrx[i], scry[i], scrx[(i+1)%4], scry[(i+1)%4], 55, screen);
        linedraw(scrx[i+4], scry[i+4], scrx[((i+1)%4)+4], scry[((i+1)%4)+4], 55, screen);
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
