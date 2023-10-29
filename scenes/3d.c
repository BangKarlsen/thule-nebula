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
