/**
 * @file common.h
 * @author Kenrkraftwerk (kernkraftdev@hotmail.com)
 * @brief a library for macros and libraries
 * @details This module wraps shm_open(3), mmap(2), and sem_open(3) into a small
 *          set of create/connect/destroy/disconnect functions. A server process
 *          must call the *_create functions before any client calls *_connect,
 *          since the server is responsible for allocating the underlying
 *          resources. There must be exactly one server; there may be many
 *          clients.
 * @version 0.1
 * @date 2026-09-15
 *
 * @copyright Copyright (c) 2026
 *
 */
#ifndef COMMON_H
#define COMMON_H

/*Checks and balances(?) */
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <limits.h>
#include <unistd.h>

/*IPC Process communication and shared memory */
#include <fcntl.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

/*Standard libraries */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/*String libary */
#include <string.h>
#include <strings.h>

/*Misc*/
#include <regex.h>
#include <time.h>

#define error(msg)							\
    do									\
    {									\
        perror(msg);							\
        printf("Error (%d): %s\n", errno, strerror(errno));		\
        exit(EXIT_FAILURE);						\
    } while (true)


#define MAX_BUFF_SIZE 2048
struct shm {
	size_t data[MAX_BUFF_SIZE];
	size_t readhead;
	size_t writehead;
  sig_atomic_t alive;
};

/*SHARED MEMORY OPTIONS */
#define SHM_NAME "/eXXXXXXXX_shm"
#define SHM_OFLAG_SERVER O_RDWR | O_CREAT | O_EXCL
#define SHM_OFLAG_CLIENT O_RDWR
#define SHM_MODE S_IRWXU
#define MMP_PROT_FLAGS PROT_READ | PROT_WRITE
#define MMP_FLAGS MAP_SHARED
#define MMP_OFFSET 0
/*END SHARED MEMORY OPTIONS*/

/*SEMAPHORE OPTIONS */
/**
 * @brief For the circular buffer use 3 semaphores, sem_t *free, sem_t *used, sem_t *write. Each of these semphores are
needed for proper IPC communication between processess
 *
 */
#define FREE_SPACE_SEMAPHORE "/eXXXXXXXX_sem_free"
#define FREE_SPACE_SEMAPHORE_SIZE MAX_BUFF_SIZE

#define USED_SPACE_SEMAPHORE "/eXXXXXXXX_sem_used"
#define USED_SPACE_SEMAPHORE_SIZE 0

#define WRITE_SPACE_SEMAPHORE "/eXXXXXXXX_sem_write" // mutex
#define WRITE_SPACE_SEMAPHORE_SIZE 1

#define OPEN_SEM_MODE S_IRWXU
#define OPEN_SEM_MODE_SERVER O_RDWR | O_CREAT | O_EXCL
#define OPEN_SEM_MODE_CLIENT O_RDWR
/*END SEMAPHORE OPTIONS */

/*SEMAPHORE ERROR*/
#define OPENING_SHM_ERROR_SERVER "Creating shared memory failed"
#define OPENING_SHM_ERROR_CLIENT "Opening shared memory failed. Ensure a supervisor is running"

#define TRUNCATING_SHM_ERROR "Initializing shared memory failed"
#define MAPPING_SHM_ERROR "Mapping shared memory failed"
#define CLOSING_SHM_ERROR "Closing shared memory file failed"
#define UNMAPPING_SHM_ERROR "Unmapping shared memory failed"
#define UNLINKING_SHM_ERROR "Unlinking shared memory failed"

#define OPENING_SEM_ERROR "Opening semaphore failed"
#define CLOSING_SEM_ERROR "Closing semaphore failed"
#define UNLINKING_SEM_ERROR "Unlinking semaphore failed"
/*END SEMAPHORE ERROR*/


#endif


