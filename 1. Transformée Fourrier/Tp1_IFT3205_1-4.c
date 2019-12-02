
/*------------------------------------------------------*/
/* Prog    : Tp1_IFT3205-1-4.c                          */
/* Auteur  : Dereck McDuff & Mathieu Matos              */
/* Date    : 12/02/2019                                 */
/* version : the best                                   */ 
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

#include "FonctionDemo1.h"

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/   
/*------------------------------------------------*/
#define NAME_VISUALISER "display "
#define NAME_IMG_OUT "image-Tp1_IFT3205-1-4c"

char* names_img_in[] = {
  "D1r", "D11r", "D46r"
};

/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/
/*------------------------------------------------*/
void CenterImg(float** ,int, int);

void LogImg();

/*------------------------------------------------*/
/* IMPLEMENTATION DE FONCTIONS  ------------------*/   
/*------------------------------------------------*/
void CenterImg(float** m,int length, int width) {
  for(int x=0; x<length; x++) {
    for(int y=0; y<width; y++) {
      m[x][y] *= pow(-1, (x+y));
    }
  }
}

void LogImg(float** m,int length, int width) {
  for(int x=0; x<length; x++) {
    for(int y=0; y<width; y++) {
      m[x][y] = log(1+m[x][y]);
    }
  }
}

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
 {
  int i,j,k;
  int length,width;
  char BufSystVisuImg[100];
  float** MatriceImgR;
  float** MatriceImgI;
  float** MatriceImgM;

  /*Allocation memoire pour la matrice image*/
  MatriceImgR=LoadImagePgm(names_img_in[2],&length,&width);
  MatriceImgI=fmatrix_allocate_2d(length,width);
  MatriceImgM=fmatrix_allocate_2d(length,width);

  /*Initialisation a zero de toutes les matrices */
  for(i=0;i<length;i++) {
    for(j=0;j<width;j++) 
      {
        MatriceImgI[i][j]=0.0;
        MatriceImgM[i][j]=0.0;
      }
  } 
  
  /* Centrer la chose */
  CenterImg(MatriceImgR,length,width);

  /*FFT*/
  FFTDD(MatriceImgR,MatriceImgI,length,width);

  /*Module*/
  Mod(MatriceImgM,MatriceImgR,MatriceImgI,length,width);
  
  /*Pour visu*/
  LogImg(MatriceImgM,length,width);
  Recal(MatriceImgM,length,width);              
  
  /*Sauvegarde de MatriceImgM sous forme d'image pgm*/
  SaveImagePgm(NAME_IMG_OUT,MatriceImgM,length,width);

  /*Liberation memoire pour les matrices*/
  free_fmatrix_2d(MatriceImgR);
  free_fmatrix_2d(MatriceImgI); 
  free_fmatrix_2d(MatriceImgM);

  /*Commande systeme: visualisation de Ingout.pgm*/
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT);
  strcat(BufSystVisuImg,".pgm&");
  printf(" %s",BufSystVisuImg);
  system(BufSystVisuImg);

  /*retour sans probleme*/ 
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}


