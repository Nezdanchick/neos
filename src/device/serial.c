#include <uefi.h>
#include "device/serial.h"

char get_serial() {
    FILE *f;
    char buff[2];

    if((f = fopen("/dev/serial", "r"))) {
        while(!fread(buff, 1, 1, f));
        buff[1] = 0;
        fclose(f);
    }
    else {
        fprintf(stderr, "Unable to open serial port\n");
    }
    return buff[0];
}