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
    
    size_t num_read = fread(image_data_in, image_size_bytes, 1, fIn);

    if(num_read < 0){
        perror("Read Error");
        free(image_data_in);
        free(image_data_out);
        exit(-1);
    }

    int n_pixels = height*width;
    printf("n_pixels: %d\n", n_pixels);
    printf("n_pixels/8: %d\n", n_pixels/8);

    __m256 r_percentage = _mm256_set1_ps(0.3);
    __m256 g_percentage = _mm256_set1_ps(0.58);
    __m256 b_percentage = _mm256_set1_ps(0.11);
    __m256 r_avx, g_avx, b_avx, gray_avx;
    __m256i vindex = _mm256_set_epi32(21, 18, 15, 12, 9, 6, 3, 0);

    int pixel_offset ;
    
    start = clock();
    
    for(int pixel_index = 0; pixel_index < n_pixels/8; pixel_index++){

        pixel_offset = pixel_index*24;
        
        gray_avx =  _mm256_setzero_ps();

        r_avx = _mm256_i32gather_ps(image_data_in+pixel_offset, vindex, 1);
        g_avx = _mm256_i32gather_ps(image_data_in+pixel_offset+1, vindex ,1);
        b_avx = _mm256_i32gather_ps(image_data_in+pixel_offset+2, vindex ,1);

        gray_avx = _mm256_fmadd_ps(r_avx, r_percentage, gray_avx);
        gray_avx = _mm256_fmadd_ps(g_avx, g_percentage, gray_avx);
        gray_avx = _mm256_fmadd_ps(b_avx, b_percentage, gray_avx);

        image_data_out[pixel_offset] = image_data_out[pixel_offset+1] = image_data_out[pixel_offset+2] = gray_avx[0];
        image_data_out[pixel_offset+3] = image_data_out[pixel_offset+4] = image_data_out[pixel_offset+5] = gray_avx[1];
        image_data_out[pixel_offset+6] = image_data_out[pixel_offset+7] = image_data_out[pixel_offset+8] = gray_avx[2];
        image_data_out[pixel_offset+9] = image_data_out[pixel_offset+10] = image_data_out[pixel_offset+10] = gray_avx[3];
        image_data_out[pixel_offset+12] = image_data_out[pixel_offset+13] = image_data_out[pixel_offset+14] = gray_avx[4];
        image_data_out[pixel_offset+15] = image_data_out[pixel_offset+16] = image_data_out[pixel_offset+17] = gray_avx[5];
        image_data_out[pixel_offset+18] = image_data_out[pixel_offset+19] = image_data_out[pixel_offset+20] = gray_avx[6];
        image_data_out[pixel_offset+21] = image_data_out[pixel_offset+22] = image_data_out[pixel_offset+23] = gray_avx[7];

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
