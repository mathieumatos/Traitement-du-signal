/*------------------------------------------------------*/
/* Prog    : Tp9_IFT3205.c                              */
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

#include "FonctionDemo9.h"

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/   
/*------------------------------------------------*/
#define NAME_VISUALISER_IMG "./display "
#define NAME_VISUALISER     "./ViewSig.sh "

/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/   
/*------------------------------------------------*/
static float last_y[] = { 0.0, 0.0 };
const float theta = 2 * PI * 500/11025.0;

float return_actual_value(float* ptr, int num) {
    if(num < 0) {
        return 0;
    }
    return ptr[num];
}

float y(int num, float* x, float rho) {
   float yn = return_actual_value(x, num) - 2 * cos(theta) * return_actual_value(x, num - 1) + return_actual_value(x, num - 2) + 2 * rho * cos(theta) * last_y[0] - rho * rho * last_y[1];
   last_y[1] = last_y[0];
   last_y[0] = yn;
   return yn;
}
/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
 {
  int i,j;
  char BufSystVisuSig[100];
  int length;
  float rho = 0.99;

  float* SignX=LoadSignalDat("SoundFile", &length);
  float* SignY=fmatrix_allocate_1d(length);

   for(i=0;i<length;i++)  {
      SignX[i] = SignX[i] + sin(500/11025.0 * 2 * PI * i);
   }

   for(i=0; i<length; i++) {
      SignY[i] = y(i, SignX, rho);
   }

   //Sauvegarde en fichier .dat
   SaveSignalDat("SoundFileRest",SignX,length);

   //Visu Ecran
   strcpy(BufSystVisuSig,NAME_VISUALISER);
   strcat(BufSystVisuSig,"SoundFileRest.dat&");
   printf(" %s",BufSystVisuSig);
   system(BufSystVisuSig);

  printf("\num C'est fini ... \num\num");
  return 0; 	 
}


