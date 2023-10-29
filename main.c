#include <stdio.h>
#include <Hardware>
#include "system/logger.h"
#include "scenes/3d.h"
#include "scenes/dots.h"



extern Start();
//extern drawFeedYourHead();
//extern drawBackground();

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
        //tick_3d(screen);
        tick_dots(screen, sync);
    }
}

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
