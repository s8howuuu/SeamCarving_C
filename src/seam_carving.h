#ifndef seam_carving_h
#define seam_carving_h

#include "imglib.h"
#include <stdio.h>
#include <limits.h>

#define NO_LAST 10

typedef struct seam{
	struct seam *left;
	struct seam *right;
	struct seam *last; 
	struct seam *down;
	struct seam *lastseam; 

	pixel1 lum;
	pixel col;
	int gra;
	long energy;
} seam;
typedef struct {
    int width;
	int height;
	int width_actural;
	seam *buf;
	seam *first_pixel;
	seam *last_pixel;
} seam_bundle_t;
typedef seam_bundle_t * seam_bundle;
//void transpose(image a);
//grayimage gradient(grayimage);
seam_bundle bundlize(image img, char *filter);
seam* find_seam(seam_bundle s,int t);
void compute_energy(seam_bundle s);
void free_seam_bundle(seam_bundle s);
// half(image a);
image seam_carving(image img,int target_width, char *filter, char *direction, char *mode);
#endif 
