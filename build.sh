#!/bin/sh

PROGNAME="dwm-status"

DEBUGFLAGS="-g -O0"
DEBUGEXT="_debug"
if [[ $1 = '-R' ]]; then
    DEBUGFLAGS=
    DEBUGEXT=
    shift
fi

CFLAGS="-std=c11 -pedantic -pipe -D_DEFAULT_SOURCE $(pkg-config --cflags x11)"
WARNINGS="-Wall -Wextra -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function"
LDLIBS="$(pkg-config --libs x11)"

time clang $DEBUGFLAGS $CFLAGS $WARNINGS $@ -o $PROGNAME$DEBUGEXT $PROGNAME.c $LDLIBS
