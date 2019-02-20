#!/bin/sh

PROGNAME="dwm-status"

CFLAGS="-std=c11 -g -O0 -pedantic -pipe -D_DEFAULT_SOURCE $(pkg-config --cflags x11)"
WARNINGS="-Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function"
LDLIBS="$(pkg-config --libs x11)"

time clang $CFLAGS $WARNINGS $@ -o $PROGNAME $PROGNAME.c $LDLIBS
