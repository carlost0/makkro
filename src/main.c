#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#ifdef USE_X11
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
    #include <X11/keysym.h>
#endif /*USE_X11*/

#include "io.h"

int main() {
    vec2_res pos;
    for (;;) {
        pos = get_pointer_pos();
        printf("%d, %d\n", pos.vec.x, pos.vec.y);
    }

    return 0;
}
