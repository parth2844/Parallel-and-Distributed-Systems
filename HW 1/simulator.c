/*
Homework Group- 34
Name: Parth Shah				Name: Yasin Zuhuruddin
UCINetId: parths4				UCINetId: kzuhurud
Student Id: 54809514			Student Id: 11443054
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

// c doesn't have boolean, define my own
#ifndef _BOOL
#define _BOOL

#define bool int
#define false 0
#define true 1

#endif

// Credit for basic circular queue, adapted as needed
// https://www.geeksforgeeks.org/circular-queue-set-1-introduction-array-implementation/
struct Queue {
    int rear, front;

    int size;
    int numElements;
    int* arr;
    bool* contains; 
} q;

// "Constructor"
void Queue(int size) {
    q.front = q.rear = -1;
    q.size = size;
    q.numElements = 0;
    q.arr = (int*) malloc(size * sizeof(int));
    q.contains = (bool*) malloc(size * sizeof(bool));
    memset(q.arr, -1, q.size * sizeof(int));
    memset(q.arr, false, q.size * sizeof(bool));
}

// "Destructor"
void teardown() {
    free(q.arr);
    free(q.contains);
    q.front = q.rear = -1;
    q.size = -1;
    q.numElements = -1;
}

void Enqueue(int value) {
    if((q.front == 0 && q.rear == q.size - 1) || 
       (q.rear == (q.front - 1) % (q.size - 1))) {
        printf("Shouldn't be called\nQueue is full %d\n", q.numElements);
        exit(-1);
    }

    q.numElements++;
    q.contains[value] = true;

    if(q.front == -1) {
        q.front = q.rear = 0;
        q.arr[q.rear] = value;
        return;
    }

    if(q.rear == q.size - 1 && q.front != 0) {
        q.rear = 0;
        q.arr[q.rear] = value;
        return;
    }

    q.rear++;
    q.arr[q.rear] = value;
}

int Dequeue() {
    if(q.front == -1) {
        printf("Queue is empty exiting\n");
        exit(-1);
    }

    int toReturn = q.arr[q.front];
    q.arr[q.front] = -1;
    q.contains[toReturn] = false;
    q.numElements--;

    if(q.front == q.rear) {
        q.front = -1;
        q.rear = -1;
        return toReturn;
    }

    if(q.front == q.size - 1) {
        q.front = 0;
    } else {
        q.front++;
    }

    return toReturn;
}

// Used for O(1) lookup if a processor is 
// already on the waiting list
bool contains(int key) {
    return q.contains[key];
}

// Debug code
void DisplayQueue() { 
    if(q.front == -1) { 
        printf("\nQueue is Empty"); 
        return; 
    } 
    
    printf("\nElements in Circular Queue are: "); 
    int i;
    if(q.rear >= q.front) { 
        printf("%d \n", q.numElements);
        
        for ( i = q.front; i <= q.rear; i++) 
            printf("%d ", q.arr[i]); 
    } else { 
        printf("%d \n", q.numElements);
        for ( i = q.front; i < q.size; i++) 
            printf("%d ", q.arr[i]); 
  
        for ( i = 0; i <= q.rear; i++) 
            printf("%d ", q.arr[i]); 
    } 

    printf("\n");
} 

// Global arrays
#define NUM_MEMORY_BLOCKS 2048

int numProcessors = -1;
bool memory[NUM_MEMORY_BLOCKS];
int* processor = NULL;
int* accessCount = NULL; 
int* normalDist = NULL;

int uniformDistribution(int high) {
    int random = (int)rand();
    return (random % high);
}

// Method for normal distribution 
/*https://stackoverflow.com/a/28551411*/
double rand_normal(double mean, double stddev)
{//Box muller method
    static double n2 = 0.0;
    static int n2_cached = 0;
    if (!n2_cached)
    {
        double x, y, r;
        do
        {
            x = 2.0*rand()/RAND_MAX - 1;
            y = 2.0*rand()/RAND_MAX - 1;

            r = x*x + y*y;
        }
        while (r == 0.0 || r > 1.0);
        {
            double d = sqrt(-2.0*log(r)/r); // Compile using -lm
            double n1 = x*d;
            n2 = y*d;
            double result = n1*stddev + mean;
            n2_cached = 1;
            return result;
        }
    }
    else
    {
        n2_cached = 0;
        return n2*stddev + mean;
    }
}


//Generates normally distibuted random number
int normalDistribution(int high, int pno) {
    int random = round(rand_normal(normalDist[pno],high/6.0)); //Compile using -lm 
    if(random % high < 0) {
        return (random % high) + high;
    }
    return (random % high);
}

void printMemory() {
    printf("Memory\n");
    int i;
    for(i = 0; i < NUM_MEMORY_BLOCKS; i++) {
        printf("%d ", memory[i]);
    }

    printf("\n");
}

void printProcessor() {
    printf("Procsesor %d\n", numProcessors);
    int i;
    for( i = 0; i < numProcessors; i++) {
        printf("%d ", processor[i]);
    }

    printf("\n");
}

void printAccessTime() {
    printf("Access time %d\n", numProcessors);

    int i;
    for(i = 0; i < numProcessors; i++) {
        printf("%d ", accessCount[i]);
    }

    printf("\n");
}

// Dequeue all values from the waiting list
// and enqueue the ones that were not able to 
// find a matching 
void assignIfPriority() {
    int amount = q.numElements;
    int i;
    for(i = 0; i < amount; i++) {
        int processorIndex = Dequeue();
        // If processor is free assign
        // else enqueue
        int memoryIndex = processor[processorIndex];
        
        if(memory[memoryIndex] == false) {
            // memory hit
            memory[memoryIndex] = true;
            processor[processorIndex] = -1;
            accessCount[processorIndex]++;
        } else {
            // To ignore in future;
            Enqueue(processorIndex);
        }
    }
}

void assignNormal() {
    // For every entry, check if the memory block
    // it is requesting is taken or not. 
    // If the processor is not already on the
    // waiting list (-1), put in on the waiting list
    int i;
    for(i = 0; i < numProcessors; i++) {
        if(processor[i] == -1) 
            continue;

        int memoryIndex = processor[i];
        if(memory[memoryIndex] == false) {
            processor[i] = -1;
            memory[memoryIndex] = true;
            // memory hit
            accessCount[i]++;
        } else if(contains(i) == false) {
            Enqueue(i);
        }
    }
}

// Main simulate function
void simulate() {
    assignIfPriority();
    assignNormal();
}

// Assign processors to a random memory block
void randomAssignNonAssignedProcessors(int numMemory) {
    int i;
    for(i = 0; i < numProcessors; i++) {
        if(processor[i] == -1) {
            processor[i] = uniformDistribution(numMemory);
        }
    }
}

// To normally assign processors a memory module in susequent cycles
void normalAssignNonAssignedProcessors(int numMemory) {
    int i;
    for(i = 0; i < numProcessors; i++) {
        if(processor[i] == -1) {
            processor[i] = normalDistribution(numMemory, i);
        }
    }
}

// To get the uniformly distributed memory modules in 1st cycle
void getUniformDistribution(int numMemory) {
    int i;
    for(i = 0; i < numProcessors; i++) {
        if(normalDist[i] == -1) {
            normalDist[i] = uniformDistribution(numMemory);
        }
    }
}

// Calculate the time average if every
// processor has made at least one hit
double calculateSum(int cycle) {
    double total = 0.0;
    int i;
    for( i = 0; i < numProcessors; i++) {
        if(accessCount[i] == 0) {
            return -1;
        }

        total += (double)cycle/accessCount[i];
    }
    
    return total;
}

void resetArrays() {
    memset(memory, 0, NUM_MEMORY_BLOCKS * sizeof(memory[0]));
    memset(processor, -1, numProcessors * sizeof(processor[0]));
    memset(accessCount, 0, numProcessors * sizeof(accessCount[0]));
    memset(normalDist, -1, numProcessors * sizeof(normalDist[0]));
}

int main(int argc, char** argv) {
    if(argc != 3) {
        printf("Invalid number of arguments");
        return -1;
    }

    srand(time(NULL));
    
    numProcessors = atoi(argv[argc-2]);

    // Init arrays
    processor = (int*) malloc(numProcessors * sizeof(int));
    accessCount = (int*) malloc(numProcessors * sizeof(int));
    normalDist = (int*) malloc(numProcessors * sizeof(int));

    // TODO: Choose distribution pattern
    char distribution = argv[argc-1][0];
    
    // Init queue
    Queue(numProcessors);
    int i;
    for( i = 1; i <= NUM_MEMORY_BLOCKS; i++) {
        // Reset memory
        resetArrays();

        double prev = -1;
        getUniformDistribution(i);
        int j;
        for(j = 1; j <= pow(10, 6); j++) {
            // Choose which distribution type
            switch (distribution)
            {
                case 'u':
                    randomAssignNonAssignedProcessors(i);
                    break;
                
                case 'n':
                    // Get uniform distribution for the first cycle only
                    normalAssignNonAssignedProcessors(i);
                    break;
                
                default:
                    break;
            }
            
            simulate();
            memset(memory, 0, NUM_MEMORY_BLOCKS * sizeof(memory[0]));
            double avg = calculateSum(j);
            if(avg < 0) continue;
            avg /= numProcessors;
            if(prev == -1) {
                prev = avg;
            } else {
                if(fabs(1 - prev/avg) < 0.02) {
                    prev = avg;
                    break;
                }
                prev = avg;
            }
        }
        printf("%0.4f\n", prev);
    }

    free(processor);
    free(accessCount);
    free(normalDist);
    teardown();

    return 0;
}