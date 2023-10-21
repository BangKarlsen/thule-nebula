#include <stdio.h>
#include <Hardware>
#include <hardware/custom.h>
#include "system/logger.h"
#include "scenes/3d.h"

volatile struct Custom *custom = (struct Custom *)0xdff000;


extern Start();
//extern drawFeedYourHead();
extern drawBackground();

/* #define COLOR00 0xdff180 */
/* extern UWORD* newPalette; */
/* extern UWORD* updatePaletteFlag; */

int main()
{
    printf("\nFind your tr0ffel and romkugler - here we go!..--~~~>>0\n\n(kom saa demo)\n\n\n");
    SERIAL_LOG("\n <Begin debug> \n")

    Start();

    SERIAL_LOG("\n <End debug> \n")
    return 0;
}

void maintick(__reg("a0") char* screen, __reg("d0") int sync)
{
    if(sync > 0 && sync < 10000) 
    {
        //drawFeedYourHead();
        //tick_3d_unroll(screen);
        tick_3d(screen);
    }
/*
    if(!(sync % 10)) 
    {
        log_fmt("sync=%d\n", sync);
        custom->color[0] = 0xf0f;	// bg white
    }
    else 
    {
        custom->color[0] = 0xf0f;	// bg black
    }
    */
}

/* void calculate(__reg("a0") unsigned char *screen, __reg("d0") int step ) */
/* { */
/*     const int width = 320; */
/*     int x = 160 + step % 20; */
/*     int y = 80 + step % 34; */

/*     /1* SERIAL_LOG_FMT("step=%d\n", step) *1/ */
/*     screen[x+y*width] = step%256; */
/* } */

void drawback(__reg("a0") unsigned char *screen, __reg("a1") unsigned char *texture)
{
    const int width = 320;
    
    // naive fill screen, but it works - there is texture!
    for(int x = 0; x < 320; x++) {
        for(int y = 0; y < 176; y++) {
            screen[x + y * width] = texture[x + y * width]; 
        }       
    }
}
