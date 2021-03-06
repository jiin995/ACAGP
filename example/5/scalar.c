/** 
* Author: Gabriele Previtera
* Description: 
* This code calcule new vector of float multiplying the elements by two and adding 1
* 
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

    start = clock();

    for (int i = 0; i < size; i++){
        
        temp[i] = (value[i]*2)+1;
    }
    
    end = clock();
    
    if(debug_print){
        print_vect(temp, size, "result");
    }

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Used time %.10f s\n", cpu_time_used);
    
    return 0;
}