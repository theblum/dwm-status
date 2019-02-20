#if !defined(OPTIONS_H)
/* ===========================================================================
 * File: options.h
 * Date: 19 Feb 2019
 * Creator: Brian Blumberg <blum@disroot.org>
 * Notice: Copyright (c) 2019 Brian Blumberg
 * ===========================================================================
 */

struct options {
    char *progname;
    char *logfile;
    char *pidfile;
    int daemon;
    int kill;
};

#define OPTIONS_H
#endif
