#ifndef SPECTO_H
#define SPECTO_H

#include <fftw3.h>
#include <libpng/png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <err.h>

typedef struct list
{
    int size;
    double *data;
    struct list *next;
}list;

/* A coloured pixel. */
typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
}
pixel_t;

/* A picture. */
typedef struct
{
    pixel_t *pixels;
    size_t width;
    size_t height;
}
bitmap_t;

void spectogram(char *pathname);

#endif