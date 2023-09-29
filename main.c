#include <stdio.h>
#include <clib/debug_protos.h>

extern Start();

#define MSG_LENGTH 32
char msg_buffer[MSG_LENGTH];
#define SERIAL_LOG(msg) kprintf(msg);
#define SERIAL_LOG_FMT(fmt, var) snprintf(msg_buffer, MSG_LENGTH, fmt, var); kprintf(msg_buffer);

int main() {

        printf("\nFind your tr0ffel and romkugler - here we go!..--~~~>>0\n\n(kom saa demo)\n\n");
        SERIAL_LOG("\n <Begin debug> \n")

        Start();

        SERIAL_LOG("\n <End debug> \n")
        return 0;
}

void calculate( __reg("a0") unsigned char *screen, __reg("d0") int step )
{
        const int width=320;

        int x = 160 + step%20;
        int y = 80 + step%34;

        SERIAL_LOG_FMT("step=%d\n", step)

        screen[x+y*width] = step%256;
}
