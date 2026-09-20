#include "circularbuff.h"
#include "semaphore.h"

/* Per-process state*/
static struct shm *circularbuff = NULL;
static sem_t *sem_free = NULL;
static sem_t *sem_used = NULL;
static sem_t *sem_write = NULL;

/*Read the circubuff.h for documentation */
void initializeCircularBuffer(struct shm *shm, bool isServer)
{
    circularbuff = shm;

    if (isServer)
    {
        sem_free = initializeSemaphore_Server(FREE_SPACE_SEMAPHORE, FREE_SPACE_SEMAPHORE_SIZE);
        sem_used = initializeSemaphore_Server(USED_SPACE_SEMAPHORE, USED_SPACE_SEMAPHORE_SIZE);
        sem_write = initializeSemaphore_Server(WRITE_SPACE_SEMAPHORE, WRITE_SPACE_SEMAPHORE_SIZE);
    }
    else
    {
        sem_free = initializeSemaphore_Client(FREE_SPACE_SEMAPHORE);
        sem_used = initializeSemaphore_Client(USED_SPACE_SEMAPHORE);
        sem_write = initializeSemaphore_Client(WRITE_SPACE_SEMAPHORE);
    }
}

/*Read the circubuff.h for documentation */
int closeCircularBuffer(bool isServer)
{
    if (isServer)
    {
        cleanSemaphore_Server(sem_free, FREE_SPACE_SEMAPHORE);
        cleanSemaphore_Server(sem_used, USED_SPACE_SEMAPHORE);
        cleanSemaphore_Server(sem_write, WRITE_SPACE_SEMAPHORE);
    }
    else
    {
        cleanSemaphore_Client(sem_free);
        cleanSemaphore_Client(sem_used);
        cleanSemaphore_Client(sem_write);
    }

    /* The mapping itself is released by cleanSharedMemory_*; do NOT free() it. */
    circularbuff = NULL;
    sem_free = sem_used = sem_write = NULL;

    return 0;
}

/*Read the circubuff.h for documentation */
void writeCircularBuffer(struct GRAPH_EDGE edge)
{
    sem_wait(sem_free);
    sem_wait(sem_write);
    circularbuff->data[circularbuff->writehead] = edge;
    circularbuff->writehead = (circularbuff->writehead + 1) % MAX_BUFF_SIZE;
    sem_post(sem_write);
    sem_post(sem_used);
}

/*Read the circubuff.h for documentation */
struct GRAPH_EDGE readCircularBuffer(void)
{
    struct GRAPH_EDGE rv;

    sem_wait(sem_used);
    rv = circularbuff->data[circularbuff->readhead];
    circularbuff->readhead = (circularbuff->readhead + 1) % MAX_BUFF_SIZE;
    sem_post(sem_free);

    return rv;
}