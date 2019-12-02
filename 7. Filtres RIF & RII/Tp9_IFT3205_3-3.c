/*------------------------------------------------------*/
/* Prog    : Tp9_IFT3205.c                              */
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

#include "FonctionDemo9.h"

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/
/*------------------------------------------------*/
#define NAME_VISUALISER_IMG "./display "
#define NAME_VISUALISER     "./ViewSig.sh "

/*------------------------------------------------*/
/* PROTOTYPE DE REPTILLIONS ----------------------*/
/*------------------------------------------------*/
double rsb(float* igrec, float* iks, int l) {
    double sum1 = 0.0;
    double sum2 = 0.0;
    int i;
    for (i = 0; i < l; i++){
        sum1 += (igrec[i]) * (igrec[i]);
        sum2 += (iks[i]) * (iks[i]);
    }
    sum1/=l;
    sum1 = sqrt(sum1);
    sum2/=l;
    sum2 = sqrt(sum2);

    return 20*(log((sum1/sum2))/log(10));
}

static float last_y[3] = {
    0.0, 
    0.0, 
    0.0
};

float return_actual_value(float* ptr, int n) {
    if(n < 0)
        return 0;
    
    return ptr[n];
}

float y(int n, float* x) {
    float yn = -1/2.0 * return_actual_value(x, n) + return_actual_value(x, n - 1) +
        17/6.0 * last_y[0] - 11/6.0 * last_y[1] + 1/3.0 * last_y[2];

    last_y[2] = last_y[1];
    last_y[1] = last_y[0];
    last_y[0] = yn;
    
    return yn;
}

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL ---------------------------*/
/*------------------------------------------------*/
int main(int argc,char **argv)
{
    int i,j;
    char BufSystVisuSig[100];
    int length;
    float rho = 0.99;

    //===============================
    //Question 1
    //===============================

    float* SignX=LoadSignalDat("SoundFileInCanal", &length);
    float* SignY=fmatrix_allocate_1d(length);

    for(i=0; i<length; i++) SignY[i] = y(i, SignX);
    
    printf("\nRSB: %f",rsb(SignX, SignY, length));

    //Sauvegarde en fichier .dat
    SaveSignalDat("SoundFileInCanalRestor", SignY, length);

    //Visu Ecran
    strcpy(BufSystVisuSig,NAME_VISUALISER);
    strcat(BufSystVisuSig,"SoundFileInCanalRestor.dat&");
    printf(" %s",BufSystVisuSig);
    system(BufSystVisuSig);

    //retour sans probleme
    printf("\n C'est fini ... \n\n");
    return 0;
}
