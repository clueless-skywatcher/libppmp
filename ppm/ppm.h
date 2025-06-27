#ifndef __PPM_H__
#define __PPM_H__

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>

typedef struct {
    uint16_t r, g, b;
} Pixel;


typedef struct {
    unsigned int cols; // Width of the image
    unsigned int rows; // Height of the image
    unsigned int maxval; // Maximum colour depth of a pixel
    
    Pixel **pixels;

} PPMImage;

PPMImage *ppm_readimage(const char *filename);
void ppm_destroy(PPMImage *image);
bool ppm_writeimage(const char *filename, PPMImage *image);

#endif