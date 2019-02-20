/* ===========================================================================
 * File: dwm-status.c
 * Date: 18 Feb 2019
 * Creator: Brian Blumberg <blum@disroot.org>
 * Notice: Copyright (c) 2019 Brian Blumberg
 * ===========================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>
#include <errno.h>
#include <error.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#include "options.h"
#include "status.h"

#define TS_ONESEC &(struct timespec){1, 0}
#define PIDFILE "/run/user/1000/dwm-status.pid"
#define LOGFILE "/tmp/dwm-status.log"

#include "status.c"
#include "options.c"

static int running;
static int pidfd;
static struct options opts;

static struct statusitems items[] = {
    { datetime, 0 },
    { 0, 0 },
};

static void
handler(int signum)
{
    if(lockf(pidfd, F_ULOCK, 0) < 0)
        error(errno, errno, "Unable to unlock %s", opts.pidfile);
    close(pidfd);

    unlink(opts.pidfile);

    running = 0;
}

static void
daemonize(void)
{
    pid_t pid = fork();
    if(pid < 0) error(errno, errno, "Unable to fork parent");
    if(pid > 0) exit(EXIT_SUCCESS);

    umask(0);
    chdir("/");

    int logfd = open(opts.logfile, O_RDWR|O_APPEND|O_CREAT, 0640);
    if(logfd < 0) error(errno, errno, "Unable to open %s", opts.logfile);
    dup2(logfd, STDIN_FILENO);
    dup2(logfd, STDOUT_FILENO);
    dup2(logfd, STDERR_FILENO);
    close(logfd);

    if(setsid() < 0) {
        error(errno, errno, "Unable to become session leader");
    }

    pid = fork();
    if(pid < 0) error(errno, errno, "Unable to fork child");
    if(pid > 0) exit(EXIT_SUCCESS);
}

static void
killit(void)
{
    pidfd = open(opts.pidfile, O_RDONLY);
    if(pidfd < 0) error(errno, errno, "Unable to open %s", opts.pidfile);

    char pidstr[256];
    int nbytes = read(pidfd, pidstr, 255);
    if(nbytes < 0)
        error(errno, errno, "Unable to read from %s", opts.pidfile);
    pidstr[nbytes] = '\0';

    int pid = atoi(pidstr);
    if(kill(pid, SIGINT) < 0)
        error(errno, errno, "Unable to kill %d", pid);
}

int
main(int argc, char **argv)
{
    opts.progname = strdup(argv[0]);
    opts.logfile = strdup(LOGFILE);
    opts.pidfile = strdup(PIDFILE);

    optparse(argc, argv, &opts);

    if(opts.kill) {
        killit();
        exit(EXIT_SUCCESS);
    }

    if(opts.daemon) {
        daemonize();
    }

    pidfd = open(opts.pidfile, O_RDWR|O_CREAT, 0644);
    if(pidfd < 0) error(errno, errno, "Unable to open %s", opts.pidfile);
    if(lockf(pidfd, F_TLOCK, 0) < 0)
        error(errno, errno, "Unable to lock %s", opts.pidfile);
    dprintf(pidfd, "%d\n", getpid());

    struct sigaction sa = {
        .sa_handler = handler,
        .sa_flags   = 0,
    };
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, 0);
    sigaction(SIGTERM, &sa, 0);

    struct statusbar bar = {0};
    bar.handler = handler;

    if(!createstatus(&bar)) {
        fprintf(stderr, "%s: Unable to create status bar.\n", opts.progname);
        exit(EXIT_FAILURE);
    }

    running = 1;
    while(running) {
        clearstatus(&bar);
        for(struct statusitems *item = items; item->func; ++item)
            item->func(&bar, item->data);
        updatestatus(&bar);

        clock_nanosleep(CLOCK_MONOTONIC, 0, TS_ONESEC, 0);
    }

    destroystatus(&bar);

    free(opts.progname);
    free(opts.logfile);
    free(opts.pidfile);

    exit(EXIT_SUCCESS);
}
