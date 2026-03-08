#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>

#include "../../lib/io.h"

#include <stdio.h>
#include <strings.h>
#include <unistd.h>

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

int send_key(KeySym key) {
    if (key == NoSymbol) {
        perror("no valid keysymbol found :(\nError");
        return -1;
    }
    XKeyEvent event;

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

    Window focused_window;
    int rev_to;

    XGetInputFocus(display, &focused_window, &rev_to);
    if (focused_window == None) {
        perror("unable to find focused window :(\nError");
        XCloseDisplay(display);
        return -1;
    }

    vec2_res root_pos;
    root_pos = get_pointer_pos();
    if (root_pos.err < 0) {
        XCloseDisplay(display);
        return -1;
    }

    XSelectInput(display, focused_window, KeyPressMask | KeyReleaseMask);

    event.display = display;
    event.root = root_window;
    event.window = focused_window;
    event.subwindow = None;
    event.x = 0;
    event.y = 0;
    event.x_root = 0;
    event.y_root = 0;
    event.time = CurrentTime;
    event.state = 0;
    event.keycode = XKeysymToKeycode(display, key);

    if (event.keycode == 0) {
        perror("unable to translate keysym to keycode :(\nError");
        return -1;
    }

    /* press key */
    event.type = KeyPress;
    XSendEvent(event.display, event.window, True, KeyPressMask, (XEvent *)&event);
    XFlush(display);

    sleep(1);
    /* release key */
    event.type = KeyRelease;
    XSendEvent(event.display, event.window, True, KeyReleaseMask, (XEvent *)&event);
    XFlush(display);

    XCloseDisplay(display);

    return 0;
}
