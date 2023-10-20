#include <stdio.h>
#include <Hardware>
#include "system/logger.h"

extern Start(); // demo.s

/* #define COLOR00 0xdff180 */
/* extern UWORD* newPalette; */
/* extern UWORD* updatePaletteFlag; */

int main() {

    printf("\nFind your tr0ffel and romkugler - here we go!..--~~~>>0\n\n(kom saa demo)\n\n\n");
    SERIAL_LOG("\n <Begin debug> \n")

    Start();

    SERIAL_LOG("\n <End debug> \n")
    return 0;
}

void calculate(__reg("a0") unsigned char *screen, __reg("d0") int step ) {
    const int width = 320;
    int x = 160 + step % 20;
    int y = 80 + step % 34;

    /* SERIAL_LOG_FMT("step=%d\n", step) */
    screen[x+y*width] = step%256;
}

void drawback(__reg("a0") unsigned char *screen, __reg("a1") unsigned char *texture) {
    const int width = 320;
    
    // naive fill screen, but it works - there is texture!
    for(int x = 0; x < 320; x++) {
        for(int y = 0; y < 176; y++) {
            screen[x + y * width] = texture[x + y * width]; 
        }       
    }
}
