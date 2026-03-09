#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#ifdef USE_X11
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
    #include <X11/keysym.h>
#endif /*USE_X11*/

#include "../lib/io.h"

int main() {
    if (send_key("a") < 0)
        return -1;

    return 0;
}
