/*------------------------------------------------------*/
/* Prog    : Tp2_IFT3205-2-1.c                          */
/* Auteur  : Dereck McDuff & Mathieu Matos              */
/* Date    : 16/02/2010                                 */
/* version : Officielle                                 */ 
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

#include "FonctionDemo2.h"

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/   
/*------------------------------------------------*/
#define NAME_VISUALISER "display "
#define NAME_IMG_IN1  "UdM_1"
#define NAME_IMG_IN2  "UdM_2"
#define NAME_IMG_OUT1 "image-TpIFT3205-2-1a"
#define NAME_IMG_OUT2 "image-TpIFT3205-2-1b"

/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/   
/*------------------------------------------------*/

void TranslImg(float **m, int length, int width)
{
  for (int x = 0; x < length; x++)
  {
    for (int y = 0; y < width; y++)
    {
      m[x][y] *= pow(-1, (x + y));
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

void bilinearRotation(float** s, float** d, int length, int width, float angle) {
  int x1, x2, y1, y2;
  float deltaX, deltaY, xPrime, yPrime, fxy, fx1y, fxy1, fx1y1, derx;

  for(y2 = 0; y2 < length; y2++){
    for(x2 = 0; x2 < width; x2++){
      yPrime = -(x2 - width/2) * sin(-angle) + (y2 - length/2) * cos(-angle) + length/2;            
      y1 = floor(yPrime);

      xPrime = (x2 - width/2) * cos(-angle) + (y2 - length/2) * sin(-angle) + width/2;
      x1 = floor(xPrime);

      //margins
      if(x1 < 0 || y1 < 0 || x1 > width || y1 > length){
        d[y2][x2] = 0;

        continue;
      }

      deltaX = xPrime - x1;
      deltaY = yPrime - y1;
      fxy = checkBounds(s, x1, y1, length, width);
      fx1y = checkBounds(s, x1 + 1, y1, length, width);
      fxy1 = checkBounds(s, x1, y1 + 1, length, width);
      fx1y1 = checkBounds(s, x1 + 1, y1 + 1, length, width);
      derx = fxy + deltaX * (fx1y - fxy);


      d[y2][x2] = derx + deltaY * ((fxy1 + deltaX * (fx1y1 - fxy1)) - derx);

    }
  } 
}

int checkBounds(float** matrix, int x, int y, int length, int width){
  if(x > length)  x = x - length;
  if(y > width)   y = y - width;
  if(x < 0)       x = length - x;
  if(y < 0)       y = width - y;

  return matrix[x][y];
}

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
 {
  int i,j,k;
  int length,width;
  float Theta0;
  int x0,y0;
  char BufSystVisuImg[100];

  //Constante
  length=512;
  width=512;
  
  //Allocation Memoire 
  float** MatriceImgI1=fmatrix_allocate_2d(length,width);
  float** MatriceImgM1=fmatrix_allocate_2d(length,width);
  float** MatriceImgR1=fmatrix_allocate_2d(length,width);
  float** MatriceImgI2=fmatrix_allocate_2d(length,width);
  float** MatriceImgM2=fmatrix_allocate_2d(length,width);
  float** MatriceImgR2=fmatrix_allocate_2d(length,width);
  float** MatriceImgI3=fmatrix_allocate_2d(length,width);
  float** MatriceImgM3=fmatrix_allocate_2d(length,width);
  float** MatriceImgR3=fmatrix_allocate_2d(length,width);
  float** MatriceImg3=fmatrix_allocate_2d(length,width);

  //Lecture Image 
  float** MatriceImg1=LoadImagePgm(NAME_IMG_IN1,&length,&width);
  float** MatriceImg2=LoadImagePgm(NAME_IMG_IN2,&length,&width);

  TranslImg(MatriceImg1, length, width);
  TranslImg(MatriceImg2, length, width);

  FFTDD(MatriceImg1, MatriceImgI1, length, width);
  FFTDD(MatriceImg2, MatriceImgI2, length, width);

  Mod(MatriceImgM1, MatriceImg1, MatriceImgI1, length, width);
  Mod(MatriceImgM2, MatriceImg2, MatriceImgI2, length, width);

  LogImg(MatriceImgM1,length,width);
  LogImg(MatriceImgM2,length,width);

  Recal(MatriceImgM1,length,width);
  Recal(MatriceImgM2,length,width);

  // .... .... .... .... .... .... ....


  //Sauvegarde
  SaveImagePgm(NAME_IMG_OUT1,MatriceImgM1,length,width);
  SaveImagePgm(NAME_IMG_OUT2,MatriceImgM2,length,width);

  //Commande systeme: VISU
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT1);
  strcat(BufSystVisuImg,".pgm&");
  printf(" %s",BufSystVisuImg);
  system(BufSystVisuImg);
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT2);
  strcat(BufSystVisuImg,".pgm&");
  printf(" %s",BufSystVisuImg);
  system(BufSystVisuImg);


  //==End=========================================================

  //Liberation memoire 
  free_fmatrix_2d(MatriceImgR1);
  free_fmatrix_2d(MatriceImgI1);
  free_fmatrix_2d(MatriceImgM1);
  free_fmatrix_2d(MatriceImgR2);
  free_fmatrix_2d(MatriceImgI2);
  free_fmatrix_2d(MatriceImgM2);
  free_fmatrix_2d(MatriceImgR3);
  free_fmatrix_2d(MatriceImgI3);
  free_fmatrix_2d(MatriceImgM3);
  free_fmatrix_2d(MatriceImg1);
  free_fmatrix_2d(MatriceImg2);  
  free_fmatrix_2d(MatriceImg3);

  //retour sans probleme
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}


