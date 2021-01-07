/** 
* Author: Gabriele Previtera
* Description: 
* Simple example of mask load and store.
* This code adds 1 to even-numbered elements in the vector and subtracts 1 from odd-numbered elements 
* 
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
    
    __m256 sum;
    __m256 sub;
    
    __m256 increment = _mm256_set1_ps(1.0);

    __m256i sum_mask = _mm256_setr_epi32(-1, 0, -1, 0, -1, 0, -1, 0);
    __m256i sub_mask = _mm256_setr_epi32(0, -1, 0, -1, 0, -1, 0, -1);
    
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

    if(debug_print){
        int32_t *ss_mask = (int32_t *) &sum_mask;

        printf("sum_mask = \n %d %d %d %d %d %d %d %d\n",
                    ss_mask[0], ss_mask[1], ss_mask[2], ss_mask[3],
                    ss_mask[4], ss_mask[5], ss_mask[6], ss_mask[7]);

        ss_mask = (int32_t *) &sub_mask;

        printf("sub_mask = \n %d %d %d %d %d %d %d %d\n",
                        ss_mask[0], ss_mask[1], ss_mask[2], ss_mask[3],
                        ss_mask[4], ss_mask[5], ss_mask[6], ss_mask[7]);
        
        printf("\n");
    }

    start = clock();

    for (int i = 0; i < (size/8)+1; i++){
        sum = _mm256_maskload_ps(value+8*i, sum_mask); 
        sub = _mm256_maskload_ps(value+8*i, sub_mask);

        sum = _mm256_add_ps(sum, increment);
        sub = _mm256_sub_ps(sub, increment);

        _mm256_maskstore_ps(temp+8*i, sum_mask, sum);
        _mm256_maskstore_ps(temp+8*i, sub_mask, sub);
    }
    
    end = clock();
    
    if(debug_print){
        print_vect(temp, size, "result");
    }

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Used time %.10f\n", cpu_time_used);
    
    return 0;
}