/**
 * @file generator.c
 * @author Kernkraftwerk (kernkraftdev@hotmail.com)
 * @brief generates vertices
 * @version 0.1
 * @date 2026-09-14
 *
 * @copyright Copyright (c) 2026
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

    for (int i = 0; i <= argc; ++i)
    {
        char *s = strdup(argv[i]);

        if (s == NULL)
        {
            error("strdup");
        }
        char *endptr;
        char *first_vertices = strtok_r(s, "-", &endptr);
        char *second_vertices = strtok_r(NULL, "-", &endptr);

        rv.edgeList[i].from.name = strtoul(first_vertices, NULL, 10);
        rv.edgeList[i].to.name = strtoul(second_vertices, NULL, 10);
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
    for (int i = 0; i < index; i++)
    {
        verticesArray[i].color = rand() % 3;
    }

    /* Color the EDGES*/
    for (int i = 0; i < input.length; i++)
    {
        for (int j = 0; j < index; j++)
        {
            if (input.edgeList[i].from.name == verticesArray[j].name)
            {
                input.edgeList[i].from.color = verticesArray[j].color;
            }
        }
    }

    struct EDGE_LIST solution = {0};
    size_t indexSolution;

    for (int i = 0; i < input.length; i++)
    {
        struct EDGE e = input.edgeList[i];

        solution.edgeList[indexSolution++] = e;
    }
    solution.length = indexSolution;
    indexSolution = 0;
    return solution;
}

static void writeSolution(struct EDGE_LIST solution, struct Shm_t *shmp)
{
}

int main(int argc, char **argv)
{
    struct EDGE_LIST input = parse(argc, argv);
    struct EDGE_LIST solution = solve(input);
    exit(EXIT_SUCCESS);
}
