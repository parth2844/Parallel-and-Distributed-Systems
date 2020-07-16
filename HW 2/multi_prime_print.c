/*
Homework Group- 10
Name: Parth Shah				Name: Prasad Sawant
UCINetId: parths4				UCINetId: pvsawant
Student Id: 54809514			Student Id: 78123771
*/

//Execute on openlab using "gcc -lm -pthread multi_prime_print.c -o multi" 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <pthread.h> 

//Shared variable value stores the number that is to be checked if it is prime or not
long value=2;

//The maximum limit for the variable value 
long l = 10000000;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* isPrime(void* limit){
    
    while(1){

        //Acquiring the lock to update the shared variable
        pthread_mutex_lock(&mutex);

        //Storing the shared variable in a local variable for computation
        long current= value;
        //Updating the shared variable to be accessed by other threads
        value++;

        //Releasing the lock on shared variable
        pthread_mutex_unlock(&mutex);

        //If the max value is reached exit the thread 
        if(current>l)
            pthread_exit(NULL);

        long i;

        // Checking if the number is prime or not
        int flag=0;
        for(i=2;i<=sqrt(current);i++){
            if((current%i)==0){
                //If this condition is true means that number is not prime and set the flag to 1.
                flag=1;
                break;
            }
        }
        //If flag is 0 that means no factor for the number was found and hence it is prime
        if(flag==0)
            printf("%ld\n",current);
    }
}

int main(int argc, char** argv){
    struct timespec start, finish;
    double elapsed;
    clock_gettime(CLOCK_MONOTONIC, &start);

    // Get the thread count from command line
    int threadCount= atoi(argv[1]);

    //Initalizing an array of threads, size the input from command line
    pthread_t thread[threadCount];
    
    
    int i=0;
    for(i=0;i<threadCount;i++){
        //Creating threads and making each thread execute isPrime
        pthread_create(&thread[i], NULL, isPrime, NULL);
    }
    
    for(i=0;i<threadCount;i++){
        // Waiting for each thread to complete its processing
        pthread_join(thread[i], NULL);
    }

    //Calculating the elapsed time 
    //Reffered to https://stackoverflow.com/questions/2962785/c-using-clock-to-measure-time-in-multi-threaded-programs
    //to get the most accurate time measurement
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("\nTime Elapsed: %0.4lf\n",elapsed);
    
    return 0;
}