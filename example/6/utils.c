#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern int debug_print;
extern int size;
extern int version;

void init_float_matrix_nn(int* mat, int dim){
    for(int i=0; i<dim; i++){
        for(int j=0; j<dim; j++){
            mat[i * dim + j] = rand() % 4;
        }
    }
}

void print_matrix(int* mat, int dim, char *label){
    printf("Matrix: %s\n", label);
    for(int i = 0; i < dim; i++){
        for(int j = 0; j < dim; j++){
            printf("%d\t", mat[i * dim + j]);
        }
        printf("\n");
    }
    printf("\n");
}

void parse_opt(int argc, char *argv[] ){
    int opt = 0;

    while ((opt = getopt(argc, argv, "ds:hv:")) != -1) {
        switch(opt) {
            case 'd':
                debug_print = 1;
            break;
            case 's':
                size = atoi(optarg);
            break;
            case 'h':
                print_usage();
                exit(0);
            break;
            case 'v':
                version = atoi(optarg);
                if(version < 0 || version > 4){
                    printf("Invalid version value, it must be between 1 and 3\n\n");
                    exit(-1);
                }
            break;
        }
    }
}

void print_usage(){
    printf("Usage: ./name [-options]\n");
    printf("\t-d: print debug info, such as matrix\n");
    printf("\t-s value: limit matrix size to value instead of max value\n");
    printf("\t-v value: execute version \"value\" of algorithm\n");

    printf("\n");
}