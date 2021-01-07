/** 
* Author: Gabriele Previtera
* Description: simple sum between two float vectors of 32 elements with AVX repeated 1000 times
* It is just an example to make performance evaluations.
*/
#include <stdio.h>
#include <stdlib.h>

#include <time.h>

// AVX
#include <immintrin.h> 
#include <x86intrin.h> 

#define N 1000
#define V_SIZE 32

int main( int argc, char *argv[] ){

    float value[V_SIZE] __attribute__((aligned(32))) = 
        {   0.0f,1.0f,2.0f,3.0f,4.0f,5.0f,6.0f,7.0f,
            8.0f,9.0f,10.0f,11.0f,12.0f,13.0f,14.0f,15.0f,
            16.0f,17.0f,18.0f,19.0f,20.0f,21.0f,22.0f,23.0f,24.0f,
            25.0f,26.0f,27.0f,28.0f,29.0f,30.0f,31.0f,
        };
    
    float temp[V_SIZE] __attribute__((aligned(32)));

    __m256 sum = _mm256_load_ps(value); 
    __m256 increment = _mm256_set1_ps(1.0);
    
    clock_t start, end;
    double cpu_time_used;

    start = clock();

    for(int j=0; j < N; j++){
        for (int i = 0; i < 4; i++){
            sum = _mm256_load_ps(value+8*i); 
            sum = _mm256_add_ps(sum, increment);
            _mm256_store_ps(temp+8*i, sum);
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

