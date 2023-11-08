//
// Header
// buffer.h file
// David J Tinley
// 11/05/2023
// Header file for the producer and consumer source files
//

#ifndef _BUF_HEADER_
#define _BUF_HEADER_

#include <ctype.h> // Declaring necessary header files
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define BUFFER_SIZE  2 // Setting buffer size to only hold 2 items
#define THREAD_COUNT 3 // Setting thread count to 3 (arbitrary)

struct shared_buffer { // Buffer shared between processes
    int             buffer[BUFFER_SIZE]; // Array of integers
    int             count;               // Used for counting array index
    pthread_mutex_t mutex_buffer;        // Mutex thread used for pthreads
    sem_t           semaphoreEmpty;      // Empty semaphore
    sem_t           semaphoreFull;       // Full semaphore
};

#endif
