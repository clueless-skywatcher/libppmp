#define _GNU_SOURCE

#include <stdio.h>
#include <libgen.h>

#include "ppm/ppm.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("usage: %s <filename>", argv[0]);
        return EXIT_FAILURE;
    }
    
    char *filename = argv[1];
    char *output = NULL;
    asprintf(&output, "rainbow_%s", basename(filename));
    
    PPMImage *image = ppm_readimage(filename);

    Pixel red = {255, 0, 0};

    ppm_set_pixel(image, 2, 2, &red);
    Pixel *pixel = ppm_get_pixel(image, 3, 3);
    printf("%d %d %d\n", pixel->r, pixel->g, pixel->b);

    ppm_writeimage(output, image);
    ppm_destroy(image);

    return EXIT_SUCCESS;
}