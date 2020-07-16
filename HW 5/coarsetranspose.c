/*
Homework Group- 08
Name: Parth Shah				Name: Junaid Magdum
UCINetId: parths4				UCINetId: jmagdum
Student Id: 54809514			Student Id: 94685845
*/

// To compile on openlab: gcc -lm -pthread coarsetranspose.c 


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <pthread.h> 


int n=0;
int A[10000][10000];
int delta;
//Shared variable counter
int counter=0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* transposeMatrix(void* limit){
    int l = *(int *) limit;
    while(1){

        //Acquiring the lock to update the shared variable
        pthread_mutex_lock(&mutex);

        //Storing the shared variable in a local variable for computation
        int c= counter;

        //Updating the shared variable to be accessed by other threads
        counter+=delta;

        //Releasing the lock on shared variable
        pthread_mutex_unlock(&mutex);

        //If the max value is reached exit the thread 
        if(c>=l)
            pthread_exit(NULL);

        int d;
        for(d=0;d<delta;d++,c++){
            if(c>=l)
                pthread_exit(NULL);
            int i,j;
            //Calculate value of i and j i.e. the numbers to be transposed
            i= c/n;
            j= c%n;

            //Swap the elements only when the element is on upper triangular of A i.e. j>i
            if(j>i){
                int temp=A[i][j];
                A[i][j]=A[j][i];
                A[j][i]=temp;
            }
        }
    }
    
}

void printMatrix(){
    int i,j;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            printf("%d\n", A[i][j]);
        }
        //printf("\n");
    }
}

void intializeMatrix(){
    int i,j;
    int val=0;
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            A[i][j]=val++;
        }
    }
}

int main(int argc, char** argv){
    struct timespec start, finish;
    double elapsed;
    clock_gettime(CLOCK_MONOTONIC, &start);

    //Get Size of matrix as input from command line
    n = atoi(argv[1]);

    int l=n*n;

    //Get thread count
    int threadCount= atoi(argv[2]);

    //Get delta from command line
    delta = atoi(argv[3]);
    
    //Initalizing an array of threads, size is the input from command line
    pthread_t thread[threadCount];

    //Initalize matrix A with values 0-(n^2-1)
    intializeMatrix();
    
    int i=0;
    for(i=0;i<threadCount;i++){
        //Creating threads and making each thread execute isPrime
        pthread_create(&thread[i], NULL, transposeMatrix, &l);
    }
    
    for(i=0;i<threadCount;i++){
        // Waiting for each thread to complete its processing
        pthread_join(thread[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    //printf("\nTime Elapsed: %0.4lf\n",elapsed);

    //Function to Print the matrix
    if(n<=8)
        printMatrix();

    return 0;
}