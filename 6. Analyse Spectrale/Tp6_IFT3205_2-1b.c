/*------------------------------------------------------*/
/* Prog    : Tp6_IFT3205.c                              */
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
  //Question 2.1.(b)
  //===============================
  float*  Sign1=LoadSignalDat("moteur1",&length);
  float*  Sign1I=fmatrix_allocate_1d(length);
  float*  Sign1M=fmatrix_allocate_1d(length);

  int n_echantillons = 8192;
  int taux = 11025;
  float delta = 1.0/(taux /(float) n_echantillons);


  FFT1D(Sign1,Sign1I,length);
  ModVct(Sign1M,Sign1,Sign1I,length);
  // CenterVct(Sign1M,length);
  SaveSignalDat("FFT_Moteur1",Sign1M,length); 

  int iA, iB;
  float maxA = 0;
  float maxB = 0;

  for (i=0; i<n_echantillons/2;i++) {
    if(maxA < Sign1M[i]) {
      maxB = maxA;
      maxA = Sign1M[i];
      iB = iA;
      iA = i;
    } else if (maxB < Sign1M[i]) {
      iB = i;
      maxB = Sign1M[i];
    }
  }

  printf("Le numéro d'échantillon spectral associé au clic : %f  |  %d \n", 1 / (iB * delta), iB);
  printf("Le numéro d'échantillon spectral associé au plus grand pic : %f  |  %d \n", 1 / (iA * delta * 2), iA);

  //Visu
  strcpy(BufSystVisuSig,NAME_VISUALISER);
  strcat(BufSystVisuSig,"FFT_Moteur1.dat&");
  printf(" %s",BufSystVisuSig);
  system(BufSystVisuSig);

  //==End=========================================================
  //==============================================================

  //retour sans probleme
  printf("\n C'est fini ... \n\n");
  return 0; 	 
}


