/*
Homework Group- 10
Name: Parth Shah				Name: Prasad Sawant
UCINetId: parths4				UCINetId: pvsawant
Student Id: 54809514			Student Id: 78123771
*/

//Execute on openlab using command "gcc -lm  single_prime_print.c -o sp"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

//Returns 1 for Prime numbers and 0 otherwise
int isPrime(int n){
    int i;
    for(i=2;i<=sqrt(n);i++){
        if((n%i)==0)
            return 0;
    }
    return 1;
}

int main(){
    struct timespec start, finish;
    double elapsed;

    clock_gettime(CLOCK_MONOTONIC, &start);
    long i=2;
    long limit = pow(10,7);
    
    // This loop iterates over numbers in range 10^7 
    while (i<limit){
        if(isPrime(i))  //The function isPrime checks if the input number is Prime or not
            printf("%ld\n", i);
        i++;
    }
    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("\nTime Elapsed: %0.4lf\n",elapsed);
    return 0;
}