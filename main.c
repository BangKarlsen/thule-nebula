#include <stdio.h>

extern Start();

int main() {

        printf("\nFind your tr0ffel and romkugler - here we go!..--~~~>>0\n\n(kom saa demo)\n\n\n\n");

        Start();

        return 0;
}

void calculate( __reg("a0") unsigned char *screen, __reg("d0") step )
{
        const int width=320;

        int x = 160 + step%20;
        int y = 80 + step%34;

        screen[x+y*width] = step%256;
}
