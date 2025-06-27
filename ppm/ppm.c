#include "ppm.h"

PPMImage *ppm_readimage(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error: could not open file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    size_t filesize = ftell(file);

    int file_desc = fileno(file);
    rewind(file);

    PPMImage *image = malloc(sizeof *image);

    char type[3] = {0};
    fscanf(file, "%2s", type);
    if (strcmp(type, "P6") != 0) {
        perror("Not a valid P6 format");
        fclose(file);
        return NULL;
    }

    fscanf(file, "%u %u", &(image->cols), &(image->rows));
    fscanf(file, "%u", &(image->maxval));

    off_t offset = ftell(file);

    uint8_t *rawdata = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, file_desc, 0);

    if (rawdata == MAP_FAILED) {
        perror("mmap error");
        fclose(file);
        free(image);
        return NULL;
    }

    uint8_t bytes_per_value = image->maxval > 256 ? 2 : 1;
    const int values_per_pixel = 3;

    uint8_t *pixeldata = rawdata + offset + 1;

    image->pixels = calloc(image->rows, sizeof(Pixel*));

    for (int y = 0; y < image->rows; y++) {
        image->pixels[y] = calloc(image->cols, sizeof(Pixel));
        for (int x = 0; x < image->cols; x++) {
            uint8_t *pixelstart = pixeldata 
                + (y * image->cols * values_per_pixel * bytes_per_value)
                + (x * values_per_pixel * bytes_per_value);
            if (bytes_per_value == 1) {
                image->pixels[y][x].r = pixelstart[0];
                image->pixels[y][x].g = pixelstart[1];
                image->pixels[y][x].b = pixelstart[2];
            } else {
                image->pixels[y][x].r = (pixelstart[0] << 8) | pixelstart[1];
                image->pixels[y][x].g = (pixelstart[2] << 8) | pixelstart[3];
                image->pixels[y][x].b = (pixelstart[4] << 8) | pixelstart[5];
            }
        }
    }

    return image;
}

void ppm_destroy(PPMImage *image) {
    for (int y = 0; y < image->rows; y++) {
        free(image->pixels[y]);
    }
    free(image);
}

bool ppm_writeimage(const char *filename, PPMImage *image) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Failed to open file");
        return false;
    }

    fprintf(file, "P6\n%u %u\n%u\n", image->cols, image->rows, image->maxval);
    
    uint8_t bytes_per_value = image->maxval > 256 ? 2 : 1;
    for (int y = 0; y < image->rows; y++) {
        for (int x = 0; x < image->cols; x++) {
            fwrite(&(image->pixels[y][x].r), bytes_per_value, 1, file);
            fwrite(&(image->pixels[y][x].g), bytes_per_value, 1, file);
            fwrite(&(image->pixels[y][x].b), bytes_per_value, 1, file);
        }
    }

    return true;
}