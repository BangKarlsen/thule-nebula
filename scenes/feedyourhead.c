/*
   OPENING
   fire effect spejlet i y-aksen (y=88)
   text overlay 'THULE NEBULA' twister oveni, hvid skrivt evt a la
   credits overlay
*/

#include <proto/exec.h> // AllocMem, FreeMem
#include <string.h>     // memset
#include "../system/logger.h"

#define FIRE_W 320
#define FIRE_H 64
#define SCREEN_W 320
#define SCREEN_H 176
#define SCREEN_HALF_H 88

UBYTE* fire;

void init_feedyourhead(__reg("a0") char *screen)
{
    fire = AllocMem(FIRE_W * FIRE_H, MEMF_FAST);
    if (!fire)
    {
        SERIAL_LOG("Could not make fire >:|")
    }

    memset(fire, 0, FIRE_W * FIRE_H);
    memset(screen, 0, SCREEN_W * SCREEN_H); // clear screen
                                            //
    SERIAL_LOG("init good")
}

// Returns values from 1 to 255 inclusive, period is 255
// From http://www.arklyffe.com/main/2010/08/29/xorshift-pseudorandom-number-generator/
/*
or:

move.l  a1,a0           ;saves pointer
moveq   #4,d0
.random ror.l   d0,d0           ;tanx to Azure for this..
addq.l  #7,d0           ;smart (and chip) rnd generator
move.b  d0,(a0)+        ;fill texture with random values
dbra    d7,.random
*/
UBYTE rand255()
{
    static UBYTE smof = 1;
    smof ^= (smof << 7);
    smof ^= (smof >> 5);
    return smof ^= (smof << 3);
}

void addFire(char* fire, int vblank)
{
    UBYTE yMax = vblank % 2 ? 2 : 1;
    
    for (int fireY = 0; fireY < yMax * 320; fireY += 320)     // add to bottom of fire buffer
    {
        for (int fireX = 2; fireX < 320 - 2; fireX++)
        {
            if (rand255() > 220)
            {
                fire[fireY + fireX] = 250;
            } else
            {
                fire[fireY + fireX] = rand255() % 100;
            }
        }
    }
}

void extraSparks(char* fire)
{
    int x = rand255() + 32;     // what ???
    int y = rand255() % FIRE_H - 10;    // buggy? could be negative?
    fire[y * FIRE_W + x - 1] = rand255();
    fire[(y + 1) * FIRE_W + x] = rand255();
    fire[y * FIRE_W + x + 1] = rand255();
}

void updateFire(char* fire, int vblank) 
{
    /* move fire upwards, start at bottom*/
    for (int y_lines = 0; y_lines < FIRE_H * 320 ; y_lines += 320)
    {
        // erstat `y_lines * 320` med var.
        // så kun muls 320 én gang.
        // og noget med y_lines += 320 
        for (int x = 0; x < 320; ++x)
        {
            int temp;
            if (x == 0) /* at the left border*/
            {
                temp = fire[y_lines];
                temp += fire[y_lines + 1];
                temp += fire[y_lines - 320];
                temp /= 3;
            }
            else if (x == 320 - 1) /* at the right border*/
            {
                temp = fire[y_lines + x];
                temp += fire[y_lines - 320 + x];
                temp += fire[y_lines + x - 1];
                temp /= 3;
            }
            else /* main case */
            {
                temp = fire[y_lines + x];
                temp += fire[y_lines + x + 1];
                temp += fire[y_lines + x - 1];
                temp += fire[y_lines - 320 + x];
                temp >>= 2;
            }
            if (1 || vblank < 3000)
            {
                if (temp > 170)
                {
                    temp -= 2; /* decay_lines */
                }
            } else {
                if (temp > 0 ) {
                    temp -= 1;
                } /*else
                    {
                    temp = 0;
                    }*/
            }

            fire[y_lines + x] = temp;
        }
    }

}

void copyFireToScreen(char* fire, char* screen)
{
    UBYTE* image = screen + SCREEN_W * SCREEN_HALF_H;        /*start in the right bottom corner*/
    UBYTE* imageMirror = screen + 320 * SCREEN_HALF_H;  // is this correct?

    for (int y = 0; y < FIRE_H; y++)
    {
        image -= 320;   // skip every second line for xtra coolness
        imageMirror += 320;
        for (int x = 0; x < FIRE_W; x++)
        {
            *image = fire[y * 320 + x];
            image--;
            *imageMirror = fire[y * 320 + x];
            imageMirror++;
        }
    }
}

void tick_feedyourhead(__reg("a0") unsigned char* screen, __reg("d0") int vblank)
{
    log_fmt("vblank=%d\n", vblank);

    addFire(fire, vblank);

    if (vblank % 2)
    {
        extraSparks(fire);
    }

    updateFire(fire, vblank);
    copyFireToScreen(fire, screen);
}

void deinit_feedyourhead()
{
    if (fire)
    {
        kprintf("deinit_feedyourhead\n");
        FreeMem(fire, FIRE_W * FIRE_H);
    }
}
