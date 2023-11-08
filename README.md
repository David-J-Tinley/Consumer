# Producer/Consumer Program in C

By: David J Tinley <br />
For: Operating Systems Project 1

## TODO:
- [ ] Create a Unix Makefile that builds and runs both programs
- [ ] Figure out how to translate source code to MacOS.
- [ ] Update README

## Introduction

This README provides an overview of the producer/consumer program implemented
in C. It describes the purpose of the program, how to use it, and key details
about the header file `buffer.h`, `producer.c`, and `consumer.c` source files.
Overall I was able to get the programs to work successfully but I am sure the
code could be optimized better given more time.

## Project Description

The producer/consumer program is designed to demonstrate interprocess
communication and synchronization using shared memory and semaphores.
The program uses a shared buffer to illustrate the producer and consumer
processes operating concurrently. The producer file adds random integers until
the buffer is full and the consumer file consumes the integers. The code for
this program is written in C.

## Header File

The provided header file, `buffer.h`, is an essential component of the project.
It contains the necessary includes, data structures, and constants required for
the program. I declared the `BUFFER_SIZE` as 2 since the instructions said that
our shared memory table should only hold 2 items at a time. I also made the
`THREAD_COUNT` as 3. This was just an arbitrary number selected to make sure there
were multiple threads working.

## Producer (producer.c)

The `producer.c` file is where producer process is defined and implemented.
It includes the producer function, which simulates the behavior of a producer
thread. The producer file is where the
shared memory file is created and where the semaphores are initialized. I have
the actual producer function just loop 20 times to show that it is working
correctly. I have it set up to wait when the buffer is full and keep checking to
see if there is room available. If there is it will start adding random integers
to the buffer array. I use the sleep functions so the program doesn't execute so
fast that you cannot follow it. At the end of the main function the mapping,
semaphores, and threads are cleaned up.

## Consumer (consumer.c)

The `consumer.c` file is where the consumer process is defined and implemented.
It includes the consumer function that simulates the behavior of a consumer
thread. The consumer program first tries to open the shared memory file and if
it is successful it then maps it to the file descriptor. It has almost identical
implementation to the producer file beyond this. Where it creates threads but
with the consumer function.

## Usage

The program can be used to simulate the producer/consumer problem and study
interprocess communication and synchronization. To use these two processes from
the command line you would input `./producer & ./consumer &` into the prompt.
I have found that it tends to work better if there is a slight pause between
when `./producer` is executed and when `./consumer` is executed. You can create
this pause by running `./producer & sleep 1s; ./consumer &` in the terminal.
I think this is from the producer needing a chance to create the shared memory
before the consumer returns a `file not found` error. Or you could just run each
program from separate terminal instances.

## Compilation

To compile the producer/consumer program, follow these general steps:

1. To compile the producer program use `gcc producer.c -lpthread -lrt -o
   producer`. This will create an executable called `producer`.

2. To compile the consumer program use `gcc consumer.c -lpthread -lrt -o
   consumer`. This will create an executable called `consumer`.
