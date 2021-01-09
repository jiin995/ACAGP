/**
 * Author: Gabriele Previtera
 * Description:
 *      Multiplication between square matrices using AVX
 * Code extracted from: 
 *      https://codereview.stackexchange.com/questions/177616/avx-simd-in-matrix-multiplication
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "utils.h"

//AVX
#include <immintrin.h> 
#include <x86intrin.h> 

#define MAX_SIZE 1024

int debug_print = 0;
int size = MAX_SIZE;
int version = 1;

void version4( int* mat1, int* mat2, int* result, int size);
void version3( int* mat1, int* mat2, int* result, int size);
void version2( int* mat1, int* mat2, int* result, int size);
void version1( int* mat1, int* mat2, int* result, int size);


int a[MAX_SIZE][MAX_SIZE] __attribute__((aligned(32)));
int b[MAX_SIZE][MAX_SIZE] __attribute__((aligned(32)));
int c[MAX_SIZE][MAX_SIZE] __attribute__((aligned(32)));

int main( int argc, char *argv[] ){
    time_t t;
    clock_t start, end;
    double cpu_time_used;

    void (*alg) ( int*, int* , int* , int);

    //Init rand seed
    srand((unsigned) time(&t));

    //Parse options
    parse_opt(argc, argv );

    printf("Matrix size: %d\n\n", size);

    //Init vector with random data
    init_float_matrix_nn((int *) a, size);
    init_float_matrix_nn((int *) b, size);

    if(debug_print){
        print_matrix((int *) a, size, "a");
        print_matrix((int *) b, size, "b");
    }

    printf("Version: %d\n\n", version);

    switch (version){
        case 1:
            alg = version1;
        break;
        
        case 2:
            alg = version2;
        break;    
        
        case 3:
            alg = version3;
        break;
        
        default:
            alg = version1;
        break;
    }

    //Performing multiplication with the selected algorithm
    start = clock();

    alg((int *) a, (int *) b, (int *) c, size);

    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Used time %f s\n", cpu_time_used);

    if(debug_print){
        print_matrix((int *) c, size, "c");
    }
    
    return 0;
}

/**
*
*   Calculate the product row by column between matrices, using AVX.
*   With loop k, it calculates 8 values of the result matrix, multiplying each value of the i-th row 
*   of the first matrix with 8 values of column j of the second matrix. These calculated values are added 
*   to an accumulator and at the end of the iteration of the cycle with index k, 8 values of the result 
*   matrix are calculated
*
*/
void version1( int* mat1, int* mat2, int* result, int size){
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j += 8) {

            __m256i sumA = _mm256_setzero_si256();

            for (int k = 0; k < MAX_SIZE; k++) {
                __m256i bc_mat1 = _mm256_set1_epi32(mat1[i * size + k]);;
                __m256i vecA_mat2 = _mm256_loadu_si256((__m256i*)&mat2[k * size + j]);
                __m256i prodA = _mm256_mullo_epi32(bc_mat1, vecA_mat2);
                sumA = _mm256_add_epi32(sumA, prodA);
            }

            _mm256_storeu_si256((__m256i*)&result[i*size +j], sumA);
        }
    }
}

/**
 * 
 *  Similar to version 1, with the difference that it multiplies the value of the first matrix with 16 
 *  values of the second matrix.
 *  This reduces the likelihood of having cache misses and therefore better performance with large arrays
 * 
 */
void version2( int* mat1, int* mat2, int* result, int size){
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j += 16) {

            __m256i sumA = _mm256_setzero_si256();
            __m256i sumB = _mm256_setzero_si256();

            for (int k = 0; k < size; k++) {
                __m256i bc_mat1 = _mm256_set1_epi32(mat1[i * size + k]);;
                __m256i vecA_mat2 = _mm256_loadu_si256((__m256i*)&mat2[k* size + j]);
                __m256i vecB_mat2 = _mm256_loadu_si256((__m256i*)&mat2[k * size +j + 8]);
                __m256i prodA = _mm256_mullo_epi32(bc_mat1, vecA_mat2);
                __m256i prodB = _mm256_mullo_epi32(bc_mat1, vecB_mat2);
                sumA = _mm256_add_epi32(sumA, prodA);
                sumB = _mm256_add_epi32(sumB, prodB);
            }
            
            _mm256_storeu_si256((__m256i*)&result[i * size + j], sumA);
            _mm256_storeu_si256((__m256i*)&result[i *size + j + 8], sumB);
        }
    }
}

/**
 * 
 *  To better exploit the cache in the case of large arrays, the matrix is divided into blocks and then
 *  the version 2 algorithm is applied
 * 
 */
void version3( int* mat1, int* mat2, int* result, int size){

    size_t jb = (512u < size) ? 512u : size; 
    size_t kb = (24u  < size) ? 24u : size;

    for (size_t jj = 0; jj < size; jj += jb) 
    {
        for (size_t kk = 0; kk < size; kk += kb)
        {
            for (size_t i = 0; i < size; i += 1) {
                for (size_t j = jj; j < jj + jb; j += 16) {
                    __m256i sumA_1, sumB_1;
                    if (kk == 0) { 
                        sumA_1 = sumB_1 = _mm256_setzero_si256();
                    }
                    else {
                        sumA_1 = _mm256_load_si256((__m256i*)&result[i * size + j]);
                        sumB_1 = _mm256_load_si256((__m256i*)&result[i * size + j + 8]);
                    }
                    size_t limit = (size < kk + kb) ? size : kk + kb;
                    for (size_t k = kk; k < limit; k++) { 
                        __m256i bc_mat1_1 = _mm256_set1_epi32(mat1[i * size + k]);
                        __m256i vecA_mat2 = _mm256_loadu_si256((__m256i*)&mat2[k * size + j]);
                        __m256i vecB_mat2 = _mm256_loadu_si256((__m256i*)&mat2[k * size + j + 8]);
                        sumA_1 = _mm256_add_epi32(sumA_1, _mm256_mullo_epi32(bc_mat1_1, vecA_mat2));
                        sumB_1 = _mm256_add_epi32(sumB_1, _mm256_mullo_epi32(bc_mat1_1, vecB_mat2));
                    }
                    _mm256_storeu_si256((__m256i*)&result[i * size + j], sumA_1);
                    _mm256_storeu_si256((__m256i*)&result[i * size + j + 8], sumB_1);
                }
            }
        }
    }
}



void version4( int* mat1, int* mat2, int* result, int size){
    size_t jb = (512u < size) ? 512u : size;
    size_t kb = (24u < size) ? 24u : size;

    for (size_t jj = 0; jj < size; jj += jb)
    {
        for (size_t kk = 0; kk < size; kk += kb)
        {
            for (size_t i = 0; i < size; i += 2) {
                for (size_t j = jj; j < jj + jb; j += 16) {
                    __m256i sumA_1, sumB_1, sumA_2, sumB_2;
                    if (kk == 0) {
                        sumA_1 = sumB_1 = sumA_2 = sumB_2 = _mm256_setzero_si256();
                    }
                    else {
                        sumA_1 = _mm256_load_si256((__m256i*)&result[i * size + j]);
                        sumB_1 = _mm256_load_si256((__m256i*)&result[i * size + j + 8]);
                        sumA_2 = _mm256_load_si256((__m256i*)&result[(i + 1) * size + j]);
                        sumB_2 = _mm256_load_si256((__m256i*)&result[(i + 1) * size + j + 8]);
                    }
                    size_t limit = (size < kk + kb) ? size : kk + kb;
                    for (size_t k = kk; k < limit; k++) {
                        __m256i bc_mat1_1 = _mm256_set1_epi32(mat1[i * size + k]);
                        __m256i vecA_mat2 = _mm256_loadu_si256((__m256i*)&mat2[k * size + j]);
                        __m256i vecB_mat2 = _mm256_loadu_si256((__m256i*)&mat2[k * size + j + 8]);
                        sumA_1 = _mm256_add_epi32(sumA_1, _mm256_mullo_epi32(bc_mat1_1, vecA_mat2));
                        sumB_1 = _mm256_add_epi32(sumB_1, _mm256_mullo_epi32(bc_mat1_1, vecB_mat2));
                        __m256i bc_mat1_2 = _mm256_set1_epi32(mat1[(i + 1) * size + k]);
                        sumA_2 = _mm256_add_epi32(sumA_2, _mm256_mullo_epi32(bc_mat1_2, vecA_mat2));
                        sumB_2 = _mm256_add_epi32(sumB_2, _mm256_mullo_epi32(bc_mat1_2, vecB_mat2));
                    }
                    _mm256_storeu_si256((__m256i*)&result[i * size + j], sumA_1);
                    _mm256_storeu_si256((__m256i*)&result[i * size + j + 8], sumB_1);
                    _mm256_storeu_si256((__m256i*)&result[(i + 1) * size +j], sumA_2);
                    _mm256_storeu_si256((__m256i*)&result[(i + 1) * size + j + 8], sumB_2);
                }
            }
        }
    }
}