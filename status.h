#if !defined(STATUS_H)
/* ===========================================================================
 * File: status.h
 * Date: 19 Feb 2019
 * Creator: Brian Blumberg <blum@disroot.org>
 * Notice: Copyright (c) 2019 Brian Blumberg
 * ===========================================================================
 */

#include <X11/Xlib.h>

#define MAXSTR 1024

struct statusbar {
    Display *dpy;
    Window w;
    char str[MAXSTR];
    size_t len;
    void (*handler)(int signum);
};

struct statusitems {
    void (*func)(struct statusbar *bar, void *data);
    void *data;
};

#define STATUS_H
#endif
