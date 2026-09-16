#ifndef COMMON
#define COMMON

#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>
#include <limits.h>
#include <unistd.h>

#include <semaphore.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <strings.h>

#include <time.h>

#define buff_length (32)
#define SHM_PATH "/12519647shm"
#define MAX_EDGES 1028
#define MAX_SOLUTION 128

#define error_verbose(msg, program_name)                                                                               \
    do                                                                                                                 \
    {                                                                                                                  \
        printf("%s\n", msg);                                                                                           \
        fprintf(stderr, "Usage: %s [-r] [file1]...\n", program_name);                                                  \
        printf("Error (%d): %s\n", errno, strerror(errno));                                                            \
        exit(EXIT_FAILURE);                                                                                            \
    } while (0)

#define error(msg)                                                                                                     \
    do                                                                                                                 \
    {                                                                                                                  \
        perror(msg);                                                                                                   \
        exit(EXIT_FAILURE);                                                                                            \
    } while (true)

/**
 * @brief The Datastructures to implement: EDGE, VERTICES, COLOR, EDGE_LIST, INITIALIZE_SHM
 *
 */
enum COLOR_VERTICES
{
    RED_ = 0,
    BLUE_ = 1,
    GREEN_ = 2
};

struct VERTICES
{
    size_t name;
    enum COLOR_VERTICES color;
};

struct EDGE
{
    struct VERTICES from;
    struct VERTICES to;
};

struct EDGE_LIST
{
    struct EDGE edgeList[MAX_EDGES];
    ssize_t length;
};

struct Shm
{
    bool killprocess;
    size_t numGenerators;

    struct EDGE_LIST circular_buffer[buff_length];
    size_t readIndex;
    size_t writeIndex;

    sem_t writeMutex; // write mutex for generators
    sem_t numUsed;    // num of used indices in buffer (= supervisor should read)
    sem_t numFree;    // num of used indices in buffer (= generator can write)
};

#endif