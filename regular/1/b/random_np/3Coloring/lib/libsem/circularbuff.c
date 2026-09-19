#include "circularbuff.h"
#include "semaphore.h"

static struct shm *circularbuff;

/*Read the circubuff.h for documentation */
void
initializeCircularBuffer(struct shm *shm)
{
	circularbuff = shm;
}

/*Read the circubuff.h for documentation */
int
closeCircularBuffer(bool isServer)
{
	free(circularbuff);

	return circularbuff == NULL ? 0 : -1;
}

/*Read the circubuff.h for documentation */
void
writeCircularBuffer(struct GRAPH_EDGE edge)
{
	sem_wait(circularbuff->free);
	sem_wait(circularbuff->write);
    /*@deprecated  circularbuff->data[circularbuff->writehead] = value;*/
	circularbuff->data[circularbuff->writehead] = edge;
	circularbuff->writehead += 1;
	circularbuff->writehead %= MAX_BUFF_SIZE;
	sem_post(circularbuff->used);
	sem_post(circularbuff->write);
}

/*Read the circubuff.h for documentation */
struct GRAPH_EDGE
readCircularBuffer()
{
	struct GRAPH_EDGE rv;
	sem_wait(circularbuff->used);
    /*@deprecated rv = circularbuff->data[circularbuff->readhead]; */
	circularbuff->readhead = (circularbuff->readhead + 1) % MAX_BUFF_SIZE;
	sem_post(circularbuff->free);
	return rv;
}
