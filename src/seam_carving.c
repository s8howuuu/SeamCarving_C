#include <math.h>
#include <string.h>

#include "imglib.h"
#include "seam_carving.h"

int max(int x, int y){return x > y ? x : y;}
int min(int x, int y){return x < y ? x : y;}

/*void transpose(image a){
    int Y = a->height,X = a->width,x,y;
    image b = alloc_img(Y,X);
    for(x = 0; x < X; x++){
        for(y = 0; y < Y; y++){
            b->buf[x*Y+y][0] = a->buf[y*X+x][0];
            b->buf[x*Y+y][1] = a->buf[y*X+x][1];
            b->buf[x*Y+y][2] = a->buf[y*X+x][2];
        }
    }
    free_img(a);
    a = b;
}*/

/*image half(image a){
    int Y = a->height,X = a->width/2,x,y;
    image b = alloc_img(X,Y);
    for(x = 0; x < X; x++){
        for(y = 0; y < Y; y++){
            b->buf[y*X+x][0] = (a->buf[y*a->width+2*x][0]+a->buf[y*a->width+2*x+1][0])/2;
            b->buf[y*X+x][1] = (a->buf[y*a->width+2*x][1]+a->buf[y*a->width+2*x+1][1])/2;
            b->buf[y*X+x][2] = (a->buf[y*a->width+2*x][2]+a->buf[y*a->width+2*x+1][2])/2;
        }
    }
    return b;
}*/



seam_bundle alloc_seam_bundle(int width, int height)
{
    seam_bundle s;
    s = malloc(sizeof(seam_bundle_t));
    s->buf = malloc(height*width*sizeof(seam));
    s->first_pixel = s->buf;//avoid memory leak
    s->last_pixel = s->buf+width*height-1; //for finding least energy
    s->width = width;
    s->height = height;
    s->width_actural = width;
    return s;
}

void free_seam_bundle(seam_bundle s){
    free(s->buf);
    free(s);
}

/*grayimage enlarge(grayimage img,int edge){
    grayimage img_large = alloc_grayimg(img->width+2*edge,img->height+2*edge);
    int x,y;
    int targetx, targety;
    for(y=0;y<img->height+2*edge;y++){
        for (x=0; x<img->width+2*edge; x++) {
            targety=max(min(y-edge,img->height-1),0);
            targetx=max(min(x-edge,img->width-1),0);
            img_large->buf[y*img_large->width+x][0] = img->buf[targety*img->width+targetx][0];
        }
    }
    return img_large;
}*/
/*grayimage gradient(grayimage img){
    grayimage grad = alloc_grayimg(img->width,img->height);
    grayimage img_large = enlarge(img,1);
    
    int x,y;
    int X = img->width, Y = img->height;
    for(y=0;y<Y;y++){
        for (x=0; x<X; x++) {
            grad->buf[y*X+x][0] = abs(img_large->buf[(y+1)*(X+2)+x][0]-img_large->buf[(y+1)*(X+2)+x+2][0])
            +abs(img_large->buf[y*(X+2)+x+1][0]-img_large->buf[(y+2)*(X+2)+x+1][0]);
        }
    }
    free_grayimg(img_large);
    return grad;
}*/

/*grayimage gradient_filter(grayimage img, int wx[], int wy[]){
    grayimage grad = alloc_grayimg(img->width,img->height);
    grayimage img_large = enlarge(img,1);
    
    int x,y;
    int X = img->width+2, Y = img->height;
    for(y=0;y<Y;y++){
        for (x=0; x<X-2; x++) {
            grad->buf[y*(X-2)+x][0] = 
            abs(img_large->buf[y*X+x][0]*wx[0]     +img_large->buf[y*X+x+1][0]*wx[1]    +img_large->buf[y*X+x+2][0]*wx[2]
             +img_large->buf[(y+1)*X+x][0]*wx[3] +img_large->buf[(y+1)*X+x+1][0]*wx[4]+img_large->buf[(y+1)*X+x+2][0]*wx[5]
             +img_large->buf[(y+2)*X+x][0]*wx[6] +img_large->buf[(y+2)*X+x+1][0]*wx[7]+img_large->buf[(y+2)*X+x+2][0]*wx[8])
            + abs(img_large->buf[y*X+x][0]*wy[0]     +img_large->buf[y*X+x+1][0]*wy[1]    +img_large->buf[y*X+x+2][0]*wy[2]
             +img_large->buf[(y+1)*X+x][0]*wy[3] +img_large->buf[(y+1)*X+x+1][0]*wy[4]+img_large->buf[(y+1)*X+x+2][0]*wy[5]
             +img_large->buf[(y+2)*X+x][0]*wy[6] +img_large->buf[(y+2)*X+x+1][0]*wy[7]+img_large->buf[(y+2)*X+x+2][0]*wy[8]);
        }
    }
    free_grayimg(img_large);
    return grad;
}*/

seam_bundle bundlize(image img, char *filter){
    int width = img->width, height = img->height;
    seam_bundle s = alloc_seam_bundle(width,height);
    int i,y;

    grayimage gimg = tograyscale(img);
    //grayimage grad;

   /* if(!strcmp(filter,"Sobel")){
        int wx[9] = {1,0,-1,2,0,-2,1,0,-1};
        int wy[9] = {1,2,1,0,0,0,-1,-2,-1};
        grad = gradient_filter(gimg,wx,wy);
    }
    else if(!strcmp(filter,"Central")){
        int wx[9] = {0,0,0,1,0,-1,0,0,0};
        int wy[9] = {0,1,0,0,0,0,0,-1,0};
        grad = gradient_filter(gimg,wx,wy);
    }
    else if(!strcmp(filter,"Prewitt")){
        int wx[9] = {1,0,-1,1,0,-1,1,0,-1};
        int wy[9] = {1,1,1,0,0,0,-1,-1,-1};
        grad = gradient_filter(gimg,wx,wy);
    }
    else
        grad = gradient(gimg);*/
    (s->buf[0]).right = s->buf+1;
    (s->buf[0]).left = NULL;
    s->buf[width*height-1].left = s->buf+width*height-2;
    s->buf[width*height-1].right = NULL;
    for(i=1;i<width*height-1;i++){
        s->buf[i].right = s->buf+i+1;
        s->buf[i].left = s->buf+i-1;
    }
    //printf("%diiiiiiiii\n",i);
    for(i=0;i<width*height;i++)
	{
        s->buf[i].col[0] = img->buf[i][0];
        s->buf[i].col[1] = img->buf[i][1];
        s->buf[i].col[2] = img->buf[i][2];
        s->buf[i].lum[0] = gimg->buf[i][0];
        if(i==width*height-1){
            //printf("%d %d %d \n",s->buf[i].col[0],s->buf[i].col[1],s->buf[i].col[2]);
                
            }
        //s->buf[i].gra = (int) grad->buf[i][0];
    }
    //printf("%d\n",i);
    for(i = 0; i < width; i++){
        (s->buf[i]).last = NULL;
        (s->buf[i]).down = s->buf+width+i;
        for(y = 1; y < height-1; y ++){
            if(i == 198&&y ==209){
             
                //printf("198xxxx209\n");
                
            }
            (s->buf+y*width+i)->last = s->buf+(y-1)*width+i;
            (s->buf+y*width+i)->down = s->buf+(y+1)*width+i;
            if((s->buf+y*width+i)->last == NULL){
                    //printf("kikllllf01\n");
                }
                if((s->buf+y*width+i)->down == NULL){
                    //printf("kikllllf02\n");
                }
            if(i == 198&&y ==209){
             
                //printf("198xxxx209xx\n");
                if((s->buf+y*width+i)->last == NULL){
                    //printf("kikllllf01\n");
                }
                if((s->buf+y*width+i)->down == NULL){
                    //printf("kikllllf02\n");
                }
                
            }
        }
        (s->buf[(height-1)*width+i]).down = NULL;
        (s->buf[(height-1)*width+i]).last = s->buf+(height-2)*width+i;
    }
    //printf("%d,%d\n",i,y);
	(s->buf)->energy = 0;
    for(i=1;i<width;i++){
		s->buf[i].energy = ((((s->buf + i)->right)->col[0]) - (s->buf[i].col[0])) * ((((s->buf + i)->right)->col[0]) - (s->buf[i].col[0])) + ((((s->buf + i)->right)->col[1]) - (s->buf[i].col[1])) * ((((s->buf + i)->right)->col[1]) - ((s->buf + i)->col[1])) + ((((s->buf + i)->right)->col[2]) - ((s->buf + i)->col[2])) * ((((s->buf + i)->right)->col[2]) - ((s->buf + i)->col[2]));
        (s->buf+i)->lastseam = NULL;
    }
    //printf("%d\n",i);
    //free_grayimg(grad);
	for (int i = 1; i < height; i++) {
		s->buf[i * width].energy = ((((s->buf + i * width)->last)->col[0]) - ((s->buf[i * width]).col[0])) * ((((s->buf + i * width)->last)->col[0]) - ((s->buf[i * width]).col[0])) + ((((s->buf + i * width)->last)->col[1]) - ((s->buf[i * width]).col[1])) * ((((s->buf + i * width)->last)->col[1]) - ((s->buf[i * width]).col[1])) + ((((s->buf + i * width)->last)->col[2]) - ((s->buf[i * width]).col[2])) * ((((s->buf + i * width)->last)->col[2]) - ((s->buf[i * width]).col[2]));
	}
	//printf("%d\n",i);
	free_grayimg (gimg);
    free_img(img);
    return s;
}

image debundlize_color(seam_bundle s){
    image img = alloc_img(s->width_actural, s->height);
    int i=0;
    seam * p = s->first_pixel;
    while(p!=NULL){
        img->buf[i][0] = p->col[0];
        img->buf[i][1] = p->col[1];
        img->buf[i++][2] = p->col[2];
        p = p->right;
    }

    return img;
}
/*grayimage debundlize_gray(seam_bundle s){
    grayimage img = alloc_grayimg(s->width_actural, s->height);
    int i=0;
    seam * p = s->first_pixel;
    while(p->right!=NULL){
        img->buf[i++][0] = p->lum[0];
        p = p->right;
    }
    return img;
}*/
/*grayimage debundlize_gradient(seam_bundle s){
    grayimage img = alloc_grayimg(s->width_actural, s->height);
    int i=0;
    seam * p = s->first_pixel;
    while(p->right!=NULL){
        img->buf[i++][0] = (luminance)p->gra;
        p = p->right;
    }
    return img;
}*/
/*grayimage debundlize_gradient_full(seam_bundle s){
    grayimage img = alloc_grayimg(s->width, s->height);
    int i=0;
    while(i < s->width * s->height - 1){
        img->buf[i][0] = (luminance)(s->buf+i)->gra;
        i++;
    }
    return img;
}*/
/*grayimage debundlize_energy(seam_bundle s,double rate){
    grayimage img = alloc_grayimg(s->width_actural, s->height);
    int i=0;
    seam * p = s->first_pixel;
    while(p->right!=NULL){
        img->buf[i++][0] = (luminance) ((p->energy)/rate);
        p = p->right;
    }
    return img;
}*/


seam *which_min_p(seam *a,seam *b,seam *c){
    if(a->energy<b->energy){
        if(c->energy<a->energy) return c;
        else return a;
    }
    else{
        if(c->energy<b->energy) return c;
        else return b;
    }
}
seam *which_min_b(seam *a,seam *b){
    if(a->energy<b->energy){
        return a;
    }
    else{
        return b;
    }
}

seam *find_seam(seam_bundle s,int t){
    int stat = 0;
	seam *tpseam;
	long fucku[s->width_actural];
	long fuckur[s->width_actural];
	long temp = 0;
	int suckmc = 0;
	for (int i = 0; i <s->width_actural ; i++) {
		fucku[i] = (s->buf[(s->height - 1) * (s->width_actural) + i]).energy;
		fuckur[i] = (s->buf[(s->height - 1) * (s->width_actural) + i]).energy;
	}
	
	for (int i = 0; i < s->width_actural; i++) 
	{
		for (int j = i + 1; j < s->width_actural; j++) 
		{
			if (fuckur[i] > fuckur[j])
			{
				temp = fuckur[i];
				fuckur[i] = fuckur[j];
				fuckur[j] = temp;
			}
		}
	}
	for (int i = 0; i < s->width_actural; i++) {
		if (fuckur[0] == fucku[i]) {
			suckmc = i;
		}
	}
	    if(t==2){
            printf("%d\n",suckmc);
        }

	tpseam = s->buf+(s->height - 1) * (s->width_actural) + suckmc;
	seam* fks = tpseam;
    //printf("%d\n",s->width_actural);
    //printf("%ld\n",(tpseam-(s->first_pixel))%(s->width_actural));
    //printf("%ld\n",(tpseam-(s->first_pixel)+1)%(s->width_actural));
    //printf("%ld\n",(tpseam-(s->first_pixel)));
    for (int i = 0; i < s->height - 1; i++){
        //printf("%d\n",i);
		if(((((tpseam)-(s->first_pixel))%(s->width_actural)))&&((((tpseam)-(s->first_pixel)+1)%(s->width_actural)))){
            tpseam->lastseam = which_min_p ((tpseam->last)->right, tpseam->last, (tpseam->last)->left);
			if ((tpseam->lastseam)->energy == (tpseam->last)->energy){
			tpseam->lastseam = tpseam->last;
            if(t==2){
                stat = -1;
            printf("%d\n",suckmc);
        }
			}
			else if ((tpseam->lastseam)->energy == ((tpseam->last)->left)->energy) {
				tpseam->lastseam = (tpseam->last)->left;
                if(t==2){
                    stat = -1;
                    suckmc = suckmc-1;
            printf("%d\n",suckmc);
        }
            }
        if(t == 2&&stat == 0){
                    suckmc = suckmc+1;
            printf("%d\n",suckmc);
			}
	}else if((((tpseam)-(s->first_pixel))%(s->width_actural))==0){
        tpseam->lastseam = which_min_b ((tpseam->last)->right, tpseam->last);
			if ((tpseam->lastseam)->energy == (tpseam->last)->energy){
			tpseam->lastseam = tpseam->last;
            if(t==2){
                stat = -1;
            printf("%d\n",suckmc);
        }
        if(t==2&&stat == 0){
                suckmc = suckmc+1;
            printf("%d\n",suckmc);
        }
			}
    }else{
        tpseam->lastseam = which_min_b ((tpseam->last)->left, tpseam->last);
			if ((tpseam->lastseam)->energy == (tpseam->last)->energy){
			tpseam->lastseam = tpseam->last;
            if(t==2){
                stat = -1;
            printf("%d\n",suckmc);
        }
			}
			if(t==2&&stat == 0){
                suckmc = suckmc-1;
            printf("%d\n",suckmc);
        }
			}
			tpseam = tpseam->lastseam;
            stat = 0;
    }
	return fks;
}

void compute_energy(seam_bundle s){
     int x,y;
     int x0 = s->width;
     int X = s->width_actural, Y = s->height;
    seam *p = s->first_pixel,*q = s->first_pixel;
	q = ((p->right)->down);
	for (x = 1; x < X; x++) {
		for (y = 1; y < Y; y++) {
			q->energy = (((q->last)->col[0]) - (q->col[0])) * (((q->last)->col[0]) - (q->col[0])) + (((q->last)->col[1]) - (q->col[1])) * (((q->last)->col[1]) - (q->col[1])) + (((q->last)->col[2]) - (q->col[2])) * (((q->last)->col[2]) - (q->col[2])) + (((q->left)->col[0]) - (q->col[0])) * (((q->left)->col[0]) - (q->col[0])) + (((q->left)->col[1]) - (q->col[1])) * (((q->left)->col[1]) - (q->col[1])) + (((q->left)->col[2]) - (q->col[2])) * (((q->left)->col[2]) - (q->col[2]));
			if(q->down != NULL){
				q = q->down;
			}
		}
		for (int i = 1; i < Y-1; i++) {
			q = q->last;
		}
		q = q->right;
	}
	//printf("xxxxxxxx\n");
	int i,o;
	for(i =1;i<Y;i++){
            for(o=0;o<X;o++){
                if(((s->first_pixel+X*i+o)->last) == NULL){
            //printf("uckyouddsfsdfa\n");
            //printf("x%d %dy\n",o,i);
        }
            }
        
    }
     //printf("x%d %dyIIIIIIIIIIBLSZ\n",o,i);
    for(i=1;i<Y;i++){
        q=s->first_pixel+x0*i+(x0-X);
        o=0;
        if((q->last) == NULL){
            //printf("uckyou\n");
            //printf("x%d %dy\n",o,i);
            //printf("uckyou\n");
        }
        q->energy += which_min_b(q->last,(q->last)->right)->energy;
        //printf("x%d %dy\n",o,i);
        for(o=1;o<X-1;o++){
          q=s->first_pixel+X*i+o;
          if((q->last) == NULL){
            //printf("uckyou0\n");
           // printf("x%d %dy\n",o,i);
            // printf("uckyou\n");
        }
        if((q->last)->left == NULL){
            //printf("uckyou1\n");
            //printf("x%d %dy\n",o,i);
            //printf("uckyou\n");
        }
        if((q->last)->right == NULL){
            //printf("uckyou2\n");
            //printf("x%d %dy\n",o,i);
            //printf("uckyou\n");
        }
          q->energy += which_min_p(q->last,(q->last)->left,(q->last)->right)->energy;
          //printf("x%d %dy\n",o,i);
          //printf("x%d %dy\n",o,i);
        }
        q=q->right;
        if((q->last)->left == NULL){
            //printf("uckyou\n");
            //printf("x%d %dy\n",o,i);
            //printf("uckyou\n");
        }
        q->energy += which_min_b(q->last,(q->last)->left)->energy;
        //printf("x%d %dy\n",o,i);
    }
	
    /*for(y=1;y<Y;y++){
        q = q->down;
        p = q;
        p->lastseam = which_min_b(p->last,p->last->right);
        p->energy=p->lastseam->energy+p->gra;
        p=p->right;

        for(x=1;x<X-1;x++){
            p->lastseam = which_min_p(p->last->left,p->last,p->last->right);
            p->energy=p->lastseam->energy+p->gra;
            p=p->right;
        }
        p->lastseam = which_min_b(p->last,p->last->left);
        p->energy=p->lastseam->energy+p->gra;
        p=p->right;
    }*/
}
/*void show_seam(seam_bundle s){
    seam *p = find_seam(s);
    while(1){
        p->gra = (luminance) 255;
        if(p->lastseam==NULL) break;
        p=p->lastseam;
    }
}*/
image delete_seam(seam_bundle s){
            int t =0;
			seam* p = find_seam (s,t);
			while (1) {
				if (p->left != NULL) {
					p->left->right = p->right;
                    
				}
				else {
					s->first_pixel = s->first_pixel->right;//the very first pixel of the image (avoid leaking)
				}
				if (p->right != NULL) {
					p->right->left = p->left;
				}
				else {
					s->last_pixel = s->last_pixel->left;//the very last pixel of the image
				}
				if (p->last == NULL) {
					break;
				}
				if (p->lastseam == p->last->right) { p->right->last = p->last; p->last->down = p->right; }
				if (p->lastseam == p->last->left) { p->left->last = p->last; p->last->down = p->left; }
				p->left = NULL;
                p->right =NULL;
				p = p->lastseam;
			}
			s->width_actural--;
            image t01 = debundlize_color(s);
            free_seam_bundle(s);
            return t01;
            
}


image seam_carving(image img,int target_width, char *filter, char *direction, char *mode){
    /*if(!strcmp(direction,"Horizontal")){
        transpose(img);
    }*/
    seam_bundle s = bundlize(img,filter);
    int k;
    for(k=0;k < target_width;k++){
        image tempimg;
        compute_energy(s);
        //printf("%d\n",k);
        //show_seam(s);
        tempimg = delete_seam(s);
        s = bundlize(tempimg,filter);
    }
    image newimg;
    /*if(!strcmp(mode,"Seam")){
        newimg = tocolor(debundlize_gradient_full(s));
    }
    else if(!strcmp(mode,"Energy")){
        int rate = 13;
        newimg = tocolor(debundlize_energy(s,rate));
    }
    else if(!strcmp(mode,"Gradient")){
        newimg = tocolor(debundlize_gradient_full(s));
    }
    else{
        newimg = debundlize_color(s);
        printf("** new width: %d **\n",newimg->width);
    }

    if(!strcmp(direction,"Horizontal")){
        transpose(newimg);
    }*/
	newimg = debundlize_color (s);
    free_seam_bundle(s);
    return newimg;
}
