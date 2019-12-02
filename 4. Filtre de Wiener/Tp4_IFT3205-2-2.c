//------------------------------------------------------
// Prog    : Tp4_IFT3205                          
// Auteur  : Dereck McDuff & Mathieu Matos                               
// Date    :                                  
// version : Saphire                                            
// langage : C                                          
// labo    : DIRO                                       
//------------------------------------------------------

//------------------------------------------------
// FICHIERS INCLUS -------------------------------
//------------------------------------------------
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "FonctionDemo4.h"

//------------------------------------------------
// DEFINITIONS -----------------------------------   
//------------------------------------------------
#define NAME_VISUALISER "display "
#define NAME_IMG_IN  "cameraman"
#define NAME_IMG_OUT1 "image-TpIFT3205-2-2"
#define NAME_IMG_OUT2 "cameraman_degraded"

float** mat;
float** mat_rest;
float** mat_rest;
float** m_img_m;
float** m_img_r;
float** m_img_i;
float** temp_m;
float** temp_r;
float** temp_i;
float** temp2_r;
float** temp2_i;


// float** mat_tmp0;
// float** mat_tmp1;
// float** mat_tmp2;
// float** mat_tmp3;
// float** mat_tmp4;
// float** mat_tmp5;
// float** mat_tmp6;
// float** mat_tmp7;
// float** mat_tmp8;
// float** mat_tmp9;
// float** mat_tmp10;
// float** mat_tmp11;
// float** mat_tmp12;

//------------------------------------------------
// PROTOTYPE DE FONCTIONS  -----------------------   
//------------------------------------------------

//>Taille Image
int length=256;
int width=256;
int size_image=256;

//>Parametre de degradation
int size=9;
float variance_noise=0.5;
int psf=1;

//>Nb d'Iterations
int nbiter=20;

//>ImprovmentSNR
float isnr;

//------------------------------------------------
//------------------------------------------------
// FONCTIONS  ------------------------------------   
//------------------------------------------------
//------------------------------------------------

void filter_that(float** m, int lenght, int width) {
  for (int i=0; i < length; i++) {
    for (int j=0; j < width; j++) {
      if((i < size/2.0 || i >= length - size/2.0)  && 
      (j < size/2.0 || j >= width - size/2.0)) {
        m[i][j] = (length * width)/((float)size * size);
      } else {
        m[i][j] = 0.0;
      }
    }
  }
}

void zero_that(float** m, int lenght, int width) {
  for (int i=0; i < length; i++) {
    for (int j=0; j < width; j++) {
      m[i][j] = 0;
    }
  }
}

float passe_carreau(float** m1, float** m2, int length, int width) {
  float total = 0.0;
  for (int i=0; i < length; i++) {
    for (int j=0; j < width; j++) {
      float diff = m1[i][j] - m2[i][j];
      total += diff*diff;
    }
  }
  return total;
}

void ModSquare(float** m,float** mr,float** mi,int length,int width) {
 for(int i=0;i<length;i++) {
    for(int j=0;j<width;j++) {
      m[i][j] = (mr[i][j]*mr[i][j])+(mi[i][j]*mi[i][j]);
    }
  }
}

void MultiplyMatrices(float** m1r,float** m1i, float** m2r,float** m2i, int length,int width) {
  for(int i=0;i<length;i++) {
    for(int j=0;j<width;j++) {
      float r1 = m1r[i][j], r2 = m2r[i][j];
      float i1 = m1i[i][j], i2 = m2i[i][j];
      m1r[i][j] = r1 * r2 - i1 * i2;
      m1i[i][j] = r1 * i2 + r2 * i1;
    }
  }
}

void copy_that(float** m1, float** m2, int length, int width) {
  for(int i=0; i<length; i++) {
    for(int j=0; j<width; j++) {
      m1[i][j] = m2[i][j];
    }
  }
}

//---------------------------------------------------------
//---------------------------------------------------------
// PROGRAMME PRINCIPAL   ----------------------------------                     
//---------------------------------------------------------
//---------------------------------------------------------
int main(int argc,char **argv)
 {
  int i,j,k;
  int lgth,wdth;
  char BufSystVisuImg[100];

  //Allocation memoire matrice
  mat=fmatrix_allocate_2d(length,width);
  mat_rest=fmatrix_allocate_2d(length,width);
  mat_rest = fmatrix_allocate_2d(length, width);
  m_img_m = fmatrix_allocate_2d(length, width);
  m_img_r = fmatrix_allocate_2d(length, width);
  m_img_i = fmatrix_allocate_2d(length, width);
  temp_m = fmatrix_allocate_2d(length, width);
  temp_r = fmatrix_allocate_2d(length,width);
  temp_i = fmatrix_allocate_2d(length,width);
  temp2_r = fmatrix_allocate_2d(length, width);
  temp2_i = fmatrix_allocate_2d(length, width);
  
  //=========================================================
  //== PROG =================================================
  //=========================================================
  
  float** mat_img_orig = LoadImagePgm(NAME_IMG_IN,&lgth,&wdth);
  
  degradation(mat_img_orig, mat_rest, length, width, psf, size, variance_noise);

  zero_that(temp_i,length,width);
  zero_that(m_img_i,length,width);
  filter_that(temp_r,length,width);
  copy_that(m_img_r,mat_rest, length,width);
      
  FFTDD(mat_rest, m_img_i, length, width);
  FFTDD(temp_r, temp_i, length, width);


  float isnr;
  
  ModSquare(m_img_m, mat_rest, m_img_i, length, width);
  ModSquare(temp_m, temp_r, temp_i, length, width);

  for(i=0; i<length; i++) {
    for(j=0; j<width; j++) {
      float denom = temp_m[i][j] + (variance_noise/(length*width)) / m_img_m[i][j];
      temp2_r[i][j] = temp_r[i][j] / denom;
      temp2_i[i][j] = -temp_i[i][j] / denom;
    }
  }
  
  MultiplyMatrices(mat_rest, m_img_i, temp2_r, temp2_i, length, width);
  IFFTDD(mat_rest, m_img_i, length, width);
  
  for(i=0; i<length; i++) {
    for(j=0; j<width; j++) {
        mat_rest[i][j] = fmax(fmin(mat_rest[i][j], 255), 0);
    }
  }
  float num = passe_carreau(mat_img_orig, m_img_r, length, width);
  float den = passe_carreau(mat_img_orig, mat_rest, length, width);
  isnr = 10 * log10( num / den);

  printf("\n\n==========\n%02d - ISNR : %lf\n==========\n", 0, isnr);
      
    //---------------------------------------------
  // SAUVEGARDE et VISU
  // -------------------
  // Le resultat de la restoration > mat_rest
  // L'image d�grad�e              > m_img_r
  // L'image non d�grad�e          > mat_img_orig
  //----------------------------------------------
  SaveImagePgm(NAME_IMG_OUT1,mat_rest,length,width);
  SaveImagePgm(NAME_IMG_OUT2,m_img_r,length,width);
  
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT2);
  strcat(BufSystVisuImg,".pgm&");
  printf("\n > %s",BufSystVisuImg);
  system(BufSystVisuImg);
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_IN);
  strcat(BufSystVisuImg,".pgm&");
  printf("\n > %s",BufSystVisuImg);
  system(BufSystVisuImg);
  strcpy(BufSystVisuImg,NAME_VISUALISER);
  strcat(BufSystVisuImg,NAME_IMG_OUT1);
  strcat(BufSystVisuImg,".pgm&");
  printf("\n > %s",BufSystVisuImg);
  system(BufSystVisuImg);
  
  //retour sans probleme 
  printf("\n C'est fini ... \n\n");
  return 0;	
}


