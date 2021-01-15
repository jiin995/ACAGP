/** 
* Author: Gabriele Previtera
* Description: 
* Simple example of fusion multiply add.
* This code calcule new vector of float multiplying the elements by two and adding 1
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <time.h>

#include "utils.h"


// AVX
#include <immintrin.h> 
#include <x86intrin.h> 

#define V_MAX_SIZE 65536

int debug_print = 0;
int size = V_MAX_SIZE;

float value[V_MAX_SIZE] __attribute__((aligned(32)));
float temp[V_MAX_SIZE] __attribute__((aligned(32)));

int main( int argc, char *argv[] ){
    time_t t;
    clock_t start, end;
    double cpu_time_used;
    
    __m256 temp_avx;
    
    __m256 increment = _mm256_set1_ps(1.0);
    __m256 multiplicand = _mm256_set1_ps(2.0);

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

    for (int i = 0; i < (size/8); i++){
        
        int offset = 8*i;

        temp_avx = _mm256_load_ps(value+offset);

        temp_avx = _mm256_fmadd_ps(temp_avx, multiplicand, increment);

        _mm256_store_ps(temp+offset, temp_avx);
    }
    
    end = clock();
    
    if(debug_print){
        print_vect(temp, size, "result");
    }

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Used time %.10f s\n", cpu_time_used);
    
    return 0;
}