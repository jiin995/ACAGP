/**
 * Author: Gabriele Previtera
 * Basic example that works in memory:
 * It takes an image (bitmap format) and creates another in which the colors are a grayscale.
 * In this code the entire image is loaded into memory, then the color conversion formula 
 * is applied and the new pixel is always saved in memory and at the end it is written to a file. 
 * 
 * This version use Intel AVX and use 
 *     
 * gray = R + G + B
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//AVX
#include <immintrin.h> 
#include <x86intrin.h> 


int main( int argc, char *argv[] ){

    FILE *fIn = fopen("../sample.bmp", "rb");
    FILE *fOut = fopen("../sample_gray.bmp", "wb");

    clock_t start, end;
    double cpu_time_used;
    
    if (!fIn || !fOut)
    {
        printf("File error.\n");
        return 0;
    }

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, fIn);
    fwrite(header, sizeof(unsigned char), 54, fOut);

    int width = *(int*)&header[18];
    int height = abs(*(int*)&header[22]);
    int stride = (width * 3 + 3) & ~3;
    int padding = stride - width * 3;
    int image_size_bytes = *(int*)&header[34];

    printf("width: %d (%d)\n", width, width * 3);
    printf("height: %d\n", height);
    printf("stride: %d\n", stride);
    printf("padding: %d\n", padding);
    printf("image_size_bytes: %d\n", image_size_bytes);

    unsigned char *image_data_in __attribute__((aligned(32))) = malloc(sizeof(char)*image_size_bytes);
    unsigned char *image_data_out __attribute__((aligned(32))) = malloc(sizeof(char)*image_size_bytes);

    unsigned char pixel[3] __attribute__((aligned(32)));
    
    size_t num_read = fread(image_data_in, image_size_bytes, 1, fIn);

    if(num_read < 0){
        perror("Read Error");
        free(image_data_in);
        free(image_data_out);
        exit(-1);
    }

    unsigned int readed_bytes = 0;

    int n_pixels = height*width;
    printf("n_pixels: %d\n", n_pixels);
    printf("n_pixels/32: %d\n", n_pixels/32);
    
    start = clock();

    for(int pixel_index = 0; pixel_index < n_pixels/32; pixel_index++){

        unsigned char r[32] __attribute__((aligned(32))) = { image_data_in[pixel_index*96], image_data_in[pixel_index*96+3], image_data_in[pixel_index*96+6],
                                image_data_in[pixel_index*96+9], image_data_in[pixel_index*96+12],image_data_in[pixel_index*96+15],
                                image_data_in[pixel_index*96+18], image_data_in[pixel_index*96+21], image_data_in[pixel_index*96+24],
                                image_data_in[pixel_index*96+27], image_data_in[pixel_index*96+30], image_data_in[pixel_index*96+33],
                                image_data_in[pixel_index*96+36], image_data_in[pixel_index*96+39], image_data_in[pixel_index*96+42],
                                image_data_in[pixel_index*96+45], image_data_in[pixel_index*96+48], image_data_in[pixel_index*96+51],
                                image_data_in[pixel_index*96+54], image_data_in[pixel_index*96+57], image_data_in[pixel_index*96+60],
                                image_data_in[pixel_index*96+63], image_data_in[pixel_index*96+66], image_data_in[pixel_index*96+69],
                                image_data_in[pixel_index*96+72], image_data_in[pixel_index*96+75], image_data_in[pixel_index*96+78],
                                image_data_in[pixel_index*96+81], image_data_in[pixel_index*96+84], image_data_in[pixel_index*96+87],
                                image_data_in[pixel_index*96+90], image_data_in[pixel_index*96+93]
        };

        unsigned char g[32] __attribute__((aligned(32))) = { image_data_in[pixel_index*96+1], image_data_in[pixel_index*96+4], image_data_in[pixel_index*96+7],
                                image_data_in[pixel_index*96+10], image_data_in[pixel_index*96+13],image_data_in[pixel_index*96+16],
                                image_data_in[pixel_index*96+19], image_data_in[pixel_index*96+22], image_data_in[pixel_index*96+25],
                                image_data_in[pixel_index*96+28], image_data_in[pixel_index*96+31], image_data_in[pixel_index*96+34],
                                image_data_in[pixel_index*96+37], image_data_in[pixel_index*96+40], image_data_in[pixel_index*96+43],
                                image_data_in[pixel_index*96+46], image_data_in[pixel_index*96+49], image_data_in[pixel_index*96+52],
                                image_data_in[pixel_index*96+55], image_data_in[pixel_index*96+58], image_data_in[pixel_index*96+61],
                                image_data_in[pixel_index*96+64], image_data_in[pixel_index*96+67], image_data_in[pixel_index*96+70],
                                image_data_in[pixel_index*96+73], image_data_in[pixel_index*96+76], image_data_in[pixel_index*96+79],
                                image_data_in[pixel_index*96+82], image_data_in[pixel_index*96+85], image_data_in[pixel_index*96+88],
                                image_data_in[pixel_index*96+91], image_data_in[pixel_index*96+94]
        };

        unsigned char b[32] __attribute__((aligned(32))) = { image_data_in[pixel_index*96+2], image_data_in[pixel_index*96+5], image_data_in[pixel_index*96+8],
                                image_data_in[pixel_index*96+11], image_data_in[pixel_index*96+14],image_data_in[pixel_index*96+17],
                                image_data_in[pixel_index*96+20], image_data_in[pixel_index*96+23], image_data_in[pixel_index*96+26],
                                image_data_in[pixel_index*96+29], image_data_in[pixel_index*96+32], image_data_in[pixel_index*96+35],
                                image_data_in[pixel_index*96+38], image_data_in[pixel_index*96+41], image_data_in[pixel_index*96+44],
                                image_data_in[pixel_index*96+47], image_data_in[pixel_index*96+50], image_data_in[pixel_index*96+53],
                                image_data_in[pixel_index*96+56], image_data_in[pixel_index*96+59], image_data_in[pixel_index*96+62],
                                image_data_in[pixel_index*96+65], image_data_in[pixel_index*96+68], image_data_in[pixel_index*96+71],
                                image_data_in[pixel_index*96+74], image_data_in[pixel_index*96+77], image_data_in[pixel_index*96+80],
                                image_data_in[pixel_index*96+83], image_data_in[pixel_index*96+86], image_data_in[pixel_index*96+89],
                                image_data_in[pixel_index*96+92], image_data_in[pixel_index*96+95]
        };

        __m256i_u r_avx = _mm256_loadu_si256((__m256i_u*) &r);
        __m256i_u g_avx = _mm256_loadu_si256((__m256i_u*) &g);
        __m256i_u b_avx = _mm256_loadu_si256((__m256i_u*) &b);

        __m256i_u gray_avx = _mm256_add_epi8(r_avx, g_avx);
        gray_avx = _mm256_add_epi8(gray_avx, b_avx);

        unsigned char *gray_avx_c = (unsigned char *) &gray_avx;

        for(int i=0; i<32; i++){
            memset(image_data_out+(pixel_index*96)+i*3,  gray_avx_c[i], 3);
        }          
    }

    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Used time %f s\n", cpu_time_used);

    fwrite(image_data_out, image_size_bytes, 1, fOut);

    fclose(fOut);
    fclose(fIn);

    free(image_data_in);
    free(image_data_out);
    
    return 0;
}
