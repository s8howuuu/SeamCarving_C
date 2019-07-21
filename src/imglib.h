#ifndef _imglib_h
#define _imglib_h
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <math.h>
#include <sys/queue.h>
#define GET_PIXEL(IMG, X, Y) (IMG->buf[ ((Y) * IMG->width + (X)) ])
typedef int color_component;
typedef color_component pixel[3];
typedef struct {
    int width;
    int height;
    pixel * buf;
} image_t;
typedef image_t * image;
typedef int luminance;
typedef luminance pixel1[1];
typedef struct {
    int width;
    int height;
    pixel1 *buf;
} grayimage_t;
typedef grayimage_t * grayimage;
image alloc_img(int width, int height);
void free_img(image);
image get_ppm(FILE *pf);
void output_ppm(FILE *fd, image img);
grayimage alloc_grayimg(int width, int height);
grayimage tograyscale(image);
void free_grayimg(grayimage);
image tocolor(grayimage);
#endif
