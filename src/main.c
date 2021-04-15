#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include "imglib.h"
#include "seam_carving.h"

int main(int argc, const char *argv[]){
	int i=0;
    char *filter = "Cental";//"Sobel", "Central", "Prewitt"; default = "Central(Simple)"
    int opt;
    opt = 0;
    char *np = NULL;
    int count = 50;
  
    char *open_name = "../data/owl.ppm";
    int test = 19; 


    char *direction = "Vertical";//"Horizontal";default = "Vertical"
    char *mode = "Enery";//"Seam","Energy","Gradient";default = "Picture"
    image t = get_ppm(fopen(open_name,"r"));
    if (t == NULL){
        exit(EXIT_FAILURE);
    }
    image t1=seam_carving(t,count,filter,direction,mode);
    if(t1==NULL){
        printf("image failure\n");
    }
    char *save_name = "../data/output.ppm";
    printf("System starts to print\n");
    output_ppm(fopen(save_name,"w"),t1);
    free_img(t1);

    return 0;
}

