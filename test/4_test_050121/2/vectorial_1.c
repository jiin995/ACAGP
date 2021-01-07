/**
 * Author: Gabriele Previtera
 * Basic example that works in memory:
 * It takes an image (bitmap format) and creates another in which the colors are a grayscale.
 * In this code the entire image is loaded into memory, then the color conversion formula 
 * is applied and the new pixel is always saved in memory and at the end it is written to a file. 
 * 
 * This version use Intel AVX and use 
 *     
 * gray = 0.3 * R + 0.59 * G + 0.11 * B
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//AVX
#include <immintrin.h> 
#include <x86intrin.h> 

__m256 load_r(unsigned char * image_data_in);
__m256 load_g(unsigned char * image_data_in);
__m256 load_b(unsigned char * image_data_in);

__m256i r1_mask, r2_mask, r3_mask;
__m256i g1_mask, g2_mask, g3_mask;
__m256i b1_mask, b2_mask, b3_mask;


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
    printf("n_pixels/8: %d\n", n_pixels/32);

    __m256 r_percentage = _mm256_set1_ps(0.3);
    __m256 g_percentage = _mm256_set1_ps(0.58);
    __m256 b_percentage = _mm256_set1_ps(0.11);

    r1_mask = _mm256_setr_epi32(-1, 0, 0, -1, 0, 0, -1, 0);
    r2_mask = _mm256_setr_epi32(0, -1, 0, 0, -1, 0, 0, -1);
    r3_mask = _mm256_setr_epi32(0, 0, -1, 0, 0, -1, 0, 0);

    g1_mask = _mm256_setr_epi32(0, -1, 0, 0, -1, 0, 0, -1);
    g2_mask = _mm256_setr_epi32(0, 0, -1, 0, 0, -1, 0, 0);
    g3_mask = _mm256_setr_epi32(-1, 0, 0, -1, 0, 0, -1, 0);    

    b1_mask = _mm256_setr_epi32(0, 0, -1, 0, 0, -1, 0, 0);
    b2_mask = _mm256_setr_epi32(-1, 0, 0, -1, 0, 0, -1, 0);
    b3_mask = _mm256_setr_epi32(0, -1, 0, 0, -1, 0, 0, -1);

    start = clock();

    for(int pixel_index = 0; pixel_index < /*n_pixels/8*/ 1; pixel_index++){

        /*float r[8] __attribute__((aligned(32))) = { image_data_in[pixel_index*24], image_data_in[pixel_index*24+3], image_data_in[pixel_index*24+6],
                                image_data_in[pixel_index*24+9], image_data_in[pixel_index*24+12],image_data_in[pixel_index*24+15],
                                image_data_in[pixel_index*24+18], image_data_in[pixel_index*24+21]
        };

        float g[8] __attribute__((aligned(32))) = { image_data_in[pixel_index*24+1], image_data_in[pixel_index*24+4], image_data_in[pixel_index*24+7],
                                image_data_in[pixel_index*24+10], image_data_in[pixel_index*24+13],image_data_in[pixel_index*24+16],
                                image_data_in[pixel_index*24+19], image_data_in[pixel_index*24+22]
        };

        float b[8] __attribute__((aligned(32))) = { image_data_in[pixel_index*24+2], image_data_in[pixel_index*24+5], image_data_in[pixel_index*24+8],
                                image_data_in[pixel_index*24+11], image_data_in[pixel_index*24+14],image_data_in[pixel_index*24+17],
                                image_data_in[pixel_index*24+20], image_data_in[pixel_index*24+23]
        };
        */
        
        __m256 gray_avx =  _mm256_setzero_ps();

        /*__m256 r_avx = _mm256_load_ps(r);
        __m256 g_avx = _mm256_load_ps(g);
        __m256 b_avx = _mm256_load_ps(b);
        */

        __m256 r_avx = load_r(image_data_in+24*pixel_index);
        __m256 g_avx = load_g(image_data_in+24*pixel_index);
        __m256 b_avx = load_b(image_data_in+24*pixel_index);

       float *red_avx_c = (float *) &r_avx;      
       float *green_avx_c = (float*) &g_avx;
       float *blue_avx_c = (float *) &b_avx;
       
       printf("\n");
       
       for(int i=0; i<8; i++){
            
            /*
            unsigned char gray = image_data_in[pixel_index*96+i*3]+image_data_in[pixel_index*96+i*3+1]+image_data_in[pixel_index*96+i*3+2];
            
            */
            printf("red[%d]:%d\n", i, image_data_in[pixel_index*24+i*3]);
            printf("green[%d]:%d\n", i, image_data_in[pixel_index*24+i*3+1]);
            printf("blue[%d]:%d\n", i, image_data_in[pixel_index*24+i*3+2]);
            
            /*
            printf("gray_avx: %d\n", gray_avx_c[i]);
            */
            printf("red_avx[%d]:%.2f\n", i, r_avx[i]);
            printf("green_avx[%d]:%.2f\n", i, g_avx[i]);
            printf("blue_avx[%d]:%.2f\n", i, b_avx[i]);

            printf("\n");
        }


        r_avx = _mm256_mul_ps(r_avx, r_percentage);
        g_avx = _mm256_mul_ps(g_avx, g_percentage);
        b_avx = _mm256_mul_ps(b_avx, b_percentage);

        gray_avx = _mm256_add_ps(r_avx, g_avx);
        gray_avx = _mm256_add_ps(gray_avx, b_avx);


        for(int i=0; i<8; i++){
            memset(image_data_out+(pixel_index*24)+i*3,  gray_avx[i], 3);
        }
        
    }

    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Used time %f\n", cpu_time_used);

    fwrite(image_data_out, image_size_bytes, 1, fOut);

    fclose(fOut);
    fclose(fIn);

    free(image_data_in);
    free(image_data_out);
    
    return 0;
}

__m256 load_r(unsigned char * image_data_in){

    __m256 r1 = _mm256_maskload_ps(image_data_in, r1_mask);
    __m256 r2 = _mm256_maskload_ps(image_data_in+8, r2_mask);
    __m256 r3 = _mm256_maskload_ps(image_data_in+16, r3_mask);

    r1 = _mm256_blend_ps(r1, r2, 0b01001001);
    __m256 r = _mm256_blend_ps(r1, r3, 0b00100100);

    return r1;
}

__m256 load_g(unsigned char * image_data_in){

    __m256 g1 = _mm256_maskload_ps(image_data_in, g1_mask);
    __m256 g2 = _mm256_maskload_ps(image_data_in+8, g2_mask);
    __m256 g3 = _mm256_maskload_ps(image_data_in+16, g3_mask);

    g1 = _mm256_blend_ps(g1, g2, 0b00100100);
    __m256 g =_mm256_blend_ps(g1, g3, 0b10010010);

    return g;
}


__m256 load_b(unsigned char * image_data_in){

    __m256 b1 = _mm256_maskload_ps(image_data_in, b1_mask);
    __m256 b2 = _mm256_maskload_ps(image_data_in+8, b2_mask);
    __m256 b3 = _mm256_maskload_ps(image_data_in+16, b3_mask);

    b1 = _mm256_blend_ps(b1, b2, 0b10010010);
    __m256 b =  _mm256_blend_ps(b1, b3, 0b01001001);

    return b;
}