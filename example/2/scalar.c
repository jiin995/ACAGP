/** 
* Author: Gabriele Previtera
* Description: simple sum between two float vectors of 32 elements repeated 1000 times.
* It is just an example to make performance evaluations.
*/
#include <stdio.h>
#include <stdlib.h>

//Time 
#include <time.h>

#define V_SIZE 32
#define N 1000


int main( int argc, char *argv[] ){

    float value[V_SIZE] = 
    {   0.0f,1.0f,2.0f,3.0f,4.0f,5.0f,6.0f,7.0f,
        8.0f,9.0f,10.0f,11.0f,12.0f,13.0f,14.0f,15.0f,
        16.0f,17.0f,18.0f,19.0f,20.0f,21.0f,22.0f,23.0f,24.0f,
        25.0f,26.0f,27.0f,28.0f,29.0f,30.0f,31.0f,
    };
    float increment[V_SIZE] = 
    {   1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,
        1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,
        1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,
        1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f
    };
    
    float temp [V_SIZE];

    clock_t start, end;
    double cpu_time_used;

    start = clock();

    for(int j = 0; j < N; j++){
        for (int i = 0; i < V_SIZE; i++){
            temp[i] = value[i] + increment[i];
        }
    }


    end = clock();

    for (int i = 0; i < V_SIZE/8; i++){
        printf("temp = value + 1 is:\n %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",
            temp[8*i], temp[1+8*i], temp[2+8*i], temp[3+8*i],
            temp[4+8*i], temp[5+8*i], temp[6+8*i], temp[7+8*i]);
    }

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Used time %.10f\n", cpu_time_used);

    return 0;
}

