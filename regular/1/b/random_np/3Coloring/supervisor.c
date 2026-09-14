/**
 * @file supervisor.c
 * @author kernkraftwerk (kernkraftdev@hotmail.com)
 * @brief read solutions from generator.c and write them in a mpage
 * @version 0.1
 * @date 2026-09-14
 *
 * @copyright Copyright (c) 2026
 *
 */

#include "common.h"

/** initialize shared memory (always the same) */

static volatile sig_atomic_t quit = false;
static void onSignal(int sig, siginfo_t *si, void *unused)
{
    quit = true;
}
static void initSignalHandler(void)
{
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);
    sa.sa_sigaction = onSignal;
    if (sigaction(SIGINT, &sa, NULL) == -1)
    {
        error("sigaction");
    }
    if (sigaction(SIGTERM, &sa, NULL) == -1)
    {
        error("sigaction");
    }
    if (sigaction(SIGQUIT, &sa, NULL) == -1)
    {
        error("sigaction");
    }
}

static void readGeneratedSolution(struct Shm_t *shmp)
{
}

int main(int argc, char **argv)
{
}
