/*------------------------------------------------------*/
/* Prog    : Tp2_IFT3205-2-2.c                          */
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
#define NAME_IMG_IN1  "lena"
#define NAME_IMG_OUT1 "image-TpIFT3205-2-2"

/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/   
/*------------------------------------------------*/

float RadToDeg(float rad) { 
  return rad * 180.0 / M_PI;
}

float DegToRad(float deg) {
  return deg * M_PI / 180.0;
}

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

void nnRotation(float** s, float** d, int length, int width, float angle) {
  int nextX, nextY, roundedNextX, roundedNextY;
  for(int x=0; x<length; x++) {
    for(int y=0; y<width; y++) {
      nextX =  (x-width/2)*cos(angle) - (y-length/2)*sin(angle) + width/2;
      nextY =  (x-width/2)*sin(angle) + (y-length/2)*cos(angle) + length/2;
      roundedNextX = round(nextX);
      roundedNextY = round(nextY);
      if (roundedNextX < 0 || roundedNextX >= length || roundedNextY < 0 || roundedNextY >= length) {
        d[x][y] = 0.0;
      } else {
        d[x][y] = s[roundedNextX][roundedNextY];
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
  float Theta0;
  int x0,y0;
  char BufSystVisuImg[100];

  //Constante
  length=512;
  width=512;
  
  //Allocation Memoire 
  float** MatriceImgDest=fmatrix_allocate_2d(length,width);

  //Lecture Image 
  float** MatriceImgSource=LoadImagePgm(NAME_IMG_IN1,&length,&width);

  float angle = DegToRad(22.5f);
  nnRotation(MatriceImgSource,MatriceImgDest,length,width,angle);

  // .... .... .... .... .... .... ....


  //Sauvegarde
  SaveImagePgm(NAME_IMG_OUT1,MatriceImgDest,length,width);

  //Commande systeme: VISU
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT1);
  strcat(BufSystVisuImg,".pgm&");
  printf(" %s",BufSystVisuImg);
  system(BufSystVisuImg);


  //==End=========================================================

  //Liberation memoire 
  free_fmatrix_2d(MatriceImgDest);
  free_fmatrix_2d(MatriceImgSource);

  //retour sans probleme
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}


