//
// Consumer
// consumer.c file
// David J Tinley
// 11/05/2023
// This is the source file for the implementation of the consumer process.
//

#include "buffer.h"

void *consumer(void *args);  // Declare function prototype
struct shared_buffer *sh_bf; // Declare pointer to shared_buffer struct

int main(void) {
  int file_descriptor;          // Int for shm_open
  const char *name;             // Passed to shm_open
  name = "/hello_shared123321"; // Name used for file descriptor

  file_descriptor = shm_open(name, O_RDWR, 0); // Open shared memory file
  if (file_descriptor == -1) {                 // Error checking for shm_open
    shm_unlink(name);
    perror("Error: shm_open consumer\n");
    printf("Error: file_descriptor\n");
    exit(EXIT_FAILURE);
  }

  sh_bf = mmap(NULL, sizeof(*sh_bf), PROT_READ | PROT_WRITE, MAP_SHARED,
               file_descriptor, 0); // Mapping sh_bf to shared memory
  if (sh_bf == MAP_FAILED) {        // Error checking mmap
    shm_unlink(name);
    munmap(sh_bf, sizeof(*sh_bf));
    printf("Error: MAP_FAILED\n");
    exit(1);
  }

  pthread_t thread[THREAD_COUNT];                 // Initializing array of
                                                  // threads

  pthread_mutex_init(&sh_bf->mutex_buffer, NULL); // Initializing mutex_buffer
                                                  // from shared_memory struct

  for (int i = 0; i < THREAD_COUNT; ++i) {  // Creating threads with consumer function
    pthread_create(&thread[i], NULL, &consumer, NULL);
  }

  for (int i = 0; i < THREAD_COUNT; ++i) { // Joining threads together
    pthread_join(thread[i], NULL);
  }

  shm_unlink(name);              // Unlinking file descriptor
  munmap(sh_bf, sizeof(*sh_bf)); // Unmapping shared memory
  return 0;                      // Exit program
}

void *consumer(void *args) { // Consumer function
  int tracker = 0;           // For tracking while loop
  int current_number;        // For printing current integer in buffer
  while (tracker < 20) {     // Looping 20 times (arbitrary number)

    sem_wait(&sh_bf->semaphoreFull);                  // Have full semaphore wait
    pthread_mutex_lock(&sh_bf->mutex_buffer);         // Lock mutex_buffer
    current_number = sh_bf->buffer[sh_bf->count - 1]; // Decrement buffer count
    sh_bf->count--;
    pthread_mutex_unlock(&sh_bf->mutex_buffer);       // Unlock mutex_buffer
    sem_post(&sh_bf->semaphoreEmpty);                 // Post empty semaphore
    printf("Consuming %d from buffer...\n", current_number); // Print number
                                                             // being consumed
    ++tracker;  // Increment tracker
    sleep(1);   // Wait 1 second
  }
}
