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

#include "utils.h"

#define MAX_SIZE 1024

int debug_print = 0;
int size = MAX_SIZE;
int version = 0;


int a[MAX_SIZE][MAX_SIZE];
int b[MAX_SIZE][MAX_SIZE];
int c[MAX_SIZE][MAX_SIZE];

int main( int argc, char *argv[] ){
    time_t t;
    clock_t start, end;
    double cpu_time_used;

    int acc = 0.0f;

    //init rand seed
    srand((unsigned) time(&t));

    //Parse options
    parse_opt(argc, argv );

    printf("Matrix size: %d\n\n", size);

    //Init vector with random data
    init_float_matrix_nn((int *) a, size);
    init_float_matrix_nn((int *) b, size);


    if(debug_print){
        print_matrix((int*) a, size, "A");
        print_matrix((int*) b, size, "B");

    }

    start = clock();

    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            c[i][j] = 0;
            for(int k=0; k<size; k++){
                c[i][j] += a[i][k] * b[k][j];	
            }
        }
    }

    end = clock();
    
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Used time %f s\n", cpu_time_used);

    if(debug_print){
        print_matrix((int *) c, size, "C");
    }

    return 0;
}