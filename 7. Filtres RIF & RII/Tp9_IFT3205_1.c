/*------------------------------------------------------*/
/* Prog    : Tp7_IFT3205.c                              */
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
float prevY[2] = {
    0.0,
    0.0
};

float getX(float* ptr, int n) {
    if(n < 0)
        return 0;
    
    return ptr[n];
}

float y(int n, float* x, float rho, float theta) {

    float yn = getX(x, n) - getX(x, n - 1) + 2 * rho * cos(theta) * prevY[0] - rho*rho * prevY[1];
    prevY[1] = prevY[0];
    prevY[0] = yn;

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

    //===============================
    //Question 1
    //===============================

    length=256;
    float*  SignX=fmatrix_allocate_1d(length);
    float*  SignY=fmatrix_allocate_1d(length);
  
    //Signal d'entr�  x(n) al�atoire compris entre [0::200]
    for(i=0;i<length;i++) SignX[i]=(int)(((float)rand()/RAND_MAX)*200.0);
    for(i=0;i<length;i++) SignY[i] = y(i, SignX, 0.99, PI/8.0);
    
    //Sauvegarde en fichier .dat
    SaveSignalDat("SignalX", SignX, length); 
    SaveSignalDat("SignalY", SignY, length); 

    //Visu Ecran
    strcpy(BufSystVisuSig,NAME_VISUALISER);
    strcat(BufSystVisuSig,"SignalX.dat&");
    printf(" %s",BufSystVisuSig);
    system(BufSystVisuSig);

    strcpy(BufSystVisuSig,NAME_VISUALISER);
    strcat(BufSystVisuSig,"SignalY.dat&");
    printf(" %s",BufSystVisuSig);
    system(BufSystVisuSig);


    //---------------------------------------------
   //Sauvegarde de SignZ (30000 echantillons 
   //al�atoire entre [0::200]) dans un 
   //fichier .wav avec une periode 
   //d'echantillonnage de 10000: Nb echant/secondes
   //(pour 3 secondes d'�coute)
   //----------------------------------------------
   //----------------------------------------------
//    if (1)
//       {
//        length=30000;
//        float*  SignZ=fmatrix_allocate_1d(length);
//        for(i=0;i<length;i++) SignZ[i]=(int)(((float)rand()/RAND_MAX)*200.0);
//        SaveSignalDatWav("SignalZ",SignZ,length,10000);
//       }
     

     //Rappel -Pour Lecture/Sauvegarde/Visu [Fichier Son]
     //=================================================
     //float*  SignIn=LoadSignalDat("NameSignalIn",&length);
     //SaveSignalDat("NameSignalOut",SignIn,length); 
     //strcpy(BufSystVisuSig,NAME_VISUALISER);
     //strcat(BufSystVisuSig,"NameSignalOut.dat&");
     //printf(" %s",BufSystVisuSig);
     //system(BufSystVisuSig);
  
    
    //==End=========================================================
    
    //retour sans probleme
    printf("\n C'est fini ... \n\n");
    return 0; 	 
}


