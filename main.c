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

    printf("%s %s", filename, output);
    
    PPMImage *image = ppm_readimage(filename);
    ppm_writeimage(output, image);
    ppm_destroy(image);

    return EXIT_SUCCESS;
}