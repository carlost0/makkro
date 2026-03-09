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
int  move_cursor(int x, int y);
int  send_key(char *key);

#endif /*MOUSE_H_*/
