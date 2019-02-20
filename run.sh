#!/bin/sh

PROGNAME="dwm-status"

PREFIX=
if [[ $1 = '-d' ]]; then PREFIX="lldb --"; shift
elif [[ $1 = '-v' ]]; then PREFIX="valgrind --leak-check=full --"; shift
fi

$PREFIX ./$PROGNAME $@
