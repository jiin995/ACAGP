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

    unsigned char *image_data_in = malloc(sizeof(char)*image_size_bytes);
    unsigned char *image_data_out = malloc(sizeof(char)*image_size_bytes);

    unsigned char pixel[3];
    
    size_t num_read = fread(image_data_in, image_size_bytes, 1, fIn);

    if(num_read < 0){
        perror("Read Error");
        free(image_data_in);
        free(image_data_out);
        exit(-1);
    }

    unsigned int readed_bytes = 0;
    start = clock();
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            pixel[0] = image_data_in[readed_bytes];
            pixel[1] = image_data_in[readed_bytes+1];
            pixel[3] = image_data_in[readed_bytes+2];
            
            //unsigned char gray = pixel[0] * 0.3 + pixel[1] * 0.58 + pixel[2] * 0.11;
            
            unsigned char gray = (pixel[0] + pixel[1] + pixel[2])*0.33;

            //memset(pixel, gray, sizeof(pixel));

            image_data_out[readed_bytes++] = gray;
            image_data_out[readed_bytes++] = gray;
            image_data_out[readed_bytes++] = gray;

        }
    }
    fwrite(image_data_out, image_size_bytes, 1, fOut);

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Used time %f\n", cpu_time_used);

    fclose(fOut);
    fclose(fIn);

    free(image_data_in);
    free(image_data_out);
    
    return 0;
}
