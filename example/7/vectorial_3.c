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
    
    size_t num_read = fread(image_data_in, image_size_bytes, 1, fIn);

    if(num_read < 0){
        perror("Read Error");
        free(image_data_in);
        free(image_data_out);
        exit(-1);
    }

    int n_pixels = height*width;
    printf("n_pixels: %d\n", n_pixels);
    printf("n_pixels/32: %d\n", n_pixels/32);
    
    start = clock();

    __m256 r_percentage = _mm256_set1_ps(0.3);
    __m256 g_percentage = _mm256_set1_ps(0.58);
    __m256 b_percentage = _mm256_set1_ps(0.11);
    __m256 r_avx, g_avx, b_avx, gray_avx;
    
    int pixel_offset ;
    
    for(int pixel_index = 0; pixel_index < n_pixels/32; pixel_index++){
        pixel_offset = pixel_index*96;
        
        gray_avx =  _mm256_setzero_ps();

        unsigned char r[32] __attribute__((aligned(32))) = { image_data_in[pixel_offset], image_data_in[pixel_offset+3], image_data_in[pixel_offset+6],
                                image_data_in[pixel_offset+9], image_data_in[pixel_offset+12],image_data_in[pixel_offset+15],
                                image_data_in[pixel_offset+18], image_data_in[pixel_offset+21], image_data_in[pixel_offset+24],
                                image_data_in[pixel_offset+27], image_data_in[pixel_offset+30], image_data_in[pixel_offset+33],
                                image_data_in[pixel_offset+36], image_data_in[pixel_offset+39], image_data_in[pixel_offset+42],
                                image_data_in[pixel_offset+45], image_data_in[pixel_offset+48], image_data_in[pixel_offset+51],
                                image_data_in[pixel_offset+54], image_data_in[pixel_offset+57], image_data_in[pixel_offset+60],
                                image_data_in[pixel_offset+63], image_data_in[pixel_offset+66], image_data_in[pixel_offset+69],
                                image_data_in[pixel_offset+72], image_data_in[pixel_offset+75], image_data_in[pixel_offset+78],
                                image_data_in[pixel_offset+81], image_data_in[pixel_offset+84], image_data_in[pixel_offset+87],
                                image_data_in[pixel_offset+90], image_data_in[pixel_offset+93]
        };

        unsigned char g[32] __attribute__((aligned(32))) = { image_data_in[pixel_offset+1], image_data_in[pixel_offset+4], image_data_in[pixel_offset+7],
                                image_data_in[pixel_offset+10], image_data_in[pixel_offset+13],image_data_in[pixel_offset+16],
                                image_data_in[pixel_offset+19], image_data_in[pixel_offset+22], image_data_in[pixel_offset+25],
                                image_data_in[pixel_offset+28], image_data_in[pixel_offset+31], image_data_in[pixel_offset+34],
                                image_data_in[pixel_offset+37], image_data_in[pixel_offset+40], image_data_in[pixel_offset+43],
                                image_data_in[pixel_offset+46], image_data_in[pixel_offset+49], image_data_in[pixel_offset+52],
                                image_data_in[pixel_offset+55], image_data_in[pixel_offset+58], image_data_in[pixel_offset+61],
                                image_data_in[pixel_offset+64], image_data_in[pixel_offset+67], image_data_in[pixel_offset+70],
                                image_data_in[pixel_offset+73], image_data_in[pixel_offset+76], image_data_in[pixel_offset+79],
                                image_data_in[pixel_offset+82], image_data_in[pixel_offset+85], image_data_in[pixel_offset+88],
                                image_data_in[pixel_offset+91], image_data_in[pixel_offset+94]
        };

        unsigned char b[32] __attribute__((aligned(32))) = { image_data_in[pixel_offset+2], image_data_in[pixel_offset+5], image_data_in[pixel_offset+8],
                                image_data_in[pixel_offset+11], image_data_in[pixel_offset+14],image_data_in[pixel_offset+17],
                                image_data_in[pixel_offset+20], image_data_in[pixel_offset+23], image_data_in[pixel_offset+26],
                                image_data_in[pixel_offset+29], image_data_in[pixel_offset+32], image_data_in[pixel_offset+35],
                                image_data_in[pixel_offset+38], image_data_in[pixel_offset+41], image_data_in[pixel_offset+44],
                                image_data_in[pixel_offset+47], image_data_in[pixel_offset+50], image_data_in[pixel_offset+53],
                                image_data_in[pixel_offset+56], image_data_in[pixel_offset+59], image_data_in[pixel_offset+62],
                                image_data_in[pixel_offset+65], image_data_in[pixel_offset+68], image_data_in[pixel_offset+71],
                                image_data_in[pixel_offset+74], image_data_in[pixel_offset+77], image_data_in[pixel_offset+80],
                                image_data_in[pixel_offset+83], image_data_in[pixel_offset+86], image_data_in[pixel_offset+89],
                                image_data_in[pixel_offset+92], image_data_in[pixel_offset+95]
        };

        __m256i_u r_avx = _mm256_loadu_si256((__m256i_u*) &r);
        __m256i_u g_avx = _mm256_loadu_si256((__m256i_u*) &g);
        __m256i_u b_avx = _mm256_loadu_si256((__m256i_u*) &b);

        __m256i_u gray_avx = _mm256_add_epi8(r_avx, g_avx);
        gray_avx = _mm256_add_epi8(gray_avx, b_avx);

        unsigned char *gray_avx_c = (unsigned char *) &gray_avx;

        image_data_out[pixel_offset] = image_data_out[pixel_offset+1] = image_data_out[pixel_offset+2] = gray_avx_c[0];
        image_data_out[pixel_offset+3] = image_data_out[pixel_offset+4] = image_data_out[pixel_offset+5] = gray_avx_c[1];
        image_data_out[pixel_offset+6] = image_data_out[pixel_offset+7] = image_data_out[pixel_offset+8] = gray_avx_c[2];
        image_data_out[pixel_offset+9] = image_data_out[pixel_offset+10] = image_data_out[pixel_offset+10] = gray_avx_c[3];
        image_data_out[pixel_offset+12] = image_data_out[pixel_offset+13] = image_data_out[pixel_offset+14] = gray_avx_c[4];
        image_data_out[pixel_offset+15] = image_data_out[pixel_offset+16] = image_data_out[pixel_offset+17] = gray_avx_c[5];
        image_data_out[pixel_offset+18] = image_data_out[pixel_offset+19] = image_data_out[pixel_offset+20] = gray_avx_c[6];
        image_data_out[pixel_offset+21] = image_data_out[pixel_offset+22] = image_data_out[pixel_offset+23] = gray_avx_c[7];
        image_data_out[pixel_offset+24] = image_data_out[pixel_offset+25] = image_data_out[pixel_offset+26] = gray_avx_c[8];
        image_data_out[pixel_offset+27] = image_data_out[pixel_offset+28] = image_data_out[pixel_offset+29] = gray_avx_c[9];
        image_data_out[pixel_offset+30] = image_data_out[pixel_offset+31] = image_data_out[pixel_offset+32] = gray_avx_c[10];
        image_data_out[pixel_offset+33] = image_data_out[pixel_offset+34] = image_data_out[pixel_offset+35] = gray_avx_c[11];
        image_data_out[pixel_offset+36] = image_data_out[pixel_offset+37] = image_data_out[pixel_offset+38] = gray_avx_c[12];
        image_data_out[pixel_offset+39] = image_data_out[pixel_offset+40] = image_data_out[pixel_offset+41] = gray_avx_c[13];
        image_data_out[pixel_offset+42] = image_data_out[pixel_offset+43] = image_data_out[pixel_offset+44] = gray_avx_c[14];
        image_data_out[pixel_offset+45] = image_data_out[pixel_offset+46] = image_data_out[pixel_offset+47] = gray_avx_c[15];
        image_data_out[pixel_offset+48] = image_data_out[pixel_offset+49] = image_data_out[pixel_offset+50] = gray_avx_c[16];
        image_data_out[pixel_offset+51] = image_data_out[pixel_offset+52] = image_data_out[pixel_offset+53] = gray_avx_c[17];
        image_data_out[pixel_offset+54] = image_data_out[pixel_offset+55] = image_data_out[pixel_offset+56] = gray_avx_c[18];
        image_data_out[pixel_offset+57] = image_data_out[pixel_offset+58] = image_data_out[pixel_offset+59] = gray_avx_c[19];
        image_data_out[pixel_offset+60] = image_data_out[pixel_offset+61] = image_data_out[pixel_offset+62] = gray_avx_c[20];
        image_data_out[pixel_offset+63] = image_data_out[pixel_offset+64] = image_data_out[pixel_offset+65] = gray_avx_c[21];
        image_data_out[pixel_offset+66] = image_data_out[pixel_offset+67] = image_data_out[pixel_offset+68] = gray_avx_c[22];
        image_data_out[pixel_offset+69] = image_data_out[pixel_offset+70] = image_data_out[pixel_offset+71] = gray_avx_c[23];
        image_data_out[pixel_offset+72] = image_data_out[pixel_offset+73] = image_data_out[pixel_offset+74] = gray_avx_c[24];
        image_data_out[pixel_offset+75] = image_data_out[pixel_offset+76] = image_data_out[pixel_offset+77] = gray_avx_c[25];
        image_data_out[pixel_offset+78] = image_data_out[pixel_offset+79] = image_data_out[pixel_offset+80] = gray_avx_c[26];
        image_data_out[pixel_offset+81] = image_data_out[pixel_offset+82] = image_data_out[pixel_offset+83] = gray_avx_c[27];
        image_data_out[pixel_offset+84] = image_data_out[pixel_offset+85] = image_data_out[pixel_offset+86] = gray_avx_c[28];
        image_data_out[pixel_offset+87] = image_data_out[pixel_offset+88] = image_data_out[pixel_offset+89] = gray_avx_c[29];
        image_data_out[pixel_offset+90] = image_data_out[pixel_offset+91] = image_data_out[pixel_offset+92] = gray_avx_c[30];
        image_data_out[pixel_offset+93] = image_data_out[pixel_offset+94] = image_data_out[pixel_offset+95] = gray_avx_c[31];        
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
