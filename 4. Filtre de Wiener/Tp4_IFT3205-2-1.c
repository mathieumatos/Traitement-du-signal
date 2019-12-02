//------------------------------------------------------
// Prog    : Tp4_IFT3205                          
// Auteur  :                                            
// Date    :                                  
// version :                                             
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
#define NAME_IMG_OUT1 "cameraman_restored"
#define NAME_IMG_OUT2 "cameraman_degraded"

//------------------------------------------------
// PROTOTYPE DE FONCTIONS  -----------------------   
//------------------------------------------------

//------------------------------------------------
// VARIABLES GLOBALES  ---------------------------   
//------------------------------------------------

// float** mat_img;
// float** mat_img_orig;
// float** mat_rest;
// float** mat_rest_prec;
// float** mat_rest_best;
// float** mat_psf;

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

//---------------------------------------------------------
//---------------------------------------------------------
// PROGRAMME PRINCIPAL   ----------------------------------                     
//---------------------------------------------------------
//---------------------------------------------------------
int main(int argc,char **argv)
 {
  int i,j,k;
  int length,width;
  char BufSystVisuImg[100];

  //Allocation memoire matrices
  float** mat_img=fmatrix_allocate_2d(length,width);
  float** mat_rest=fmatrix_allocate_2d(length,width);
  // mat_rest_prec=fmatrix_allocate_2d(length,width); 
  // mat_rest_best=fmatrix_allocate_2d(length,width); 
  // mat_psf=fmatrix_allocate_2d(length,width);

  // mat_tmp0=fmatrix_allocate_2d(length,width);
  // mat_tmp1=fmatrix_allocate_2d(length,width);
  // mat_tmp2=fmatrix_allocate_2d(length,width);
  // mat_tmp3=fmatrix_allocate_2d(length,width);
  // mat_tmp4=fmatrix_allocate_2d(length,width);
  // mat_tmp5=fmatrix_allocate_2d(length,width);
  // mat_tmp6=fmatrix_allocate_2d(length,width);
  // mat_tmp7=fmatrix_allocate_2d(length,width);
  // mat_tmp8=fmatrix_allocate_2d(length,width);
  // mat_tmp9=fmatrix_allocate_2d(length,width);
  // mat_tmp10=fmatrix_allocate_2d(length,width);
  // mat_tmp11=fmatrix_allocate_2d(length,width);
  // mat_tmp12=fmatrix_allocate_2d(length,width);
 
  //=========================================================
  //== PROG =================================================
  //=========================================================

  //>Lecture Image
  float** mat_img_orig=fmatrix_allocate_2d(length,width);
  mat_img_orig=LoadImagePgm(NAME_IMG_IN,&length,&width);

  //--------------------------------------------------------
  //>Degradation
  //--------------------------------------------------------
  // Cette fonction ajoute un flou cr�� par une psf uniforme 
  // (fonction porte) de taille sizexsize puis ajoute sur
  // cette image rendue floue, un bruit Gaussien de variance
  // variance_noise
  //
  // Entr�e : mat_img_orig :: image non d�grad�e
  // Sortie : mat_img     :: image d�grad�e
  //--------------------------------------------------------
  degradation(mat_img_orig,mat_img,length,width,psf,size,variance_noise);

  float** mat_img_degraded=fmatrix_allocate_2d(length,width);

  for (i=0;i<length;i++) {
    for(j=0;j<width;j++) {
      mat_img_degraded[i][j] = mat_img[i][j];
    }
  }

  float** hr=fmatrix_allocate_2d(length,width);
  float** hi=fmatrix_allocate_2d(length,width);
  float** ma_img_i=fmatrix_allocate_2d(length,width);

  filter_that(hr, length, width);
  zero_that(hi, length, width);
  zero_that(ma_img_i, length, width);

  FFTDD(mat_img,ma_img_i,length,width);
  FFTDD(hr,hi,length,width);

  //============
  //WIENER
  //============

  float** mat_img_m = fmatrix_allocate_2d(length, width);
  float** hm = fmatrix_allocate_2d(length, width);

  float** wr = fmatrix_allocate_2d(length, width);
  float** wi = fmatrix_allocate_2d(length, width);

  ModSquare(mat_img_m, mat_img, ma_img_i, length, width);
  ModSquare(hm, hr, hi, length, width);

  for (i=0; i<length; i++) {
    for (j=0; j<width; j++) {
      float denom = hm[i][j] + (variance_noise/(length*width)) / mat_img_m[i][j];
      wr[i][j] = hr[i][j] / denom;
      wi[i][j] = -hi[i][j] / denom;
    }
  }
  
  MultiplyMatrices(mat_img, ma_img_i, wr, wi,length,width);

  IFFTDD(mat_img, ma_img_i, length, width);

  for (i=0; i<length; i++) {
    for (j=0; j<width; j++) {
      mat_img[i][j] = fmax(fmin(mat_img[i][j],255),0);
    }
  }
  
  isnr = 10 * log10(passe_carreau(mat_img_orig, mat_img_degraded, length, width) / passe_carreau(mat_img_orig, mat_img, length, width));

  printf("\n\n==========\n%02d - ISNR : %lf\n==========\n", 0, isnr);

  //---------------------------------------------
  // SAUVEGARDE et VISU
  // -------------------
  // Le resultat de la restoration > mat_rest
  // L'image d�grad�e              > mat_img
  // L'image non d�grad�e          > mat_img_orig
  //----------------------------------------------
  SaveImagePgm(NAME_IMG_OUT1,mat_img,length,width);
  SaveImagePgm(NAME_IMG_OUT2,mat_img_degraded,length,width);
  
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

  
  //Liberation memoire pour les matrices
  if (mat_img)            free_fmatrix_2d(mat_img); 
  if (mat_img_orig)        free_fmatrix_2d(mat_img_orig); 
  // if (mat_rest)       free_fmatrix_2d(mat_rest);
  // if (mat_rest_prec)  free_fmatrix_2d(mat_rest_prec);
  // if (mat_rest_best)  free_fmatrix_2d(mat_rest_best);
  // if (mat_psf)        free_fmatrix_2d(mat_psf);
  // if (mat_tmp0)  free_fmatrix_2d(mat_tmp0);
  // if (mat_tmp1)  free_fmatrix_2d(mat_tmp1); 
  // if (mat_tmp2)  free_fmatrix_2d(mat_tmp2); 
  // if (mat_tmp3)  free_fmatrix_2d(mat_tmp3); 
  // if (mat_tmp4)  free_fmatrix_2d(mat_tmp4); 
  // if (mat_tmp5)  free_fmatrix_2d(mat_tmp5); 
  // if (mat_tmp6)  free_fmatrix_2d(mat_tmp6); 
  // if (mat_tmp7)  free_fmatrix_2d(mat_tmp7); 
  // if (mat_tmp8)  free_fmatrix_2d(mat_tmp8);
  // if (mat_tmp9)  free_fmatrix_2d(mat_tmp9); 
  // if (mat_tmp10) free_fmatrix_2d(mat_tmp10);
  // if (mat_tmp11) free_fmatrix_2d(mat_tmp11);
  // if (mat_tmp12) free_fmatrix_2d(mat_tmp12);

  //retour sans probleme 
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}


