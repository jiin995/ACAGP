/**
 * Author: Gabriele Previtera
 * Basic example that works in memory:
 * It takes an image (bitmap format) and creates another in which the colors are a grayscale.
 * In this code the entire image is loaded into memory, then the color conversion formula 
 * is applied and the new pixel is always saved in memory and at the end it is written to a file. 
 * 
 * This version use Intel AVX and use 
 *     
 * gray =  (R + G + B)/0.33
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//AVX
#include <immintrin.h> 
#include <x86intrin.h> 

typedef struct {
   unsigned char r;
   unsigned char g;
   unsigned char b;
} pixel_struct;

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

    pixel_struct *pixel_data_in __attribute__((aligned(32)));
    pixel_struct *pixel_data_out __attribute__((aligned(32))) = malloc(sizeof(char)*image_size_bytes);
    
    size_t num_read = fread(image_data_in, image_size_bytes, 1, fIn);

    if(num_read < 0){
        perror("Read Error");
        free(image_data_in);
        free(pixel_data_out);
        exit(-1);
    }

    pixel_data_in = (pixel_struct *) image_data_in;

    int n_pixels = height*width;
    printf("n_pixels: %d\n", n_pixels);
    printf("n_pixels/8: %d\n", n_pixels/8);
    
    start = clock();

    __m256 to_gray_percentage = _mm256_set1_ps(0.33);

    for(int pixel_index = 0; pixel_index < n_pixels/8; pixel_index++){

        int offset = pixel_index*8;

        pixel_struct *pixel = pixel_data_in+(offset);
        pixel_struct *pixel_out = pixel_data_out+(offset);

        __m256 gray_avx =  _mm256_setzero_ps();

        __m256 r_avx = _mm256_set_ps( pixel[0].r, pixel[1].r, pixel[2].r, pixel[3].r, pixel[4].r, pixel[5].r, pixel[6].r, pixel[7].r );
        __m256 g_avx = _mm256_set_ps( pixel[0].g, pixel[1].g, pixel[2].g, pixel[3].g, pixel[4].g, pixel[5].g, pixel[6].g, pixel[7].g );
        __m256 b_avx = _mm256_set_ps( pixel[0].b, pixel[1].b, pixel[2].b, pixel[3].b, pixel[4].b, pixel[5].b, pixel[6].b, pixel[7].b );

        gray_avx = _mm256_add_ps(r_avx, g_avx);
        gray_avx = _mm256_add_ps(gray_avx, b_avx);
        gray_avx = _mm256_mul_ps(gray_avx, to_gray_percentage);

        /*
        for(int i=0; i<8; i++){
            pixel_out[i].r = pixel_out[i].g = pixel_out[i].b = gray_avx[i];
        }
        */

        /*for(int i=0; i<8; i++){
           printf("gray_avx[]: %x\n",gray_avx[i]);
           printf("gray_avx[%d]_c: %x\n",i, (unsigned char)gray_avx[i]);

        }*/
        
        pixel_out[0].r = pixel_out[0].g = pixel_out[0].b = gray_avx[0];
        pixel_out[1].r = pixel_out[1].g = pixel_out[1].b = gray_avx[1];
        pixel_out[2].r = pixel_out[2].g = pixel_out[2].b = gray_avx[2];
        pixel_out[3].r = pixel_out[3].g = pixel_out[3].b = gray_avx[3];
        pixel_out[4].r = pixel_out[4].g = pixel_out[4].b = gray_avx[4];
        pixel_out[5].r = pixel_out[5].g = pixel_out[5].b = gray_avx[5];
        pixel_out[6].r = pixel_out[6].g = pixel_out[6].b = gray_avx[6];
        pixel_out[7].r = pixel_out[7].g = pixel_out[7].b = gray_avx[7];

    }

    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Used time %f s\n", cpu_time_used);

    fwrite((unsigned char *) pixel_data_out, image_size_bytes, 1, fOut);

    fclose(fOut);
    fclose(fIn);

    free(image_data_in);
    free(pixel_data_out);
    free(image_data_out);
    
    return 0;
}
