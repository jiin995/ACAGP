/** 
* Author: Gabriele Previtera
* Description: simple sum between two float vectors of 8 elements
* 
*/
#include <stdio.h>
#include <stdlib.h>

//Time 
#include <time.h>

#define V_SIZE 8

int main( int argc, char *argv[] ){

    float value[V_SIZE] = {0.0f,1.0f,2.0f,3.0f,4.0f,5.0f,6.0f,7.0f};
    float increment[V_SIZE] = {1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f};
    float temp [V_SIZE];

    clock_t start, end;
    double cpu_time_used;

    printf("Value value is:\n %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",
        value[0], value[1], value[2], value[3],
        value[4], value[5], value[6], value[7]);

    printf("Increment value is:\n %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",
        increment[0], increment[1], increment[2], increment[3],
        increment[4], increment[5], increment[6], increment[7]);

    start = clock();

    for (int i = 0; i < V_SIZE; i++){
        temp[i] = value[i] + increment[i];
    }
    
    end = clock();

    printf("temp = value + 1 is:\n %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",
        temp[0], temp[1], temp[2], temp[3],
        temp[4], temp[5], temp[6], temp[7]);

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Used time %.10f\n", cpu_time_used);

    return 0;
}

