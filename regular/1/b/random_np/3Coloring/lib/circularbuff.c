#include "circularbuff.h"
#include "semaphore.h"
#include <semaphore.h>
#include <stdlib.h>

static struct shm *circularbuff;

/*Read the circubuff.h for documentation */
void initializeCircularBuffer(struct shm *shm)
{
    circularbuff = shm;
}

/*Read the circubuff.h for documentation */
int closeCircularBuffer(bool isServer)
{
    free(circularbuff);

    return circularbuff == NULL ? 0 : -1;
}

/*Read the circubuff.h for documentation */
void writeCircularBuffer(int value)
{
    sem_wait(circularbuff->free);
    sem_wait(circularbuff->write);
    circularbuff->data[circularbuff->writehead] = value;
    sem_post(circularbuff->used);
    sem_post(circularbuff->write);
    circularbuff->writehead += 1;
    circularbuff->writehead %= MAX_BUFF_SIZE;
}

/*Read the circubuff.h for documentation */
int readCircularBuffer()
{
    int rv;
    sem_wait(circularbuff->used);
    rv = circularbuff->data[circularbuff->readhead];
    sem_post(circularbuff->free);
    circularbuff->readhead = (circularbuff->readhead + 1) % MAX_BUFF_SIZE;
    return rv;
}
