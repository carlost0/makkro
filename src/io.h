#ifndef IO_H_
#define IO_H_

#ifdef USE_X11
    #include <X11/Xlib.h>
    #include <X11/Xutil.h>
    #include <X11/keysym.h>

    XKeyEvent init_xkey_event(Display *display, Window root_win, Window win, const char *key);
    int       send_keysym(KeySym keysym);
#endif
typedef struct {
    int x, y;
} vec2;

typedef union {
    vec2 vec;
    int err;
} vec2_res;

void               delay(int ms);
vec2_res get_pointer_pos();
int          move_cursor(int x, int y);
int             send_key(const char *key);
int           toggle_key(const char *key, int state);
int              send_mb(unsigned char button);
int            toggle_mb(unsigned char button, int state);
int             send_str(const char *str);

#ifdef USE_X11
#endif /* USE_X11  */

#endif /* IO_H_ */
