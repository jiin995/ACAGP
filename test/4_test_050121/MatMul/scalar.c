/**
 * Author: Gabriele Previtera
 * Basic example that works in memory:
 * It takes an image (bitmap format) and creates another in which the colors are a grayscale.
 * In this code the entire image is loaded into memory, then the color conversion formula 
 * is applied and the new pixel is always saved in memory and at the end it is written to a file. 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define N 1024

void init_float_matrix_nn(int mat[N][N], int dim);

int a[N][N];
int b[N][N];
int c[N][N];

int main( int argc, char *argv[] ){

    clock_t start, end;
    double cpu_time_used;

    int acc = 0.0f;

    //int a[N][N] = {{2, 2}, {4, 5}};
    init_float_matrix_nn(a, N);

    //int b[N][N] = {{1, 4}, {6, 3}};
    init_float_matrix_nn(b, N);

  /*  for(int i = 0; i < N; i++){
        for(int j = 0; j<N; j++){
            printf("a[%d][%d]: %d\n", i, j, a[i][j]);
        }
    }
    
    for(int i = 0; i < N; i++){
        for(int j = 0; j<N; j++){
            printf("b[%d][%d]: %d\n", i, j, b[i][j]);
        }
    }
    */

    start = clock();

    for(int i=0; i<N; i++){
        for(int j=0; j<N; j++){
            c[i][j] = 0;
            for(int k=0; k<N; k++){
                c[i][j] += a[i][k] * b[k][j];	
            }
        }
    }

    end = clock();
    
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Used time %f\n", cpu_time_used);

    /*for(int i = 0; i < N; i++){
        for(int j = 0; j<N; j++){
            printf("c[%d][%d]: %d\n", i, j, c[i][j]);
        }
    }*/

    return 0;
}

void init_float_matrix_nn(int mat[N][N], int dim){
    for(int i=0; i<dim; i++){
        for(int j=0; j<dim; j++){
            mat[i][j] = rand() % 2;
        }
    }
}