#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<stdbool.h>
#include<pthread.h>
#include<sys/time.h>
#define SIZE 50
double timeLimit = 50; // seconds

int k[SIZE];
int size = SIZE;
int piLimit = 1000;
int iterations = 0;
struct timeval st, et;

int* generateRandomDistribution(int k[], int p, int piLimit){
  int i = 0;
    while(i<p){
      int x = (rand()%piLimit);
        if(x>10){
          k[i]= (x);
          i++;
        }
    }
    return k;
}

int getRandomK(int p){
    return rand()%p;
}

void printCurrentState(int k[], int size){
  for(int i = 0; i< size; i++){
    printf("%d - ", k[i]);
  }
  printf("\n");
}

bool isKBalanced(int k[], int size){
  int temp = 0;
  int min = 10000;
  int max = 0;

  for(int i = 0; i< size; i++){
    if(k[i]<min) min = k[i];
    if(k[i]>max) max = k[i];
  }
  if(abs(max-min) >1) return false;
  return true;
}

bool timeOver(){
  gettimeofday(&et, NULL);
  double elapsed_sec = (et.tv_sec - st.tv_sec) * 1000000;  //Elapsed seconds to microseconds
  double elapsed_msec = (et.tv_usec - st.tv_usec); //Elapsed microseconds
  double total_time = (elapsed_sec + elapsed_msec) / 1000000;

  if(total_time >= timeLimit)
    return true;
  return false;
}

bool loadBalance(int k[], int size, int piLimit){
  iterations++;
  printf("%d \n", iterations);
  printCurrentState(k, size);
  int currentK = getRandomK(size);
  //printf("Random: %d \n", currentK);
  int leftIndex, rightIndex;
  if(currentK == 0)
      leftIndex = size-1;
  else leftIndex = currentK-1;

  if(currentK == size-1)
      rightIndex = 0;
  else rightIndex = currentK+1;

  int average = k[leftIndex] + k[rightIndex] + k[currentK]; // 100+50+50 = 200
  int remainderAvg = average % 3; // 1

  //if(remainderAvg == 0){
    average /= 3; // 66 rem = 2
    k[currentK] = average;
    k[leftIndex] = average;
    k[rightIndex] = average;

    if(remainderAvg == 1){
      k[currentK]++;
    }
    else if(remainderAvg == 2){
      k[leftIndex]++;
      k[rightIndex]++;
    }
  //}

  return isKBalanced(k, size);

}

int main(){

srand(time(0));
generateRandomDistribution(k, size, piLimit);
printCurrentState(k, size);
bool done = false;
/* Start time */
gettimeofday(&st, NULL);

while(!done){
  bool isBalanced = loadBalance(k, size, piLimit);
  if(isBalanced) done = true;

  if(timeOver()){
    printf("Times up: Program Failed!");
    break;
  }
}
return 0;
}
