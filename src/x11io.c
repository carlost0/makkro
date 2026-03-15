#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#include "io.h"

void delay(int ms) {
    clock_t start_time = clock();
    clock_t wait_time = ms * (CLOCKS_PER_SEC / 1000);

    while (clock() - start_time < wait_time);
}

bool check_uppercase(char c) {
    if (c > 64 && c < 91)
        return true;
    return false;
}

XKeyEvent init_xkey_event(Display *display, Window root_win, Window win, char *key)  {
    XKeyEvent event;

    event.display = display;
    event.root = root_win;
    event.window = win;
    event.subwindow = None;
    event.x = 0;
    event.y = 0;
    event.x_root = 0;
    event.y_root = 0;
    event.time = CurrentTime;
    event.state = 0;
    event.keycode = XKeysymToKeycode(display, XStringToKeysym(key));

    return event;
}

vec2_res get_pointer_pos() {
    vec2_res res;

    Display *display;
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        perror("unable to open X display :(\nError:");
        res.err = -1;
        return res;
    }

    Window root_window;
    root_window = DefaultRootWindow(display);
    if (root_window == 0) {
        perror("unable to find X root window :(\nError:");
        res.err = -1;
        return res;
    }

    vec2 root;
    vec2 window;

    unsigned int mask;
    Window root_ret, child_ret;

    if (!XQueryPointer(
        display,
        root_window,
        &root_ret,
        &child_ret,
        &root.x,
        &root.y,
        &window.x,
        &window.y,
        &mask))
    {
        perror("unable to find pointer :(\nError:");
        res.err = -1;
        XCloseDisplay(display);
        return res;
    }
    XCloseDisplay(display);
    return (vec2_res)root;
}

int move_cursor(int x, int y) {
    Display *display;
    display = XOpenDisplay(NULL);
    if (display == NULL) {
        perror("unable to open X display :(\nError");
        return -1;
    }

    Window root_window;
    root_window = DefaultRootWindow(display);
    if (root_window == 0) {
        perror("unable to find X root window :(\nError");
        XCloseDisplay(display);
        return -1;
    }

    XWarpPointer(
        display,
        None,
        root_window,
        0, 0, 0, 0,
        x, y
    );

    XFlush(display);
    XCloseDisplay(display);
    return 0;
}

int send_mb(unsigned char button) {
    if (button > 5) {
        perror("unable to find keysym for button :(\nError");
        return -1;
    }

    XEvent event;
    Display *display;
    Window root_win;
    Window win;
    int rev_to;
    vec2_res root_pos;

    memset(&event, 0, sizeof(XEvent));

    KeySym buttons[5] = {
        XK_Pointer_Button1,
        XK_Pointer_Button2,
        XK_Pointer_Button3,
        XK_Pointer_Button4,
        XK_Pointer_Button5,
    };

    display = XOpenDisplay(NULL);
    if (display == NULL) {
        perror("unable to open X display :(\nError");
        return -1;
    }

    root_win = DefaultRootWindow(display);
    if (root_win == 0) {
        perror("unable to find X root window :(\nError");
        XCloseDisplay(display);
        return -1;
    }

    XGetInputFocus(display, &win, &rev_to);
    if (win == None) {
        perror("unable to find focused window :(\nError");
        XCloseDisplay(display);
        return -1;
    }

    root_pos = get_pointer_pos();
    if (root_pos.err < 0) {
        XCloseDisplay(display);
        return -1;
    }


    event.xbutton.display = display;
    event.xbutton.window = win;
    event.xbutton.root = root_win;
    event.xbutton.x = root_pos.vec.x;
    event.xbutton.y = root_pos.vec.y;
    event.xbutton.x_root = root_pos.vec.x;
    event.xbutton.y_root = root_pos.vec.y;
    event.xbutton.subwindow = root_win;
    event.xbutton.button = /*buttons[button - 1]*/ button;
    event.xbutton.same_screen = True;

    event.xbutton.type = ButtonPress;
    event.xbutton.time = CurrentTime;
    XSendEvent(display, win, True, ButtonPressMask | ButtonReleaseMask, &event);
    XFlush(display);

    delay(200);

    event.xbutton.type = ButtonRelease;
    XSendEvent(display, win, True, ButtonReleaseMask, &event);
    XFlush(display);

    XCloseDisplay(display);

    return 0;
}

int send_key(char *key) {
    XKeyEvent event;
    Display *display;
    Window root_win;
    Window win;
    int rev_to;
    vec2_res root_pos;

    display = XOpenDisplay(NULL);
    if (display == NULL) {
        perror("unable to open X display :(\nError");
        return -1;
    }

    root_win = DefaultRootWindow(display);
    if (root_win == 0) {
        perror("unable to find X root window :(\nError");
        XCloseDisplay(display);
        return -1;
    }

    XGetInputFocus(display, &win, &rev_to);
    if (win == None) {
        perror("unable to find focused window :(\nError");
        XCloseDisplay(display);
        return -1;
    }

    root_pos = get_pointer_pos();
    if (root_pos.err < 0) {
        XCloseDisplay(display);
        return -1;
    }

    XSelectInput(display, win, KeyPressMask | KeyReleaseMask);

    event = init_xkey_event(display, root_win, win, key);

    if (event.keycode == 0) {
        perror("unable to translate keysym to keycode :(\nError");
        return -1;
    }

    /* press key */
    event.type = KeyPress;
    XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent *)&event);
    XFlush(display);

    /* release key */
    event.type = KeyRelease;
    XSendEvent(event.display, event.window, True, KeyReleaseMask, (XEvent *)&event);
    XFlush(display);

    XCloseDisplay(display);

    return 0;
}

int send_str(char *str) {
    XKeyEvent  event;
    Display   *display;
    Window     root_win;
    Window     win;
    int        rev_to;
    vec2_res   root_pos;

    char       c[1];

    display = XOpenDisplay(NULL);
    if (display == NULL) {
        perror("unable to open X display :(\nError");
        return -1;
    }

    root_win = DefaultRootWindow(display);
    if (root_win == 0) {
        perror("unable to find X root window :(\nError");
        XCloseDisplay(display);
        return -1;
    }

    XGetInputFocus(display, &win, &rev_to);
    if (win == None) {
        perror("unable to find focused window :(\nError");
        XCloseDisplay(display);
        return -1;
    }

    root_pos = get_pointer_pos();
    if (root_pos.err < 0) {
        XCloseDisplay(display);
        return -1;
    }

    XSelectInput(display, win, KeyPressMask | KeyReleaseMask);

    while (*str != '\0') {
        c[0] = *str;
        event = init_xkey_event(display, root_win, win, c);

        if (c[0] == ' ') {
            event.keycode = XKeysymToKeycode(event.display, XK_space);
        }

        if (event.keycode == 0) {
            perror("unable to translate keysym to keycode :(\nError");
            return -1;
        }

        /* press key */
        event.type = KeyPress;
        XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent *)&event);
        XFlush(display);

        /* release key */
        event.type = KeyRelease;
        XSendEvent(event.display, event.window, True, KeyReleaseMask, (XEvent *)&event);
        XFlush(display);

        ++str;
    }

    XCloseDisplay(display);

    return 0;
}
