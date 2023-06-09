#include <uefi.h>
#include <stdint.h>
#include <stdbool.h>
#include "device/screen.h"

void about() {
    printf("\
      ::::    ::: :::::::::: ::::::::   :::::::: \n\
     :+:+:   :+: :+:       :+:    :+: :+:    :+: \n\
    :+:+:+  +:+ +:+       +:+    +:+ +:+         \n\
   +#+ +:+ +#+ +#++:++#  +#+    +:+ +#++:++#++   \n\
  +#+  +#+#+# +#+       +#+    +#+        +#+    \n\
 #+#   #+#+# #+#       #+#    #+# #+#    #+#     \n\
###    #### ########## ########   ########       \n\
");
}

void draw_rect(int x, int y, int w, int h, int color) {
    int x_offset = x < w + x ? 1 : -1;
    int y_offset = y < h + y ? 1 : -1;
    for (int i = x; i - x_offset != w + x; i += x_offset) {
        for (int j = y; j - y_offset != h + y; j += y_offset) {
            set_pixel(i, j, color);
        }
    }
}
int64_t pow(int64_t x, int pow)
{
    int64_t result = 1;
    while (pow)
    {
        if ( (pow & 1) == 1 )
            result *= x;
        x *= x;
        pow >>= 1;
    }
    return result;
}

int main(int argc, char **argv)
{
    init_videomode();
    clear_screen();

    about();
    getinfo_framebuffer();
    getinfo_videomodes(0);
    draw_rect(0, 0, 50, 300, 0xfff000);
    draw_rect(0, 0, 300, 50, 0xffffff);
    draw_rect(300, 300, -50, -300, 0xff0000);
    draw_rect(300, 300, -300, -50, 0x00ff00);
    
    sleep(3);
    
    for (int64_t i = pow(2,63) - 1; 1; i++) {
        printf("%d\r", i);
        sleep(5);
    }
    sleep(30);
    return 0;
}