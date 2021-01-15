#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern int debug_print;
extern int size;

void init_vector(float vet[], int dim){
    for(int i=0; i<dim; i++){
        vet[i] = (rand() / 30) % 30;
    }
}

void print_vect (float vet[], int dim, char *label){
    printf("Vector: %s\n", label);

    for(int i = 0; i < dim; i++){
        printf("%f\t", vet[i]);
     }
    printf("\n\n");
}

void parse_opt(int argc, char *argv[] ){
    int opt = 0;

    while ((opt = getopt(argc, argv, "ds:h")) != -1) {
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
        }
    }
}

void print_usage(){
    printf("Usage: ./name [-options]\n");
    printf("\t-d: print debug info, such as matrix\n");
    printf("\t-s value: limit matrix size to value instead of max value\n");
    printf("\n");
}