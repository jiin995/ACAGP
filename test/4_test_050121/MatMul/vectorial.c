/**
 * Author: Gabriele Previtera
 * https://codereview.stackexchange.com/questions/177616/avx-simd-in-matrix-multiplication
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//AVX
#include <immintrin.h> 
#include <x86intrin.h> 

#define N 1024

void init_float_matrix_nn(int mat[N][N], int dim);

void version4(int mat1[N][N], int mat2[N][N], int result[N][N]);
void version3(int mat1[N][N], int mat2[N][N], int result[N][N]);
void version2 (int mat1[N][N], int mat2[N][N], int result[N][N]);
void version1 (int mat1[N][N], int mat2[N][N], int result[N][N]);

void print_matrix(int mat[N][N], char *label);

int a[N][N] __attribute__((aligned(32)));
int b[N][N] __attribute__((aligned(32)));
int c[N][N] __attribute__((aligned(32)));

int main( int argc, char *argv[] ){
    time_t t;

    srand((unsigned) time(&t));

    clock_t start, end;
    double cpu_time_used;

    init_float_matrix_nn(a, N);

    init_float_matrix_nn(b, N);
       
    //print_matrix(a, "a");
    //print_matrix(b, "b");

    start = clock();

    version3(a, b, c);

    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Used time %f\n", cpu_time_used);

    //print_matrix(c, "c");
    
    return 0;
}

/**
* Prende un elemento per volta della prima matrice e lo moltiplica per tutta la riga con cui 
* dovrebbe essere moltiplicata della seconda matrice.
* Ad ogni iterazione fa delle somme parziali con la riga calcolata precentemente e quando si è scorsa tutta la prima riga della prima 
* matrice allora nelle somme parziali ci sarà la riga della matrice risultato
*/
void version1 (int mat1[N][N], int mat2[N][N], int result[N][N]){
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j += 8) {
            __m256i sumA = _mm256_setzero_si256();
            __m256i sumB = _mm256_setzero_si256();
            for (int k = 0; k < N; k++) {
                __m256i bc_mat1 = _mm256_set1_epi32(mat1[i][k]);;
                __m256i vecA_mat2 = _mm256_loadu_si256((__m256i*)&mat2[k][j]);
                __m256i prodA = _mm256_mullo_epi32(bc_mat1, vecA_mat2);
                sumA = _mm256_add_epi32(sumA, prodA);
            }
            _mm256_storeu_si256((__m256i*)&result[i][j], sumA);
        }
    }
}

/**
 * Uguale alla versione  uno solo che se la matrice è più grande, cerca di caricare l'elemento
 * della prima matrice meno volte.
 * Divide la riga della seconda matrice in due vettori
 */
void version2 (int mat1[N][N], int mat2[N][N], int result[N][N]){
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j += 16) {
            __m256i sumA = _mm256_setzero_si256();
            __m256i sumB = _mm256_setzero_si256();
            for (int k = 0; k < N; k++) {
                __m256i bc_mat1 = _mm256_set1_epi32(mat1[i][k]);;
                __m256i vecA_mat2 = _mm256_loadu_si256((__m256i*)&mat2[k][j]);
                __m256i vecB_mat2 = _mm256_loadu_si256((__m256i*)&mat2[k][j + 8]);
                __m256i prodA = _mm256_mullo_epi32(bc_mat1, vecA_mat2);
                __m256i prodB = _mm256_mullo_epi32(bc_mat1, vecB_mat2);
                sumA = _mm256_add_epi32(sumA, prodA);
                sumB = _mm256_add_epi32(sumB, prodB);
            }
            _mm256_storeu_si256((__m256i*)&result[i][j], sumA);
            _mm256_storeu_si256((__m256i*)&result[i][j + 8], sumB);
        }
    }
}

void version3(int mat1[N][N], int mat2[N][N], int result[N][N]){

    size_t jb = (512u < N) ? 512u : N; //numero di colonne della blocco
    size_t kb = (24u  < N) ? 24u : N;

    for (size_t jj = 0; jj < N; jj += jb) //Divisione della matrice, prende 512 colonne per volta
    {
        for (size_t kk = 0; kk < N; kk += kb)
        {
            for (size_t i = 0; i < N; i += 1) {
                for (size_t j = jj; j < jj + jb; j += 16) {
                    __m256i sumA_1, sumB_1;
                    if (kk == 0) { // se è la prima iterazione del cliclo 2 inizializzo i registri
                        sumA_1 = sumB_1 = _mm256_setzero_si256();
                    }
                    else {//altrimenti carico le somme parziali
                        sumA_1 = _mm256_load_si256((__m256i*)&result[i][j]);
                        sumB_1 = _mm256_load_si256((__m256i*)&result[i][j + 8]);
                    }
                    size_t limit = (N < kk + kb) ? N : kk + kb;
                    for (size_t k = kk; k < limit; k++) { 
                        __m256i bc_mat1_1 = _mm256_set1_epi32(mat1[i][k]);
                        __m256i vecA_mat2 = _mm256_loadu_si256((__m256i*)&mat2[k][j]);
                        __m256i vecB_mat2 = _mm256_loadu_si256((__m256i*)&mat2[k][j + 8]);
                        sumA_1 = _mm256_add_epi32(sumA_1, _mm256_mullo_epi32(bc_mat1_1, vecA_mat2));
                        sumB_1 = _mm256_add_epi32(sumB_1, _mm256_mullo_epi32(bc_mat1_1, vecB_mat2));
                    }
                    _mm256_storeu_si256((__m256i*)&result[i][j], sumA_1);
                    _mm256_storeu_si256((__m256i*)&result[i][j + 8], sumB_1);
                }
            }
        }
    }
}



void version4(int mat1[N][N], int mat2[N][N], int result[N][N]){
    size_t jb = (512u < N) ? 512u : N;
    size_t kb = (24u < N) ? 24u : N;

    for (size_t jj = 0; jj < N; jj += jb)
    {
        for (size_t kk = 0; kk < N; kk += kb)
        {
            for (size_t i = 0; i < N; i += 2) {
                for (size_t j = jj; j < jj + jb; j += 16) {
                    __m256i sumA_1, sumB_1, sumA_2, sumB_2;
                    if (kk == 0) {
                        sumA_1 = sumB_1 = sumA_2 = sumB_2 = _mm256_setzero_si256();
                    }
                    else {
                        sumA_1 = _mm256_load_si256((__m256i*)&result[i][j]);
                        sumB_1 = _mm256_load_si256((__m256i*)&result[i][j + 8]);
                        sumA_2 = _mm256_load_si256((__m256i*)&result[i + 1][j]);
                        sumB_2 = _mm256_load_si256((__m256i*)&result[i + 1][j + 8]);
                    }
                    size_t limit = (N < kk + kb) ? N : kk + kb;
                    for (size_t k = kk; k < limit; k++) {
                        __m256i bc_mat1_1 = _mm256_set1_epi32(mat1[i][k]);
                        __m256i vecA_mat2 = _mm256_loadu_si256((__m256i*)&mat2[k][j]);
                        __m256i vecB_mat2 = _mm256_loadu_si256((__m256i*)&mat2[k][j + 8]);
                        sumA_1 = _mm256_add_epi32(sumA_1, _mm256_mullo_epi32(bc_mat1_1, vecA_mat2));
                        sumB_1 = _mm256_add_epi32(sumB_1, _mm256_mullo_epi32(bc_mat1_1, vecB_mat2));
                        __m256i bc_mat1_2 = _mm256_set1_epi32(mat1[i + 1][k]);
                        sumA_2 = _mm256_add_epi32(sumA_2, _mm256_mullo_epi32(bc_mat1_2, vecA_mat2));
                        sumB_2 = _mm256_add_epi32(sumB_2, _mm256_mullo_epi32(bc_mat1_2, vecB_mat2));
                    }
                    _mm256_storeu_si256((__m256i*)&result[i][j], sumA_1);
                    _mm256_storeu_si256((__m256i*)&result[i][j + 8], sumB_1);
                    _mm256_storeu_si256((__m256i*)&result[i + 1][j], sumA_2);
                    _mm256_storeu_si256((__m256i*)&result[i + 1][j + 8], sumB_2);
                }
            }
        }
    }

}

void init_float_matrix_nn(int mat[N][N], int dim){
    for(int i=0; i<dim; i++){
        for(int j=0; j<dim; j++){
            mat[i][j] = rand() % 4;
        }
    }
}

void print_matrix(int mat[N][N], char *label){
    printf("Matrix: %s\n", label);
    for(int i = 0; i < N; i++){
        for(int j = 0; j<N; j++){
            printf("%d\t", mat[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}