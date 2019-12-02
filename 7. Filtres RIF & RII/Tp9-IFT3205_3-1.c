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
/* PROTOTYPE DE FONCTIONS  -----------------------*/   
/*------------------------------------------------*/

float getDat(float* ptr, int n) {
    if(n < 0)
        return 0;
    
    return ptr[n];
}

float y(int n, float* x) {
    return getDat(x, n) - 7.0/3 * getDat(x, n - 1) + 2.0/3 * getDat(x, n - 2);
}

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/                     
/*------------------------------------------------*/
int main(int argc,char **argv)
{
    int i,j;
    char BufSystVisuSig[100];
    int length = 10;
    float theta = 2 * PI * 500/11025.0;
    float rho = 0.99;
    float dirac[10] = {1,0,0,0,0,0,0,0,0,0};
    float ident[10] = {1,1,1,1,1,1,1,1,1,1};


    printf("Reponse-Dirac:: ");

    for(i=0; i<length; i++) printf("[%f]", y(i, dirac));


    printf("\nReponse-Echelon:: ");
    
    for(i=0; i<length; i++) printf("[%f]", y(i, ident));

    printf("\n");

    return 0;    
}


