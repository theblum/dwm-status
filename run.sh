#!/bin/sh

PROGNAME="dwm-status"

PREFIX=
if [[ $1 = '-d' ]]; then PREFIX="lldb"
elif [[ $1 = '-v' ]]; then PREFIX="valgrind --leak-check=full"
fi

$PREFIX ./$PROGNAME
