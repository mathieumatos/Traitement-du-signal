/*------------------------------------------------------*/
/* Prog    : Tp3_IFT3205-2-1a.c                          */
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
#define NAME_IMG_OUT "image-TpIFT3205-2-1a"

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

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
 {
  int i,j;
  int length,width;
  char BufSystVisuImg[100];

  //Lecture Image 
  float** originalImage=LoadImagePgm(NAME_IMG_IN,&length,&width);
  float** targetImage=fmatrix_allocate_2d(length*4,width*4);

  //Interpolation 
  matrix_resize_ppvoisin(originalImage,targetImage,length,width,length*4,width*4);

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


