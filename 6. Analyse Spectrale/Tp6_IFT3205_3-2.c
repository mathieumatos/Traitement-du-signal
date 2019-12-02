/*------------------------------------------------------*/
/* Prog    : Tp6_IFT3205.c                              */
/* Auteur  : Dereck McDuff et Mathieu Matos             */
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

#include "FonctionDemo6.h"

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/   
/*------------------------------------------------*/
#define NAME_VISUALISER "./ViewSig.sh "

/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/   
/*------------------------------------------------*/

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
 {
  int i,j,k;
  int length;
  char BufSystVisuSig[100];


  //===============================
  //Question 3.2
  //===============================
  float*  Sign1=LoadSignalDat("moteur2",&length);
  float*  Sign1I=fmatrix_allocate_1d(length);
  float*  Sign1M=fmatrix_allocate_1d(length); 


  float* tempMat = fmatrix_allocate_1d(512);
  float* tempMatI = fmatrix_allocate_1d(512);
  float* matMoy = fmatrix_allocate_1d(512);
  float* matMoyI = fmatrix_allocate_1d(512);
  



  k = 0;

  //init les matrices à 0
  for(j = 0; j < 512; j++) {
      matMoy[j] = 0;
      matMoyI[j] = 0;
  }

  for(i = 0; i < length - 256; i += 256) {
      k++;
      
      for(j = 0; j < 512; j++) {
          tempMat[j] = Sign1[i + j];
          tempMatI[j] = 0;
      }

      FFT1D(tempMat, tempMatI, 512);
      
      for(j = 0; j < 512; j++) {
          matMoy[j] += fabs(tempMat[j]);
          matMoyI[j] += fabs(tempMatI[j]);
      }
  }

  for(j = 0; j < 512; j++) {
      matMoy[j] /= (float)k;
      matMoyI[j] /= (float)k;
  }
  
  ModVct(tempMat, matMoy, matMoyI, 512);
  CenterVct(tempMat, 512);
  SaveSignalDat("FFT_Moteur2_PermMoy",tempMat,512);  
   
  //Visu
  strcpy(BufSystVisuSig,NAME_VISUALISER);
  strcat(BufSystVisuSig,"FFT_Moteur2_PermMoy.dat&");
  printf(" %s",BufSystVisuSig);
  system(BufSystVisuSig);

  //==End=========================================================
  //==============================================================

  //retour sans probleme
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}


