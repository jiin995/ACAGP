/** 
* Author: Gabriele Previtera
* Description: simple sum between two float vectors of 8 elements with AVX
* 
*/
#include <stdio.h>
#include <stdlib.h>

#include <time.h>

// AVX
#include <immintrin.h> 
#include <x86intrin.h> 


int main( int argc, char *argv[] ){

    float value[8] __attribute__((aligned(32))) = {0.0f,1.0f,2.0f,3.0f,4.0f,5.0f,6.0f,7.0f};

    __m256 temp = _mm256_load_ps(value); 
    __m256 increment = _mm256_set1_ps(1.0);
    
    printf("Value value is:\n %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",
        value[0], value[1], value[2], value[3],
        value[4], value[5], value[6], value[7]);

     printf("Increment value is:\n %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",
        increment[0], increment[1], increment[2], increment[3],
        increment[4], increment[5], increment[6], increment[7]);

    temp = _mm256_add_ps(temp, increment);
    
    printf("temp = value + 1 is:\n %.2f %.2f %.2f %.2f %.2f %.2f %.2f %.2f\n",
        temp[0], temp[1], temp[2], temp[3],
        temp[4], temp[5], temp[6], temp[7]);

    return 0;
}

