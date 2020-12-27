/**
 * Esempio base che però lavora in memoria:
 * Prende un'immagine (formato bitmap) e ne crea un'altra in cui i colori 
 * sono una scala di grigi.
 * In questo codice si carica l'intera immagine in memoria, poi si applica 
 * la formula di conversione del colore e si salva il nuovo pixel 
 * sempre in memoria e alla fine si scrive su file. 
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
    FILE *fIn = fopen("sample_5184×3456.bmp", "rb");
    FILE *fOut = fopen("sample_5184×3456_gray.bmp", "wb");

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

    //unsigned char *image_data_in = malloc(sizeof(char)*image_size_bytes);
    //unsigned char *image_data_out = malloc(sizeof(char)*image_size_bytes);

    unsigned char pixel[3] __attribute__((aligned(32)));
    
    size_t num_read = fread(image_data_in, image_size_bytes, 1, fIn);

    if(num_read < 0){
        perror("Read Error");
        free(image_data_in);
        free(image_data_out);
        exit(-1);
    }

    unsigned int readed_bytes = 0;
    /*for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            pixel[0] = image_data_in[readed_bytes];
            pixel[1] = image_data_in[readed_bytes+1];
            pixel[3] = image_data_in[readed_bytes+2];
            unsigned char gray = pixel[0] * 0.3 + pixel[1] * 0.58 + pixel[2] * 0.11;
            memset(pixel, gray, sizeof(pixel));

            image_data_out[readed_bytes++] = pixel[0];
            image_data_out[readed_bytes++] = pixel[1];
            image_data_out[readed_bytes++] = pixel[2];

        }
    }*/
    int n_pixels = height*width;
    printf("n_pixels: %d\n", n_pixels);
    printf("n_pixels/8: %d\n", n_pixels/32);

    start = clock();
    for(int pixel_index = 0; pixel_index < n_pixels/8; pixel_index++){
    //for(int pixel_index = 0; pixel_index < 2; pixel_index++){

        /*unsigned char r[32] __attribute__((aligned(32))) = { image_data_in[pixel_index], image_data_in[pixel_index+3], image_data_in[pixel_index+6],
                                image_data_in[pixel_index+9], image_data_in[pixel_index+12],image_data_in[pixel_index+15],
                                image_data_in[pixel_index+18], image_data_in[pixel_index+21], image_data_in[pixel_index+24],
                                image_data_in[pixel_index+27], image_data_in[pixel_index+30], image_data_in[pixel_index+33],
                                image_data_in[pixel_index+36], image_data_in[pixel_index+39], image_data_in[pixel_index+42],
                                image_data_in[pixel_index+45], image_data_in[pixel_index+48], image_data_in[pixel_index+51],
                                image_data_in[pixel_index+54], image_data_in[pixel_index+57], image_data_in[pixel_index+60],
                                image_data_in[pixel_index+63], image_data_in[pixel_index+66], image_data_in[pixel_index+69],
                                image_data_in[pixel_index+72], image_data_in[pixel_index+75], image_data_in[pixel_index+78],
                                image_data_in[pixel_index+81], image_data_in[pixel_index+84], image_data_in[pixel_index+87],
                                image_data_in[pixel_index+90], image_data_in[pixel_index+93]
        };*/

        /*unsigned char r[16] __attribute__((aligned(32))) = { image_data_in[pixel_index], image_data_in[pixel_index+3], image_data_in[pixel_index+6],
                                image_data_in[pixel_index+9], image_data_in[pixel_index+12],image_data_in[pixel_index+15],
                                image_data_in[pixel_index+18], image_data_in[pixel_index+21], image_data_in[pixel_index+24],
                                image_data_in[pixel_index+27], image_data_in[pixel_index+30], image_data_in[pixel_index+33],
                                image_data_in[pixel_index+36], image_data_in[pixel_index+39], image_data_in[pixel_index+42],
                                image_data_in[pixel_index+45]
        };*/

        float r[8] __attribute__((aligned(32))) = { image_data_in[pixel_index*24], image_data_in[pixel_index*24+3], image_data_in[pixel_index*24+6],
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
        
        //printf("r:\n %d %d %d %d\n", image_data_in[pixel_index], image_data_in[pixel_index+1], image_data_in[pixel_index+2], image_data_in[pixel_index+3]);
        __m256 gray_avx =  _mm256_setzero_ps();

        __m256 r_avx = _mm256_load_ps(r);
        __m256 g_avx = _mm256_load_ps(g);
        __m256 b_avx = _mm256_load_ps(b);

        __m256 r_percentage = _mm256_set1_ps(0.3);
        __m256 g_percentage = _mm256_set1_ps(0.58);
        __m256 b_percentage = _mm256_set1_ps(0.11);

        r_avx = _mm256_mul_ps(r_avx, r_percentage);
        g_avx = _mm256_mul_ps(g_avx, g_percentage);
        b_avx = _mm256_mul_ps(b_avx, b_percentage);

        gray_avx = _mm256_add_ps(r_avx, g_avx);
        gray_avx = _mm256_add_ps(gray_avx, b_avx);

        //unsigned char gray_avx_c[8];

        /*for(int i=0; i<8; i++){
            memset(image_data_out+(pixel_index*24)+i*3,  gray_avx[i], 3);
        }*/
        memset(image_data_out+(pixel_index*24),  gray_avx[0], 3);
        memset(image_data_out+(pixel_index*24)+3,  gray_avx[1], 3);
        memset(image_data_out+(pixel_index*24)+6,  gray_avx[2], 3);
        memset(image_data_out+(pixel_index*24)+9,  gray_avx[3], 3);
        memset(image_data_out+(pixel_index*24)+12,  gray_avx[4], 3);
        memset(image_data_out+(pixel_index*24)+15,  gray_avx[5], 3);
        memset(image_data_out+(pixel_index*24)+18,  gray_avx[6], 3);
        memset(image_data_out+(pixel_index*24)+21,  gray_avx[7], 3);
        
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
