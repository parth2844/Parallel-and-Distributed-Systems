/*
Homework Group- 08
Name: Parth Shah				Name: Junaid Magdum
UCINetId: parths4				UCINetId: jmagdum
Student Id: 54809514			Student Id: 94685845
*/

// To compile on openlab: gcc uniproctranspose.c

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

int n=0;
int A[10000][10000];

void transposeMatrix(){
    int i,j;
    for(i=0;i<n;i++){
        for(j=i+1;j<n;j++){
            int temp=A[i][j];
            A[i][j]=A[j][i];
            A[j][i]=temp;
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

    //Initalize matrix A with values 0-(n^2-1)
    intializeMatrix();
    
    //Transpose Matrix A
    transposeMatrix();

    clock_gettime(CLOCK_MONOTONIC, &finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
    //printf("\nTime Elapsed: %0.4lf\n",elapsed);

    //Function to Print the matrix
    if(n<=8)
        printMatrix();

    return 0;
}