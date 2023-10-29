#include <hardware/custom.h>

volatile struct Custom *custom = (struct Custom *)0xdff000;

void flash()
{
    custom->bplcon3 = 0x0000;   // lower nibbles
    custom->color[0] = 0x0135;  //;
    custom->bplcon3 = 0x0200;   // set bit 9
    custom->color[0] = 0x0246;  // upper nibbles
}

void tick_dots(unsigned char *screen, int sync) 
{
    const int width = 320;
    int x = 160 + sync % 20;
    int y = 80 + sync % 34;

    if(!(sync % 2)) 
    {
        /* log_fmt("sync=%d\n", sync); */
        flash();
    } else 
    {
        custom->color[0] = 0x000;  // upper nibbles
    }

    screen[x+y*width] = sync%256;
}
