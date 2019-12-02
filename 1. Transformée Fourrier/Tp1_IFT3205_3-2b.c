
/*------------------------------------------------------*/
/* Prog    : Tp1_IFT3205-3-2b.c                         */
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
#define NAME_IMG_IN  "Monrstein"
#define NAME_IMG_OUT "image-Tp1_IFT3205-3-2b"

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

void suppressHarm(float** m, int length, int width, int value) {
  for(int x=length/2-value; x<=length/2+value;  x++) {
    for(int y=width/2-value; y<=width/2+value; y++) {
      m[x][y] = 0.0;
    }
  }
}

void deleteFarHarm(float** m, int length, int width, int value) {
  for(int x=0; x<length; x++) {
    for(int y=0; y<width; y++) {
      if (sqrt((x - length/2) * (x - length/2) + (y - width/2) * (y - width/2)) > value) {
        m[x][y]=0.0;
      }
    }
  }
}

void mulByDistToCenter(float** m, int length, int width) {
  for(int x=0; x<length; x++) {
    for(int y=0; y<width; y++) {
      m[x][y] *= sqrt((x - length/2) * (x - length/2) + (y - width/2) * (y - width/2));
    }
  }
}

void deleteOnYFreqAxis(float** m, int length, int width, int size) {
  for(int x=0; x<length; x++) {
    for(int y=0; y<width; y++) {
      if (y >= width/2+size || y <= width/2-size) {
        m[x][y] = 0.0;
      }
    }
  }
}

void deleteOnXFreqAxis(float** m, int length, int width, int size) {
  for(int x=0; x<length; x++) {
    for(int y=0; y<width; y++) {
      if (x >= length/2+size || x <= length/2-size) {
        m[x][y] = 0.0;
      }
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
  MatriceImgR=LoadImagePgm(NAME_IMG_IN,&length,&width);
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

  suppressHarm(MatriceImgR, length, width, 15);
  suppressHarm(MatriceImgI, length, width, 15);


  IFFTDD(MatriceImgR, MatriceImgI, length, width);

  CenterImg(MatriceImgR, length, width);

  /*Pour visu*/
  Recal(MatriceImgR, length, width);

  /*Sauvegarde de MatriceImgM sous forme d'image pgm*/
  SaveImagePgm(NAME_IMG_OUT, MatriceImgR, length, width);

  /*Liberation memoire pour les matrices*/
  free_fmatrix_2d(MatriceImgR);
  free_fmatrix_2d(MatriceImgI);
  free_fmatrix_2d(MatriceImgM);

  /*Commande systeme: visualisation de Ingout.pgm*/
  strcpy(BufSystVisuImg, NAME_VISUALISER);
  strcat(BufSystVisuImg, NAME_IMG_OUT);
  strcat(BufSystVisuImg, ".pgm&");
  printf(" %s", BufSystVisuImg);
  system(BufSystVisuImg);

  /*retour sans probleme*/
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}

