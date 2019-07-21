#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "imglib.h"
#include "seam_carving.h"

int main(int argc, const char *argv[]){
    char *filter = "Cental";//"Sobel", "Central", "Prewitt"; default = "Central(Simple)"
    int opt;
    opt = 0;
    char *np = NULL;
    int fs01,fs02,fs03;
    int count = 10;
    fs01 = 111;
    fs02 = 111;
    fs03 = 111;
    while((opt = getopt(argc, argv,"spn:")) != -1){
    
        switch(opt){
            case 's':
            fs01 = 112;
            break;
            case 'p':
            fs02 = 112;
            break;
            case 'n':
            fs03 = 112;
            np = optarg;
            break;
        }
    }
    if(fs03==112){
    count = atoi(np);
    }
    char *open_name = "../data/owl.ppm";
    if(argv[argc-1]!=NULL){
    open_name = argv[argc-1];
    }
    if(fs01==112){
    image txr = get_ppm(fopen(open_name,"r"));
    grayimage gmit = tograyscale(txr);
    printf("width: %d\n",txr->width);
    printf("height: %d\n",txr->height);
    long lmn = 0;
    for(int i=0;i<(txr->width*txr->height);i++){
        lmn = (long)(gmit->buf[i][0])+lmn;
    }
    printf("brightness: %d\n",(int)(lmn/((long)(txr->width*txr->height))));
    free_img(txr);
    free_grayimg(gmit);
        exit(0);
    }
    if(fs02==112){
    image txr = get_ppm(fopen(open_name,"r"));
    grayimage gmit = tograyscale(txr);
    seam_bundle st = bundlize(txr,filter);
    compute_energy(st);
    int prs =2;
    seam* sppp = find_seam(st,2);
    free_seam_bundle(st);
        exit(0);
    }
    char *direction = "Vertical";//"Horizontal";default = "Vertical"
    char *mode = "Enery";//"Seam","Energy","Gradient";default = "Picture"
    image t = get_ppm(fopen(open_name,"r"));
    if (t == NULL){
        exit(EXIT_FAILURE);
    }
    image t1=seam_carving(t,count,filter,direction,mode);
    char *save_name = "../data/output.ppm";
    output_ppm(fopen(save_name,"w"),t1);
    free_img(t1);

    return 0;
}

