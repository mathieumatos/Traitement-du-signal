/*------------------------------------------------------*/
/* Prog    : Tp3_IFT3205-2-1b.c                          */
/* Auteur  : Dereck McDuff, Mathieu Matos               */
/* Date    : 03/03/2019                                 */
/* version : Emeraude                                   */ 
/* langage : C                                          */
/* labo    : DIRO                                       */
/*------------------------------------------------------*/

/*------------------------------------------------*/
/* FICHIERS INCLUS -------------------------------*/
/*------------------------------------------------*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "FonctionDemo3.h"

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/   
/*------------------------------------------------*/
#define NAME_VISUALISER "display "
#define NAME_IMG_IN  "lena128"
#define NAME_IMG_OUT "image-TpIFT3205-2-1b"

/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/   
/*------------------------------------------------*/

void matrix_resize_ppvoisin(float** src, float** dest, int length1, int width1, int length2, int width2) {
    
    float ratio_l = length2/(float)length1;
    float ratio_w = width2/(float)width1;

    for(int i=0; i<length2; i++) {
      for(int j=0; j<width2; j++) {
        int new_x = floor(j/ratio_w);
        int new_y = floor(i/ratio_l);
        dest[i][j] = src[new_y][new_x];
      }
    }
}

float matrix_value_association(float** m, int j, int i, int length, int width) {

    if(i < 0 || i >= length) {
      i = (i + length) % length;
    }

    if(j < 0 || j >= width) {
      j = (j + width) % width;
    }

    return m[i][j];
}

void matrix_resize_bili(float **src, float **dest, int length1, int width1, int length2, int width2) {
  
  float ratio_l = length2/(float)length1; 
  float ratio_w = width2/(float)width1;

  for (int i = 0; i < length2; i++) {
    for (int j = 0; j < width2; j++) {

      float xp = j / ratio_w;
      float yp = i / ratio_l;

      int x1 = (int)xp;
      int y1 = (int)yp;

      float dx = xp - x1;
      float dy = yp - y1;

      float f_xy = matrix_value_association(src, x1, y1, length1, width1);
      float f_x1y = matrix_value_association(src, x1 + 1, y1, length1, width1);
      float f_xy1 = matrix_value_association(src, x1, y1 + 1, length1, width1);
      float f_x1y1 = matrix_value_association(src, x1 + 1, y1 + 1, length1, width1);
      float f_xpy = f_xy + dx * (f_x1y - f_xy);
      float f_xpy1 = f_xy1 + dx * (f_x1y1 - f_xy1);

      dest[i][j] = f_xpy + dy * (f_xpy1 - f_xpy);
    }
  }
}

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
 {
  int length,width;
  char BufSystVisuImg[100];

  //Lecture Image 
  float** originalImage=LoadImagePgm(NAME_IMG_IN,&length,&width);
  float** targetImage=fmatrix_allocate_2d(length*4,width*4);

  //Interpolation 
  matrix_resize_bili(originalImage,targetImage,length,width,length*4,width*4);

  //Sauvegarde
  SaveImagePgm(NAME_IMG_OUT,targetImage,length*4,width*4);

  //Commande systeme: VISU
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT);
  strcat(BufSystVisuImg,".pgm&");
  printf(" %s",BufSystVisuImg);
  system(BufSystVisuImg);

  //==End=========================================================

  //Liberation memoire 
  free_fmatrix_2d(originalImage);
  free_fmatrix_2d(targetImage);  
 
  //retour sans probleme
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}


