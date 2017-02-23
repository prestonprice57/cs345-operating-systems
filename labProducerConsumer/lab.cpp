/***********************************************************************
* Program:
*    Lab ProducerConsumer
*    Brother Jones, CS 345
* Author:
*    Preston Price
*
* Summary:
*    Solution to the Consumer-Producer problem using semaphores.
************************************************************************/

#include <semaphore.h>
#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <cstdlib>
#include <iomanip>

using namespace std;

typedef int bufferItem;
#define BUFFER_SIZE 5

bufferItem buffer[BUFFER_SIZE];
int front = 0;

pthread_mutex_t mutex;
sem_t empty;
sem_t full;

void *producer( void *param );
void *consumer( void *param );

/*******************************************
* Creates and initializes threads
*******************************************/
int main (int argc, char *argv[])
{
   // Check to see if command line arguments are correct
   if (argc != 4)
   {
      cout << "Please include command line arguments:\n"
           << "1) how long to sleep\n"
           << "2) # of producer threads\n"
           << "3) # of consumer threads\n";
      return 1;
   }

   int sleep_time;
   int producers;
   int consumers;
   sscanf(argv[1], "%d", &sleep_time);
   sscanf(argv[2], "%d", &producers);
   sscanf(argv[3], "%d", &consumers);

   // initialize semaphores and mutex
   pthread_mutex_init(&mutex, NULL);
   sem_init(&empty, 0, BUFFER_SIZE);
   sem_init(&full, 0, 0);

   // initialize producer and consumer threads
   pthread_t producerThreads[producers];
   pthread_t consumerThreads[consumers];

   cout << "Produced  by P#  Consumed  by C#\n"
        << "========  =====  ========  =====\n";

   // create producer threads
   for (long i = 0; i < producers; i++)
   {
      int error;
      error = pthread_create(&producerThreads[i], NULL, producer, (void*)i);
      if (error != 0)
      {
         cout << "Error creating producer thread: " << error << endl;
         return -1;
      }
   }

   // create consumer threads
   for (long j = 0; j < consumers; j++)
   {
      int error;
      error = pthread_create(&consumerThreads[j], NULL, consumer, (void*)j); 
      if (error != 0)
      {
         cout << "Error creating consumer thread: " << error << endl;
         return -1;
      }
   }

   sleep(sleep_time);

   for (int i = 0; i < producers; i++)
   {
      pthread_cancel(producerThreads[i]);
   }

   for (int j = 0; j < consumers; j++)
   {
      pthread_cancel(consumerThreads[j]);
   }
   
}

/*******************************************
* Creates a random number and places it in
* the buffer.
*******************************************/
void *producer (void *param)
{
   bufferItem itemProduced;
   while (true)
   {
      /* sleep for a random period of time */
      usleep(rand() % 150000);
      /* generate a random number */
      itemProduced = (rand() % 1000);

      sem_wait(&empty);
      pthread_mutex_lock(&mutex);

      /* insert item into shared global buffer and print what was done */
      int val;
      sem_getvalue(&full, &val);
      buffer[(val+front)%BUFFER_SIZE] = itemProduced;
      cout << setw(5);
      cout << itemProduced;
      cout << "      P" << (long) param+1 << endl;

      pthread_mutex_unlock(&mutex);
      sem_post(&full);
   }
}

/*******************************************
* Consumes the front item in the buffer
* and resets the front variable.
*******************************************/
void *consumer(void *param)
{
   bufferItem consumedItem;

   while (true)
   {
      /* sleep for a random period of time */
      usleep(rand() % 150000);

      sem_wait(&full);
      pthread_mutex_lock(&mutex);
      
      /* consume item from shared global buffer and print what was done */
      int tmp = buffer[front];
      front = (front+1)%BUFFER_SIZE;

      cout << setw(22);
      cout << tmp;
      cout << "      C" << (long) param+1 << endl;
      
      pthread_mutex_unlock(&mutex);
      sem_post(&empty);
   }
}
