/**
 * Esempio base:
 * Prende un'immagine (formato bitmap) e ne crea un'altra in cui i colori 
 * sono una scala di grigi.
 * In questo codice si legge un pixel per volta dal file, si applica la 
 * formula di conversione del colore e salva il pixel in un nuovo file. 
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main( int argc, char *argv[] ){

    FILE *fIn = fopen("sample_5184×3456.bmp", "rb");
    FILE *fOut = fopen("sample_5184×3456_gray.bmp", "wb");

    /* time */
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

    printf("width: %d (%d)\n", width, width * 3);
    printf("height: %d\n", height);
    printf("stride: %d\n", stride);
    printf("padding: %d\n", padding);

    unsigned char pixel[3];
    
    start = clock();
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            fread(pixel, 3, 1, fIn);
            unsigned char gray = pixel[0] * 0.3 + pixel[1] * 0.58 + pixel[2] * 0.11;
            memset(pixel, gray, sizeof(pixel));
            fwrite(&pixel, 3, 1, fOut);
        }
        fread(pixel, padding, 1, fIn);
        fwrite(pixel, padding, 1, fOut);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Used time %f\n", cpu_time_used);

    fclose(fOut);
    fclose(fIn);
    return 0;
}
