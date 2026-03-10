#ifndef MOUSE_H_
#define MOUSE_H_

typedef struct {
    int x, y;
} vec2;

typedef union {
    vec2 vec;
    int err;
} vec2_res;

vec2_res get_pointer_pos();
int      move_cursor(int x, int y);
int      send_key(char *key);
int      send_str(char *str);
bool     check_uppercase(char c);

#ifdef USE_X11
    XKeyEvent init_xkey_event(Display *display, Window root_win, Window win, char *key);
#endif /* USE_X11  */
#endif /* MOUSE_H_ */
