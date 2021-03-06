 /***********************************************************************               
* Program:                                                                              
*    Lab Threads
*    Brother Jones, CS 345                                                              
* Author:                                                                               
*    Preston Price
* Summary:                                                                              
*    Perform M x N matrix multiplication on two matrices using threads.
*    IMPORTANT: must use flag -pthread in order for it to compile
************************************************************************/

#include <iostream>
#include <iomanip> 
#include <pthread.h>
using namespace std;

#define M 5  // number of rows in matrix A                                              
#define K 6  // number of rows in matrix B -- number of columns in matrix A             
#define N 8  // number of columns in matrix B                                           
#define NUM_THREADS M*N


int A [M][K] = { {1,4,3,7,9,1}, {2,5,4,8,6,3}, {3,6,5,8,2,3}, {3,8,8,1,4,1}, {1,5,4,5,7,9} };
int B [K][N] = { {1,5,6,5,7,9,8,2}, {1,2,3,5,5,6,7,8}, {3,5,9,7,3,1,4,1}, {8,3,1,2,6,5,2,4}, {3,8,8,1,4,1,3,3}, {8,7,6,5,4,3,2,1} };
int C [M][N];  // this is where the answer will be placed  

/* structure for passing data to threads */       
struct ThreadData       
{          
   int i; /* row */          
   int j; /* column */       
};

void *calculate_cell( void *tData );

int main()
{
   /* an array of threads to be joined upon */       
   pthread_t workerThreads[NUM_THREADS];

   /* We have to create M * N worker threads */       
   for (int i = 0; i < M; i++) {        
      for (int j = 0; j < N; j++ ) {       
         ThreadData* tData = new ThreadData;             
         tData->i = i;             
         tData->j = j;       
         /* Now create the thread passing it tData as a parameter */ 
         pthread_create( &workerThreads[(i*N)+j], NULL, calculate_cell, tData);
      }       
   }      

   // Join threads together
   for (int i = 0; i < NUM_THREADS; i++)          
      pthread_join(workerThreads[i], NULL);

   // Output result of matrix
   cout << "The product of matrices A and B is:\n";
   for (int i=0; i<M; i++) {
      for (int j=0; j<N; j++) {
         cout << setw(8); 
         cout << C[i][j];
      }
      cout << endl;
   }

   return 0;
}

void *calculate_cell( void *tData )
{
   struct ThreadData *new_tData = (struct ThreadData*) tData;

   int sum = 0;
   int row = new_tData->i;
   int column = new_tData->j;

   // Matrix multiplication for cell C[row][column]
   for (int i=0; i<K; i++) {
      sum += A[row][i]*B[i][column];
   }

   C[row][column] = sum;

   pthread_exit(0);
}

