//------------------------------------------------------
// Prog    : Tp5_IFT3205                          
// Auteur  : Dereck McDuff et Mathieu Matos
// Date    :                                  
// version : à jour
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

#include "FonctionDemo5.h"

//------------------------------------------------
// DEFINITIONS -----------------------------------
//------------------------------------------------
#define NAME_VISUALISER "display "
#define NAME_IMG_IN  "lena512"
#define NAME_IMG_OUT "image-TpIFT3205-2-3"
#define NAME_IMG_DEG  "lena512_Degraded"

//------------------------------------------------
// PROTOTYPE DE FONCTIONS  -----------------------
//------------------------------------------------
//----------------------------------------------------------
// copy matrix
//----------------------------------------------------------
void copy_matrix(float** mat1,float** mat2,int lgth,int wdth)
{
    int i,j;

    for(i=0;i<lgth;i++) for(j=0;j<wdth;j++) mat1[i][j]=mat2[i][j];
}



//------------------------------------------------
// CONSTANTES ------------------------------------
//------------------------------------------------
#define SIGMA_NOISE  30 

//------------------------------------------------
//------------------------------------------------
// FONCTIONS  ------------------------------------
//------------------------------------------------
//------------------------------------------------


void tcdOpp(float** mat, int coef, int n) {
    int x, y, border, i;
    int coeff_counter = 0;

    for(border = 0; border < n; border++) {
        for(i = 0; i <= border; i++) {

            //adj or not
            if(border % 2 == 0) {
                x = i;
                y = border - i;
            } else {
                x = border - i;
                y = i;
            }

            if(coeff_counter > coef) {
                mat[y][x] = 0.0;
            }
            
            coeff_counter++;
        }
    }

    for(border = n - 2; border >= 0; border--) {
        for(i = border; i >= 0; i--) {

            if(border % 2 == 0) {
                x = n - i - 1;
                y = n - (border - i) - 1;
            } else {
                x = n - (border - i) - 1;
                y = n - i - 1;
            }
            
            if(coeff_counter > coef) {
                mat[y][x] = 0.0;
            }
            
            coeff_counter++;
        }
    }
}


void zigzagWannabe(float** img, int length, int width, int coef) {

    float** imageBloc = fmatrix_allocate_2d(8, 8);
    //float** huitparhuit = fmatrix_allocate_2d(8,8);

    int bloc_i, bloc_j, i, j;

    //tentative de zigzag échouée, veuillez réessayer plus tard
    // int huitparhuit_n = length/8;
    // int i, j, m, n, *s;
    // for (i = n = 0; i < m * 2; i++)
	// 	for (j = (i < m) ? 0 : i-m+1; j <= i && j < m; j++)
	// 		s[(i&1)? j*(m-1)+i : (i-j)*m+j ] = n++;
        

    for(bloc_i = 0; bloc_i < length/8; bloc_i++){
        for(bloc_j = 0; bloc_j < width/8; bloc_j++){

            for(i=0; i < 8; i++){
                for(j = 0; j < 8; j++){
                    imageBloc[i][j] = img[8*bloc_i + i][8*bloc_j + j];
                }
            }
            
            // TCD negatif
            ddct8x8s(-5, imageBloc);
            // annule les coef TCD
            tcdOpp(imageBloc, coef, 8);
            // TCD positif
            ddct8x8s(38, imageBloc);

            //finish him!            
            for(i = 0; i < 8; i++){
                for(j = 0; j < 8; j++){
                    img[8*bloc_i + i][8*bloc_j + j] = imageBloc[i][j];
                }
            }
        }
    }

    free_fmatrix_2d(imageBloc);
    //free_fmatrix_2d(huitparhuit);
}


void seuilOpp(float** mat, int seuil, int n){

    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){

            if( fabs(mat[i][j]) < seuil){ 
                mat[i][j] = 0; 
            }

        }
    }
}

void debruitageSeuil(float** img, int length, int width, int seuil){

    float** imageBloc = fmatrix_allocate_2d(8, 8);
    //float** huitparhuit = fmatrix_allocate_2d(8,8);

    int bloc_i, bloc_j, i, j;

    //tentative de zigzag échouée, veuillez réessayer plus tard
    // int huitparhuit_n = length/8;
    // int i, j, m, n, *s;
    // for (i = n = 0; i < m * 2; i++)
	// 	for (j = (i < m) ? 0 : i-m+1; j <= i && j < m; j++)
	// 		s[(i&1)? j*(m-1)+i : (i-j)*m+j ] = n++;

    
    for(bloc_i = 0; bloc_i < length/8; bloc_i++){
        for(bloc_j = 0; bloc_j < width/8; bloc_j++){

            for(i=0; i < 8; i++){
                for(j = 0; j < 8; j++){
                    imageBloc[i][j] = img[8*bloc_i + i][8*bloc_j + j];
                }
            }
            
            ddct8x8s(-5, imageBloc);
            seuilOpp(imageBloc, seuil, 8);
            ddct8x8s(38, imageBloc);
            
            //finish him!            
            for(i = 0; i < 8; i++){
                for(j = 0; j < 8; j++){
                    img[8*bloc_i + i][8*bloc_j + j] = imageBloc[i][j];
                }
            }   
        }
    }

    free_fmatrix_2d(imageBloc);
}

//---------------------------------------------------------
//---------------------------------------------------------
// PROGRAMME PRINCIPAL   ----------------------------------                     
//---------------------------------------------------------
//---------------------------------------------------------
int main(int argc,char** argv)
{
    int length,width;
    char BufSystVisuImg[NBCHAR];

    //>Lecture Image 
    float** Img=LoadImagePgm(NAME_IMG_IN,&length,&width);
 
    //>Allocation memory
    float** ImgDegraded=fmatrix_allocate_2d(length,width);
    float** ImgDenoised=fmatrix_allocate_2d(length,width);  

    //>Degradation 
    copy_matrix(ImgDegraded,Img,length,width);
    add_gaussian_noise(ImgDegraded,length,width,SIGMA_NOISE*SIGMA_NOISE);
 
    printf("\n\n  Info Noise");
    printf("\n  ------------");
    printf("\n    > MSE = [%.2f]\n",computeMMSE(ImgDegraded,Img,length)); 
 

    //=========================================================
    //== PROG =================================================
    //=========================================================

    float** debruitage = fmatrix_allocate_2d(length, width);

    float bestMSE = INFINITY;
    float MSE;
    int best;
    
    for(int n = 0; n < 256; n++) {

        copy_matrix(debruitage, ImgDegraded, length, width);
        debruitageSeuil(debruitage, length, width, n);
        MSE = computeMMSE(debruitage, Img, length);
        
        //who will win?
        if(MSE < bestMSE) {
            bestMSE = MSE;
            best = n;
        }
    }

    copy_matrix(debruitage, ImgDegraded, length, width);
    debruitageSeuil(debruitage, length, width, best);

    //extremite
    for(int i = 0; i < length; i++){
        for(int j = 0; j < width; j++){

            if(debruitage[i][j] < 0)
                debruitage[i][j] = 0;
            else if(debruitage[i][j] > 255)
                debruitage[i][j] = 255;
            
        }
    }

    //---------------------------------------------
    // SAUVEGARDE 
    // -------------------
    // L'image dégradée             > ImgDegraded
    // Le resultat du debruitage    > ImgFiltered
    //----------------------------------------------
    SaveImagePgm(NAME_IMG_OUT, debruitage, length, width);

    //Visu ImgFiltered
    strcpy(BufSystVisuImg,NAME_VISUALISER);
    strcat(BufSystVisuImg,NAME_IMG_OUT);
    strcat(BufSystVisuImg,".pgm&");
    printf("\n > %s",BufSystVisuImg);
    system(BufSystVisuImg);

    
//--------------- End -------------------------------------     
//----------------------------------------------------------

    //Liberation memoire pour les matrices
    if (Img)          free_fmatrix_2d(Img);
    if (ImgDegraded)  free_fmatrix_2d(ImgDegraded);
    if (ImgDenoised)  free_fmatrix_2d(ImgDenoised);
  
    //Return
    printf("\n C'est fini... \n");; 
    return 0;
}
 


//----------------------------------------------------------
// Allocation matrix 3d float
// --------------------------
//
// float*** fmatrix_allocate_3d(int dsize,int vsize,int hsize)
// > Alloue dynamiquement un tableau 3D [dsize][vsize][hsize]
//
//-----------------------------------------------------------

//----------------------------------------------------------
//  ddct8x8s(int isgn, float** tab)
// ---------------------------------
//
// Fait la TCD (sgn=-1) sur un tableau 2D tab[8][8]
// Fait la TCD inverse (sgn=1) sur un tableau 2D tab[8][8]
//
//-----------------------------------------------------------
