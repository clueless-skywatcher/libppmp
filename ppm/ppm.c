#include "ppm.h"

PPMImage *ppm_readimage(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Error: could not open file");
        return NULL;
    }

    PPMImage *image = malloc(sizeof *image);

    char type[3] = {0};
    fscanf(file, "%2s", type);
    if (strcmp(type, "P6") != 0) {
        perror("Not a valid P6 format");
        fclose(file);
        return NULL;
    }

    fgetc(file);
    char c = fgetc(file);
    if (c == '#') {
        do {
            c = fgetc(file);
        } while (c != '\n' && c != EOF);
    } else {
        ungetc(c, file);
    }

    fscanf(file, "%u %u", &(image->cols), &(image->rows));
    fscanf(file, "%u", &(image->maxval));

    fgetc(file);

    uint8_t bytes_per_value = image->maxval > 256 ? 2 : 1;
    
    image->pixels = calloc(image->rows, sizeof(Pixel*));

    for (int y = 0; y < image->rows; y++) {
        image->pixels[y] = calloc(image->cols, sizeof(Pixel));
        for (int x = 0; x < image->cols; x++) {
            if (bytes_per_value == 1) {
                fread(&(image->pixels[y][x].r), 1, 1, file);
                fread(&(image->pixels[y][x].g), 1, 1, file);
                fread(&(image->pixels[y][x].b), 1, 1, file);
            } else {
                uint16_t r1, r2, g1, g2, b1, b2;
                fread(&r1, 1, 1, file);
                fread(&g1, 1, 1, file);
                fread(&b1, 1, 1, file);
                fread(&r2, 1, 1, file);
                fread(&g2, 1, 1, file);
                fread(&b2, 1, 1, file);
                image->pixels[y][x].r = r1 << 8 | r2;
                image->pixels[y][x].g = g1 << 8 | g2;
                image->pixels[y][x].b = b1 << 8 | b2;
            }
        }
    }

    fclose(file);

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

Pixel *ppm_get_pixel(PPMImage *image, int x, int y) {
    return &(image->pixels[y][x]);
}

void ppm_set_pixel(PPMImage *image, int x, int y, Pixel *pixel) {
    image->pixels[y][x] = *pixel;
}
