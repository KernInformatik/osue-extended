/**
 * @file generator.c
 * @author Kernkraftwerk (kernkraftdev@hotmail.com)
 * @brief generates vertices
 * @version 0.1
 * @date 2026-09-14
 *
 * @copyright Copyright (c) 2026
 *
 * @details attention this is just the first implementation and not the final version
 *
 */

/**
 * @details Multiple generator pro-
cesses generate random solutions to the problem and report their solutions to one supervisor process.The supervisor
process remembers the best solution so far.
The processes communicate with eachother by means of a circular buffer, which is implemented using shared semaphores and
a shared memory
 *
 */

#include "common.h"

/**
 * @brief Write the given arguments into a EDGLE_LIST
 *
 * @param argc
 * @param argv
 * @return struct EDGE_LIST
 */
static struct EDGE_LIST parse(int argc, char **argv)
{
    /** validate the given input */
    if ((argc - 1) < 1)
    {
        error("too few arguments");
    }

    if ((argc - 1) > MAX_INPUT)
    {
        error("too many arguments");
    }

    for (int i = 0; i < argc; ++i)
    {
        for (int j = 0; j < argc; ++j)
        {
            if ((i != j) && strcmp(argv[i], argv[j]) == 0)
            {
                error("duplicate arguments");
            }
        }
    }

    for (int i = 1; i < argc; ++i)
    {
        if (strchr(argv[i], '-') == NULL)
        {
            error("Wrong type of input, see synopsis");
        }
        if (strchr(argv[i], '-') != strrchr(argv[i], '-'))
        {
            error("Wrong type of input, see synopsis");
        }
    }

    for (int i = 1; i < argc; ++i)
    {
        char *s = strdup(argv[i]);

        if (s == NULL)
        {
            error("strdup");
        }

        char *endptr;
        char *first_vertices = strtok_r(s, "-", &endptr);
        char *second_vertices = strtok_r(NULL, "-", &endptr);

        if (first_vertices == NULL || second_vertices == NULL)
        {
            error("strtok_r");
        }
        free(s);
    }

    /** write the given input in a EDGE_LIST and return it */
    struct EDGE_LIST rv = {0};
    rv.length = (ssize_t)(argc - 1);
    for (int i = 1; i < argc; ++i)
    {
        char *s = strdup(argv[i]);
        if (s == NULL)
            error("strdup");

        char *endptr;
        char *first_vertices = strtok_r(s, "-", &endptr);
        char *second_vertices = strtok_r(NULL, "-", &endptr);

        rv.edgeList[i - 1].from.name = (size_t)strtoul(first_vertices, NULL, 10);
        rv.edgeList[i - 1].to.name = (size_t)strtoul(second_vertices, NULL, 10);

        free(s);
    }

    return rv;
}
/**
 * @brief Randomized solution approach for the 3 - Coloring problem
 *
 * @param input
 * @return struct EDGE_LIST
 */
static struct EDGE_LIST solve(struct EDGE_LIST input)
{
    /*Create an array of Nodes */
    struct VERTICES verticesArray[MAX_EDGES * 2]; // 2048
    size_t index = 0;

    for (int i = 0; i < input.length; ++i)
    {
        struct VERTICES first = input.edgeList[i].from;
        struct VERTICES second = input.edgeList[i].to;

        verticesArray[index++] = first;
        verticesArray[index++] = second;
    }

    /* Color the vertices */
    for (size_t i = 0; i < index; i++)
    {
        verticesArray[i].color = rand() % 3;
    }

    /* Color the EDGES*/
    for (ssize_t i = 0; i < input.length; i++)
    {
        for (size_t j = 0; j < index; j++)
        {
            if (input.edgeList[i].from.name == verticesArray[j].name)
            {
                input.edgeList[i].from.color = verticesArray[j].color;
            }
            if (input.edgeList[i].to.name == verticesArray[j].name)
            {
                input.edgeList[i].to.color = verticesArray[j].color;
            }
        }
    }

    struct EDGE_LIST solution = {0};
    ssize_t indexSolution = 0;

    for (int i = 0; i < input.length; i++)
    {
        struct EDGE e = input.edgeList[i];

        if (e.from.color == e.to.color)
            solution.edgeList[indexSolution++] = e;
    }
    solution.length = indexSolution;
    indexSolution = 0;
    return solution;
}

/**
 * @brief Writes the solution in a circular buffer via semaphores and shared memory
 * @details see common.h
 * @param solution
 * @param shmp
 */
static void writeSolution(struct EDGE_LIST solution, struct Shm *shmp)
{
    if (solution.length < MAX_EDGES)
    {
        shmp->circular_buffer[shmp->writeIndex] = solution;
        shmp->writeIndex = (shmp->writeIndex + 1) % (buff_length);
    }

    if (sem_post(&shmp->writeMutex) == -1)
        error("sem_post()");
}

int main(int argc, char **argv)
{
    struct EDGE_LIST input = parse(argc, argv);

    int fd = shm_open(SHM_PATH, O_RDWR, 0);

    assert(fd != -1);

    struct Shm *shmp = mmap(NULL, sizeof(*shmp), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (shmp == MAP_FAILED)
    {
        error("mmap");
        exit(EXIT_FAILURE);
    }

    while (!shmp->killprocess)
    {
        if ((sem_wait(&shmp->numFree) == -1) && (errno != EINTR))
        {
            error("sem_wait");
        }

        struct EDGE_LIST solution = solve(input);
        writeSolution(solution, shmp);

        if (sem_post(&shmp->numUsed) == -1)
        {
            error("sem_post");
        }
    }

    if (munmap(shmp, sizeof(*shmp)) == -1)
    {
        error("munmap");
    }

    if (close(fd) == -1)
    {
        error("close");
    }

    exit(EXIT_SUCCESS);
}
