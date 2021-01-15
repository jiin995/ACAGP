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

    __m256i vindex = _mm256_set_epi32(14, 12, 10, 8, 6, 4, 2, 0);

    for (int i = 0; i < (size/8); i++){
        
        int offset = 8*i;

        sum = _mm256_i32gather_ps (value+offset, vindex, 1);
        sub = _mm256_i32gather_ps (value+offset+1, vindex, 1);

        sum = _mm256_add_ps(sum, increment);
        sub = _mm256_sub_ps(sub, increment);

        /*for(int j = 0; j<8; j++){
            int shift = j*2;
            temp[offset+shift] = sum[j];
            temp[offset+shift+1] = sub[j];
        }*/

        temp[offset] = sum[0];
        temp[offset+1] = sub[0];
        temp[offset+2] = sum[1];
        temp[offset+3] = sub[1];
        temp[offset+4] = sum[2];
        temp[offset+5] = sub[2];
        temp[offset+6] = sum[3];
        temp[offset+7] = sub[3];
        temp[offset+8] = sum[4];
        temp[offset+9] = sub[4];
        temp[offset+10] = sum[5];
        temp[offset+11] = sub[5];
        temp[offset+12] = sum[6];
        temp[offset+13] = sub[6];        
        temp[offset+14] = sum[7];
        temp[offset+15] = sub[7];
    }
    
    end = clock();
    
    if(debug_print){
        print_vect(temp, size, "result");
    }

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Used time %.10f s\n", cpu_time_used);
    
    return 0;
}