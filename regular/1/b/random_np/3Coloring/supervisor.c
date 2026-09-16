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

static volatile sig_atomic_t quitProgram = false;
static void onSignal(int sig, siginfo_t *si, void *unused)
{
    quitProgram = true;
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

static void readGeneratedSolution(struct Shm *shmp)
{

    if (sem_wait(&shmp->numUsed) == -1)
    {
        if (errno == EINTR)
            return;
        error("sem_wait");
    }
    struct EDGE_LIST solution = shmp->circular_buffer[shmp->readIndex];
    shmp->readIndex = (shmp->readIndex + 1) % buff_length;

    if (sem_post(&shmp->numFree) == -1)
    {
        error("sem_post");
    }

    if (solution.length == 0)
    {
        printf("Colorable\n");
        quitProgram = true;
        return;
    }

    static size_t size = ULONG_MAX;

    if (solution.length < size)
    {
        size = solution.length;
        printf("[./supervisor] Solution: %ld edges:", solution.length);
    }
}

int main(int argc, char **argv)
{

    initSignalHandler();

    int fd = shm_open(SHM_PATH, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1)
    {
        error("shm_open");
    }

    if (ftruncate(fd, sizeof(struct Shm)) == -1)
    {
        error("ftruncate");
    }

    struct Shm *shmp = mmap(NULL, sizeof(*shmp), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shmp == MAP_FAILED)
    {
        error("mmap");
    }

    shmp->killprocess = false;
    shmp->numGenerators = 0;
    shmp->readIndex = 0;
    shmp->writeIndex = 0;

    if (sem_init(&shmp->writeMutex, true, 1) == -1)
    {
        error("sem_init");
    }
    if (sem_init(&shmp->numUsed, true, 0) == -1)
    {
        error("sem_init");
    }
    if (sem_init(&shmp->numFree, true, buff_length) == -1)
    {
        error("sem_init");
    }
    while (!quitProgram)
    {
        readGeneratedSolution(shmp);
    }

    printf("Terminating the %ld active generators...\n", shmp->numGenerators);
    shmp->killprocess = true;

    if (sem_destroy(&(shmp->writeMutex)) == -1)
    {
        error("sem_destroy");
    }
    if (sem_destroy(&(shmp->numUsed)) == -1)
    {
        error("sem_destroy");
    }
    if (sem_destroy(&(shmp->numFree)) == -1)
    {
        error("sem_destroy");
    }

    if (munmap(shmp, sizeof(*shmp)) == -1)
    {
        error("munmap");
    }
    if (shm_unlink(SHM_PATH) == -1)
    {
        error("shm_unlink");
    }
    if (close(fd) == -1)
    {
        error("close");
    }
    exit(EXIT_SUCCESS);
}
