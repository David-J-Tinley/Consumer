//
// Producer
// producer.c file
// David J Tinley
// 11/05/2023
// This is the source file for the producer process
//

#include "buffer.h"

struct shared_buffer *sh_bf; // Declare shared_buffer struct
void *producer(void *args);  // Producer function prototype

int main(void) {
  int file_descriptor, file_truncate;        // Int's for shm_open and ftruncate
  const char *name;                          // Passed to shm_open
  name = "/hello_shared123321";              // Name for file descriptor
  off_t size = sizeof(struct shared_buffer); // Size of shared_buffer struct

  shm_unlink(name);                          // Unlink old file descriptors

  // Creating shared memory file
  file_descriptor = shm_open(name, O_CREAT | O_EXCL | O_RDWR, 0666);
  if (file_descriptor == -1) { // Error checking shm_open
    shm_unlink(name);
    perror("Error: shm_open producer");
    printf("Error: file_descriptor\n");
    exit(EXIT_FAILURE);
  }

  // Setting file descriptor size to size of shared_buff struct
  file_truncate = ftruncate(file_descriptor, size);
  if (file_truncate != 0) { // Error checking size assignment
    shm_unlink(name);
    printf("Error: ftruncate\n");
    exit(EXIT_FAILURE);
  }

  // Mapping file descriptor to shared memory
  sh_bf = mmap(NULL, sizeof(*sh_bf), PROT_READ | PROT_WRITE, MAP_SHARED,
               file_descriptor, 0);
  if (sh_bf == MAP_FAILED) { // Error checking mmap
    shm_unlink(name);
    munmap(sh_bf, sizeof(*sh_bf));
    printf("Error: MAP_FAILED\n");
    exit(EXIT_FAILURE);
  }

  // Initializing and error checking semaphoreEmpty
  if (sem_init(&sh_bf->semaphoreEmpty, 0, BUFFER_SIZE) == -1) {
    printf("Error: sem_init(semaphoreEmpty)\n");
    exit(EXIT_FAILURE);
  }
  // Initializing and error checking semaphoreFull
  if (sem_init(&sh_bf->semaphoreFull, 0, 0) == -1) {
    printf("Error: sem_init(semaphoreFull)\n");
    exit(EXIT_FAILURE);
  }

  srand(time(NULL));              // Setting random seed time for rand()
  pthread_t thread[THREAD_COUNT]; // Initializing array of threads
  pthread_mutex_init(&sh_bf->mutex_buffer, NULL); // Initializing mutex_buffer
                                                  // from shared memory
  for (int i = 0; i < THREAD_COUNT; ++i) { // Creating threads with producer
                                           // function
    pthread_create(&thread[i], NULL, &producer, NULL);
  }
  for (int i = 0; i < THREAD_COUNT; ++i) { // Joining threads together
    pthread_join(thread[i], NULL);
  }

  shm_unlink(name);                    // Unlink file descriptor
  munmap(sh_bf, sizeof(*sh_bf));       // Unmapping shared memory
  sem_destroy(&sh_bf->semaphoreEmpty); // Destroying semaphores
  sem_destroy(&sh_bf->semaphoreFull);  // Destroying semaphores

  return 0;
}

void *producer(void *args) {         // Producer function
  int tracker = 0;                   // For tracking while loop
  while (tracker < 20) {
    int rand_integer = rand() % 100; // Generate rand int from 0 - 99
    sleep(1);                        // Wait 1 second

    while (1) { // Add to the buffer

      // Have semaphore try waiting and keep trying to produce
      if (sem_trywait(&sh_bf->semaphoreEmpty) == 0) {

        pthread_mutex_lock(&sh_bf->mutex_buffer);
        printf("Inserting %d into buffer...\n", rand_integer);
        sh_bf->buffer[sh_bf->count] = rand_integer;
        sh_bf->count++;
        pthread_mutex_unlock(&sh_bf->mutex_buffer);
        sem_post(&sh_bf->semaphoreFull);
        break;

      } else {
        usleep(100000); // If still full, wait again
      }
    }
    ++tracker;          // Increment tracker
  }
}
