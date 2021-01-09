/** 
* Author: Gabriele Previtera
* Description: 
* This code adds 1 to even-numbered elements in the vector and subtracts 1 from odd-numbered elements 
* 
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//Time 
#include <time.h>

#include "utils.h"

#define V_MAX_SIZE 65536

int debug_print = 0;
int size = V_MAX_SIZE;

float value[V_MAX_SIZE] __attribute__((aligned(32)));
float temp[V_MAX_SIZE] __attribute__((aligned(32)));

int main( int argc, char *argv[] ){
    time_t t;
    clock_t start, end;
    double cpu_time_used;

    //init rand seed
    srand((unsigned) time(&t));

    //Parse options
    parse_opt(argc, argv );

    printf("Vector size: %d\n\n", size);

    //Init vector with random data
    init_vector(value, size);

    if(debug_print){
        print_vect(value, size, "value");
    }

    //Start computation
    start = clock();

    for (int i = 0; i < V_MAX_SIZE; i++){
        if(i%2 == 0){
            temp[i] = value[i] + 1;
        }
        else{
            temp[i] = value[i] - 1;
        }
            
    }

    end = clock();

    if(debug_print){
        print_vect(temp, size, "result");
    }

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Used time %.10f s\n", cpu_time_used);

    return 0;
}