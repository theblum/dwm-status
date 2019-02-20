/* ===========================================================================
 * File: status.c
 * Date: 19 Feb 2019
 * Creator: Brian Blumberg <blum@disroot.org>
 * Notice: Copyright (c) 2019 Brian Blumberg
 * ===========================================================================
 */

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

static void (*barhandler)(int signum);

static int
xioerror(Display *dpy)
{
    barhandler(0);
    exit(EXIT_SUCCESS);
}

static int
createstatus(struct statusbar *bar) {
    int ret = True;
    bar->dpy = XOpenDisplay(0);
    if(!bar->dpy) {
        ret = False;
    }

    barhandler = bar->handler;
    XSetIOErrorHandler(xioerror);

    bar->w = DefaultRootWindow(bar->dpy);

    return ret;
}

static void
destroystatus(struct statusbar *bar) {
    XCloseDisplay(bar->dpy);
}

static void
clearstatus(struct statusbar *bar)
{
    bar->str[0] = '\0';
    bar->len = 0;
}

static void
updatestatus(struct statusbar *bar)
{
    XTextProperty tp = {
        .value    = (unsigned char *)bar->str,
        .encoding = XA_STRING,
        .format   = 8,
        .nitems   = strlen(bar->str),
    };

    XSetWMName(bar->dpy, bar->w, &tp);
    XSync(bar->dpy, False);
}

void
datetime(struct statusbar *bar, void *data)
{
    time_t ctim = time(0);
    char str[MAXSTR];
    strftime(str, MAXSTR, "%a, %b %d %Y :: %I:%M:%S%P", localtime(&ctim));
    strncat(bar->str, str, MAXSTR-bar->len-1);
    bar->str[MAXSTR-1] = '\0';
    bar->len = strlen(bar->str);
}

void
separator(struct statusbar *bar, void *data)
{
    char *sep = (char *)data;
    strncat(bar->str, sep, MAXSTR-bar->len-1);
    bar->str[MAXSTR-1] = '\0';
    bar->len = strlen(bar->str);
}
