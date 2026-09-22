#include "../lib/common.h"
#include "../lib/graph.h"
#include "../lib/sem_lib.h"
#include <semaphore.h>

struct GRAPH_EDGE_LIST parseInput(int argc, char **argv)
{

    struct GRAPH_EDGE_LIST rv;

    /* i = 1 because argc also counts the white space which is mentally deranged, took me a while to realize it*/
    for (int i = 1; i < argc; i++)
    {

        char *savePtr;
        char *first = strtok_r(argv[i], "-", &savePtr);
        char *second = strtok_r(NULL, "-", &savePtr);
        rv.edgeList[i - 1].from.name = strtol(first, NULL, 10);
        rv.edgeList[i - 1].to.name = strtol(second, NULL, 10);
    }
    rv.length = (size_t)argc - 1;

    return rv;
}

void colorizeVertex(struct GRAPH_EDGE_LIST *edgeList)
{
    for (int i = 0; i < edgeList->length; i++)
    {
        edgeList->edgeList[i].from.color = rand() % 3;

        edgeList->edgeList[i].to.color = rand() % 3;
    }
}

struct GRAPH_EDGE_LIST generateSolution(struct GRAPH_EDGE_LIST edgeList)
{
    struct GRAPH_EDGE_LIST solution;

    int counter = 0;

    colorizeVertex(&edgeList);

    for (int i = 0; i < edgeList.length; i++)
    {
        if (edgeList.edgeList[i].from.color == edgeList.edgeList[i].to.color)
        {
            solution.edgeList[counter++] = edgeList.edgeList[i];
        }
    }
    solution.length = counter;
    return solution;
}

void writeSolution(struct GRAPH_EDGE_LIST edgeList, struct shm *sharedMemory, sem_t *free, sem_t *used, sem_t *write)
{
    sem_wait(free);
    sem_wait(write);
    sharedMemory->data[sharedMemory->writehead] = edgeList;
    sem_post(write);
    sem_post(used);
    sharedMemory->writehead++;
    sharedMemory->writehead %= MAX_BUFF_SIZE;
}

int main(int argc, char **argv)
{
    int shmfd;
    struct shm *possibleSolution = sharedMemory_Client(&shmfd);
    sem_t *free, *used, *write;

    free = initializeSemaphore_Client(FREE_SPACE_SEMAPHORE);
    write = initializeSemaphore_Client(WRITE_SPACE_SEMAPHORE);
    used = initializeSemaphore_Client(USED_SPACE_SEMAPHORE);

    struct GRAPH_EDGE_LIST test = parseInput(argc, argv);
    writeSolution(generateSolution(test), possibleSolution, free, used, write);
}
