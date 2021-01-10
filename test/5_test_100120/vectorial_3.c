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
        free(image_data_out);
        exit(-1);
    }

    pixel_data_in = (pixel_struct *) image_data_in;


    int n_pixels = height*width;
    printf("n_pixels: %d\n", n_pixels);
    printf("n_pixels/32: %d\n", n_pixels/32);
    
    start = clock();

    for(int pixel_index = 0; pixel_index < n_pixels/32; pixel_index++){
        int offset = pixel_index*32;

        pixel_struct *pixel = pixel_data_in+(offset);
        pixel_struct *pixel_out = pixel_data_out+(offset);
/*

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
        */

        __m256i r_avx = _mm256_set_epi8( pixel[0].r, pixel[1].r, pixel[2].r, pixel[3].r, pixel[4].r, pixel[5].r, pixel[6].r, pixel[7].r,
            pixel[8].r, pixel[9].r, pixel[10].r, pixel[11].r, pixel[12].r, pixel[13].r, pixel[14].r, pixel[15].r ,
            pixel[16].r, pixel[17].r, pixel[18].r, pixel[19].r, pixel[20].r, pixel[21].r, pixel[22].r, pixel[23].r,
            pixel[24].r, pixel[25].r, pixel[26].r, pixel[27].r, pixel[28].r, pixel[29].r, pixel[30].r, pixel[31].r);
        __m256i g_avx = _mm256_set_epi8( pixel[0].g, pixel[1].g, pixel[2].g, pixel[3].g, pixel[4].g, pixel[5].g, pixel[6].g, pixel[7].g,
            pixel[8].g, pixel[9].g, pixel[10].g, pixel[11].g, pixel[12].g, pixel[13].g, pixel[14].g, pixel[15].g ,
            pixel[16].g, pixel[17].g, pixel[18].g, pixel[19].g, pixel[20].g, pixel[21].g, pixel[22].g, pixel[23].g,
            pixel[24].g, pixel[25].g, pixel[26].g, pixel[27].g, pixel[28].g, pixel[29].g, pixel[30].g, pixel[31].g);
        __m256i b_avx = _mm256_set_epi8( pixel[0].b, pixel[1].b, pixel[2].b, pixel[3].b, pixel[4].b, pixel[5].b, pixel[6].b, pixel[7].b,
            pixel[8].b, pixel[9].b, pixel[10].b, pixel[11].b, pixel[12].b, pixel[13].b, pixel[14].b, pixel[15].b ,
            pixel[16].b, pixel[17].b, pixel[18].b, pixel[19].b, pixel[20].b, pixel[21].b, pixel[22].b, pixel[23].b,
            pixel[24].b, pixel[25].b, pixel[26].b, pixel[27].b, pixel[28].b, pixel[29].b, pixel[30].b, pixel[31].b);


        __m256i_u gray_avx = _mm256_add_epi8(r_avx, g_avx);
        gray_avx = _mm256_add_epi8(gray_avx, b_avx);

        unsigned char *gray_avx_c = (unsigned char *) &gray_avx;

        for(int i=0; i<32; i++){
            //memset(image_data_out+(pixel_index*96)+i*3,  gray_avx_c[i], 3);
            pixel_out[i].r = pixel_out[i].g = pixel_out[i].b = gray_avx_c[i];

        }       
    }

    end = clock();

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Used time %f s\n", cpu_time_used);

    fwrite((unsigned char *) pixel_data_out, image_size_bytes, 1, fOut);

    fclose(fOut);
    fclose(fIn);

    free(image_data_in);
    free(image_data_out);
    
    return 0;
}
