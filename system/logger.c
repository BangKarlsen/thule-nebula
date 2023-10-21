#include "logger.h"


void log(unsigned char* text){
    kprintf(text);
}

#define MSG_LENGTH 32
char msg_buffer[MSG_LENGTH];

void log_fmt(unsigned char* fmt_text, int var){
    snprintf(msg_buffer, MSG_LENGTH, fmt_text, var);
    kprintf(msg_buffer);

}
