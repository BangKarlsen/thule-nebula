#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdio.h>
#include <clib/debug_protos.h>

#define SERIAL_LOG(msg) kprintf(msg);

void log(unsigned char* text);
void log_fmt(unsigned char* text, int var);

#endif
