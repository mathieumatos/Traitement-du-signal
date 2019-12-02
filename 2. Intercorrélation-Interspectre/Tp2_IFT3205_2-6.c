/*------------------------------------------------------*/
/* Prog    : Tp2_IFT3205-2-5.c                          */
/* Auteur  :                                            */
/* Date    : --/--/2010                                 */
/* version :                                            */ 
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
#define NAME_IMG_OUT1 "image-TpIFT3205-2-6"

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
      if((x+y) % 2 == 1){
        m[x][y] *= pow(-1, (x + y));
      }
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

double ressemblanceErr(float** s, float** d,int length,int width) {
  double err = 0;
  for(int x=0; x<length; x++) {
    for(int y=0; y<width; y++) {
      err += fabsf(s[x][y]-d[x][y]);
    }
  }
  return err;
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

float findOptimalAngle(float** m, float** t, int length, int width, float minA, float maxA, float delta) {

  float** resultingMatrice=fmatrix_allocate_2d(length,width);

  float optAngle = 0;
  double optErr = 0;

  for (float angle = minA; angle <= maxA; angle += delta) {
    nnRotation(m,resultingMatrice,length,width,angle);
    double newErr = ressemblanceErr(resultingMatrice,t,length,width);
    fprintf(stderr, "[%f :: %f]\t",angle,newErr);
    if (optErr == 0 || optErr > newErr) {
      optErr = newErr;
      optAngle = angle;
    }
  }
  free_fmatrix_2d(resultingMatrice);
  printf("\n\roptimal [angle,err] = [%f :: %f]",optAngle,optErr);
  return optAngle;

}

void makeBabyMatrice(float** a, float** b, int length, int width) {
  for (int i=0; i < length; i++) {
    for (int j=0; j < width; j++) {
      b[i][j] = a[i][j];
    }
  }
}


float** recalerImages(float** image1, float** image2, int transX, int transY, int len, int wid) {

    int doubleLength = len*2, doubleWidth = wid*2;
    float** matrice = fmatrix_allocate_2d(doubleLength, doubleWidth);

    for(int i=0; i<len; i++) 
        for(int j=0; j<wid; j++) {

            matrice[doubleLength/2 - len/2 + i][doubleWidth/2 - wid/2 + j] = image1[i][j];
        }
    
    for(int i=0; i<len; i++) 
        for(int j=0; j<wid; j++) {
            if(image2[i][j] < 1)

                continue;
            
            matrice[doubleLength/2 - len/2 + i + transY][doubleWidth/2 - wid/2 + j + transX] = image2[i][j];
        }
    return matrice;
}

/*------------------------------------------------*/
/* ET MAINTENANT, NOTRE PROGRAMME PRINCIPAL   ----*/
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
  
  float** MatriceImgI1=fmatrix_allocate_2d(length,width);
  float** MatriceImgI2=fmatrix_allocate_2d(length,width);
  float** MatriceImgM1=fmatrix_allocate_2d(length,width);
  float** MatriceImgM2=fmatrix_allocate_2d(length,width);
  float** MatriceImg4=fmatrix_allocate_2d(length,width);
  float** MatriceImgI4=fmatrix_allocate_2d(length,width);

// ----------------

  float** MatriceImgFINAL = fmatrix_allocate_2d(length,width);
  float** MatriceImgIFINAL = fmatrix_allocate_2d(length,width);

  float** MatriceImg5=fmatrix_allocate_2d(length,width);

  float** MatriceImgM4 = fmatrix_allocate_2d(length,width);




  float** MatriceImgI5=fmatrix_allocate_2d(length,width);
// ----------------

  float** MatriceImg3=fmatrix_allocate_2d(length,width);
  float** MatriceImgI3=fmatrix_allocate_2d(length,width);

  //Lecture Image 
  float** MatriceImg1=LoadImagePgm(NAME_IMG_IN1,&length,&width);
  float** MatriceImg2=LoadImagePgm(NAME_IMG_IN2,&length,&width);

  TranslImg(MatriceImg1,length,width);
  TranslImg(MatriceImg2,length,width);

  FFTDD(MatriceImg1,MatriceImgI1,length,width);
  FFTDD(MatriceImg2,MatriceImgI2,length,width);

  Mod(MatriceImgM1,MatriceImg1,MatriceImgI1,length,width);
  Mod(MatriceImgM2,MatriceImg2,MatriceImgI2,length,width);

  float optimalAngle = findOptimalAngle(MatriceImg1,MatriceImg2,length,width,-(M_PI/16),(M_PI/16),0.005);

  IFFTDD(MatriceImg2, MatriceImgI2, length, width);
  TranslImg(MatriceImg2, length, width);
  
  //bilinearRotation(MatriceImg2, MatriceImg3, length, width, -optimalAngle);
  nnRotation(MatriceImg2,MatriceImg3,length,width,-optimalAngle);


  makeBabyMatrice(MatriceImg3,MatriceImg4,length,width);
  makeBabyMatrice(MatriceImgI3,MatriceImgI4,length,width);  

  
  for(i=0; i<length; i++) {
      for(j=0; j<width/2; j++) {
          float temp = MatriceImg3[i][j];
          MatriceImg3[i][j] = MatriceImg3[i][width-j-1];
          MatriceImg3[i][width-j-1] = temp;
      }
  }

  for(i=0; i<length/2; i++) {
      for(j=0; j<width; j++) {
          float temp = MatriceImg3[i][j];
          MatriceImg3[i][j] = MatriceImg3[length-i-1][j];
          MatriceImg3[length-i-1][j] = temp;
      }
  }
  
  


  
  
  TranslImg(MatriceImg3, length, width);
  TranslImg(MatriceImg4, length, width);

  FFTDD(MatriceImg4, MatriceImgI4, length, width);
  Mod(MatriceImgM4,MatriceImg4,MatriceImgI4,length,width);
  
  FFTDD(MatriceImg3, MatriceImgI3, length, width);
  
  MultMatrix(MatriceImg5, MatriceImgI5, MatriceImg1, MatriceImgI1, MatriceImg3, MatriceImgI3, length, width);
  
  //Matrice Finale à partir de M4 et 5
  for(i=0; i<length; i++) {
    for(j=0; j<width; j++) {
        MatriceImgFINAL[i][j] = MatriceImg5[i][j] / (MatriceImgM4[i][j]*MatriceImgM4[i][j]);
        MatriceImgIFINAL[i][j] = MatriceImgI5[i][j] / (MatriceImgM4[i][j]*MatriceImgM4[i][j]);
    }
  }
  
  IFFTDD(MatriceImgFINAL, MatriceImgIFINAL, length, width);
  TranslImg(MatriceImgFINAL, length, width);

  IFFTDD(MatriceImgFINAL, MatriceImgIFINAL, length, width);
  TranslImg(MatriceImgFINAL, length, width);
  TranslImg(MatriceImgIFINAL, length, width);
  FFTDD(MatriceImgFINAL, MatriceImgIFINAL, length, width);
  

  //trouver trans
  float max = -INFINITY;
  int w = 0;
  int l = 0;

  for(i=0; i<length; i++) {
    for(j=0; j<width; j++) {
        if(max < MatriceImgFINAL[i][j]) {
            max = MatriceImgFINAL[i][j];
            w = i - length/2;
            l = j - width/2;
        }
    }
  }
  
  IFFTDD(MatriceImg4, MatriceImgI4, length, width);
  TranslImg(MatriceImg4, length, width);

  IFFTDD(MatriceImg1, MatriceImgI1, length, width);
  TranslImg(MatriceImg1, length, width);

  float** MatriceImgPANO = recalerImages(MatriceImg1, MatriceImg4, l, w, length, width);

  //Sauvegarde
  SaveImagePgm(NAME_IMG_OUT1,MatriceImgPANO,length*2,width*2);

  //Commande systeme: VISU
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT1);
  strcat(BufSystVisuImg,".pgm&");
  printf(" %s",BufSystVisuImg);
  system(BufSystVisuImg);


  //==End=========================================================

  //Liberation memoire 
  free_fmatrix_2d(MatriceImg1);
  free_fmatrix_2d(MatriceImg2);
  free_fmatrix_2d(MatriceImg3);
  free_fmatrix_2d(MatriceImg4);
  free_fmatrix_2d(MatriceImg5);
  free_fmatrix_2d(MatriceImgFINAL);
  free_fmatrix_2d(MatriceImgI1);
  free_fmatrix_2d(MatriceImgI2);
  free_fmatrix_2d(MatriceImgI3);
  free_fmatrix_2d(MatriceImgI5);
  free_fmatrix_2d(MatriceImgM1);    
  free_fmatrix_2d(MatriceImgM2);
  free_fmatrix_2d(MatriceImgM4);
  free_fmatrix_2d(MatriceImgPANO);




  //retour sans probleme
  printf("\n C'est fini ... \n\n");
  return 0;
}
