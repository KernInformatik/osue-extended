#include "common.h"
#include <signal.h>

static volatile sig_atomic_t quit = 0;

void sigint_handler(int signum)
{
    quit = 1;
}

struct shm *sharedMemory_Server(int *shmfd);
void cleanSharedMemory_Server(struct shm *shm, int shmfd);
sem_t *initializeSemaphore_Server(const char *sem_name, unsigned int init_value);
void cleanSemaphore_Server(sem_t *semaphore, const char *sem_name);

int main(int argc, char **argv)
{
}