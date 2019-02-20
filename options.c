/* ===========================================================================
 * File: options.c
 * Date: 19 Feb 2019
 * Creator: Brian Blumberg <blum@disroot.org>
 * Notice: Copyright (c) 2019 Brian Blumberg
 * ===========================================================================
 */

#include <getopt.h>

static struct option longopts[] = {
    { "daemon",  no_argument,       0, 'd' },
    { "kill",    no_argument,       0, 'k' },
    { "logfile", required_argument, 0, 'l' },
    { "pidfile", required_argument, 0, 'p' },
    { "help",    no_argument,       0, 'h' },
    { 0, 0, 0, 0 },
};

static void
printhelp(char *progname)
{
    printf("Usage: %s [OPTION]...\n", progname);
    printf("Creates and updates a status bar in dwm.\n\n");
    printf("  Options:\n");
    printf("    -d, --daemon          daemonize this program\n");
    printf("    -k, --kill            kill running daemon\n");
    printf("    -l, --logfile=FILE    log file location\n");
    printf("    -p, --pidfile=FILE    pid file location\n");
    printf("    -h, --help            print this help\n");
}

static void
optparse(int argc, char **argv, struct options *opts)
{
    int val, optindex = 0;

    while((val = getopt_long(argc, argv, "dkl:p:h", longopts, &optindex)) != -1) {
        switch(val) {
            case 'd':
            {
                opts->daemon = 1;
            } break;

            case 'k':
            {
                opts->kill = 1;
            } break;

            case 'l':
            {
                free(opts->logfile);
                opts->logfile = strdup(optarg);
            } break;

            case 'p':
            {
                free(opts->pidfile);
                opts->pidfile = strdup(optarg);
            } break;

            case 'h':
            {
                printhelp(opts->progname);
                exit(EXIT_SUCCESS);
            } break;

            case '?':
            {
                printhelp(opts->progname);
                exit(EXIT_FAILURE);
            }

            default: break;
        }
    }
}
