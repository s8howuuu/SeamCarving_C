#include "imglib.h"

#define MAXCMDBxF 100
#define MAXFILENAMELEN 256
#define MAXFULLCMDBUF (MAXCMDBxF + MAXFILENAMELEN)
#define PPMREADBUFLEN 256
image alloc_img(int width, int height)
{
    image img;
    img = malloc(sizeof(image_t));
    img->buf = malloc(width * height * sizeof(pixel));
    img->width = width;
    img->height = height;
    return img;
}

void free_img(image img)
{
    free(img->buf);
    free(img);
}

void free_grayimg(grayimage grayimg)
{
    free_img((image)grayimg);
}
image get_ppm(FILE *pf)
{
    char buf[PPMREADBUFLEN], *t;
    image img;
    int w, h, d;
    int r;
    
    if (pf == NULL) return NULL;
    t = fgets(buf, PPMREADBUFLEN, pf);
    if ( (t == NULL) || ( strncmp(buf, "P3\n", 3) != 0 ) ) return NULL;
    t = fgets(buf, PPMREADBUFLEN, pf);
    if ( t == NULL ) return NULL;
    r = sscanf(buf, "%d %d", &w, &h);
    if ( r < 2 ) return NULL;
    
    r = fscanf(pf, "%d", &d);
    if ( (r < 1) || ( d != 255 ) ) return NULL;
    fseek(pf, 1, SEEK_CUR);
    
    img = alloc_img(w, h);
	if (img != NULL) {
		fseek (pf, 1, SEEK_CUR);
		for (int i = 0; i < w * h; i++) {
			int tcs = 0;
			fscanf (pf, "%d", &tcs);
			img->buf[i][0] = tcs;
			fscanf (pf, "%d", &tcs);
			img->buf[i][1] = tcs;
			fscanf (pf, "%d", &tcs);
			img->buf[i][2] = tcs;
            if(i==w*h-1){
            //printf("%d %d %d \n",img->buf[i][0],img->buf[i][1],img->buf[i][2]);
                
            }
		}
	}
	else {
		return NULL;
	}
	fclose(pf);
    return img;
}
grayimage alloc_grayimg(int width, int height)
{
    grayimage img;
    img = malloc(sizeof(grayimage_t));
    img->buf = malloc(width*height*sizeof(pixel1));
    img->width = width;
    img->height = height;
    return img;
}
grayimage tograyscale(image img)
{
    int x, y;
    grayimage timg;
    double rc, gc, bc;
    double l;
    int ofs;
    
    timg = alloc_grayimg(img->width, img->height);
    
    for(x=0; x < img->width; x++)
    {
        for(y=0; y < img->height; y++)
        {
            ofs = (y * img->width) + x;
            rc = (double)img->buf[ofs][0];
            gc = (double)img->buf[ofs][1];
            bc = (double)img->buf[ofs][2];
            l = (rc + gc + bc)/3;
            timg->buf[ofs][0] = (luminance) (l);
        }
    }
    return timg;
}
image tocolor(grayimage img)
{
    int x, y;
    image timg;
    luminance l;
    int ofs;
    
    timg = alloc_img(img->width, img->height);
    
    for(x=0; x < img->width; x++)
    {
        for(y=0; y < img->height; y++)
        {
            ofs = (y * img->width) + x;
            l = img->buf[ofs][0];
            timg->buf[ofs][0] = l;
            timg->buf[ofs][1] = l;
            timg->buf[ofs][2] = l;
        }
    }
    return timg;
}
void output_ppm(FILE *fd, image img)
{
    int qs;
    fprintf(fd, "P3\n%d %d\n255\n", img->width, img->height);
    qs = img->height;
	for (int i = 0; i < qs; i++) {
		for (int n = 0; n < img->width; n++) {
			fprintf (fd, "%d %d %d ", img->buf[i*img->width+n][0], img->buf[i*img->width+n][1], img->buf[i*img->width+n][2]);
            if((i == qs-1)&&(n == img->width-1)){
                    //printf("%d %d %d \n", img->buf[i*img->width+n][0], img->buf[i*img->width+n][1], img->buf[i*img->width+n][2]);
            }
		}
		fprintf (fd, "\n");
	}
	fclose (fd);
}

