/*------------------------------------------------------*/
/* Prog    : Tp3_IFT3205-4-1.c                          */
/* Auteur  : Dereck McDuff, Mathieu Matos               */
/* Date    : 03/03/2019                                 */
/* version : Emeraude                                   */
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

#include "FonctionDemo3.h"

/*------------------------------------------------*/
/* DEFINITIONS -----------------------------------*/
/*------------------------------------------------*/
#define NAME_VISUALISER "display "
#define NAME_IMG_IN "ParcMontRoyal"
#define NAME_IMG_OUT "image-TpIFT3205-4-1"

/*------------------------------------------------*/
/* PROTOTYPE DE FONCTIONS  -----------------------*/
/*------------------------------------------------*/

void matrix_resize_ppvoisin(float **src, float **dest, int length1, int width1, int length2, int width2)
{

    float ratio_l = length2 / (float)length1;
    float ratio_w = width2 / (float)width1;

    for (int i = 0; i < length2; i++)
    {
        for (int j = 0; j < width2; j++)
        {
            int new_x = floor(j / ratio_w);
            int new_y = floor(i / ratio_l);
            dest[i][j] = src[new_y][new_x];
        }
    }
}

float matrix_value_association(float **m, int j, int i, int length, int width)
{

    if (i < 0 || i >= length)
    {
        i = (i + length) % length;
    }

    if (j < 0 || j >= width)
    {
        j = (j + width) % width;
    }

    return m[i][j];
}

void matrix_resize_bili(float **src, float **dest, int length1, int width1, int length2, int width2)
{

    float ratio_l = length2 / (float)length1;
    float ratio_w = width2 / (float)width1;

    for (int i = 0; i < length2; i++)
    {
        for (int j = 0; j < width2; j++)
        {

            float xp = j / ratio_w;
            float yp = i / ratio_l;

            int x1 = (int)xp;
            int y1 = (int)yp;

            float dx = xp - x1;
            float dy = yp - y1;

            float f_xy = matrix_value_association(src, x1, y1, length1, width1);
            float f_x1y = matrix_value_association(src, x1 + 1, y1, length1, width1);
            float f_xy1 = matrix_value_association(src, x1, y1 + 1, length1, width1);
            float f_x1y1 = matrix_value_association(src, x1 + 1, y1 + 1, length1, width1);
            float f_xpy = f_xy + dx * (f_x1y - f_xy);
            float f_xpy1 = f_xy1 + dx * (f_x1y1 - f_xy1);

            dest[i][j] = f_xpy + dy * (f_xpy1 - f_xpy);
        }
    }
}

void TranslImg(float **m, int length, int width)
{
    for (int x = 0; x < length; x++)
    {
        for (int y = 0; y < width; y++)
        {
            m[x][y] *= pow(-1, (x + y));
        }
    }
}

void loop_iter(float **img_orig, float **img_r,
               int length, int width,
               int offsetx, int offsety,
               int dft_size_x, int dft_size_y, float threshold)
{

    static int number = 0;
    int i, j;
    int MAX = 255;

    float **filtered_r = fmatrix_allocate_2d(dft_size_y, dft_size_x);
    float **filtered_i = fmatrix_allocate_2d(dft_size_y, dft_size_x);
    float **filtered_mod = fmatrix_allocate_2d(dft_size_y, dft_size_x);

    for (i = 0; i < dft_size_y; i++)
    {
        for (j = 0; j < dft_size_x; j++)
        {

            filtered_r[i][j] = img_r[offsety + i][offsetx + j];
            filtered_i[i][j] = 0;
        }
    }

    DFT(filtered_r, filtered_i, dft_size_y, dft_size_x);

    Mod(filtered_mod, filtered_r, filtered_i, dft_size_y, dft_size_x);

    float max = fabs(filtered_r[0][0]);

    for (int i = 0; i < dft_size_y; i++)
    {
        for (int j = 0; j < dft_size_x; j++)
        {
            if (filtered_mod[i][j] < threshold * max)
            {
                filtered_r[i][j] = 0;
                filtered_i[i][j] = 0;
            }
        }
    }

    IDFT(filtered_r, filtered_i, dft_size_y, dft_size_x);

    for (int i = offsety; i < offsety + dft_size_y; i++)
    {
        for (int j = offsetx; j < offsetx + dft_size_x; j++)
        {
            if (img_orig[i][j] == MAX)
            {
                img_r[i][j] = filtered_r[i - offsety][j - offsetx];
            }
        }
    }

    Mult(img_r, 1, length, width);
    free_fmatrix_2d(filtered_r);
    free_fmatrix_2d(filtered_i);
    free_fmatrix_2d(filtered_mod);

    number++;
}

/*------------------------------------------------*/
/* PROGRAMME PRINCIPAL   -------------------------*/
/*------------------------------------------------*/
int main(int argc, char **argv)
{
    int length, width;
    char BufSystVisuImg[100];
    int extra_height = 40;
    int i, j;
    int MAX = 255;

    //Lecture Image
    float **orig = LoadImagePgm(NAME_IMG_IN, &length, &width);
    float **img = fmatrix_allocate_2d(length + extra_height, width);
    float **img_r = fmatrix_allocate_2d(length + extra_height, width);

    for (i = 0; i < length; i++)
    {
        for (j = 0; j < width; j++)
        {
            img_r[i + extra_height][j] = orig[i][j];
        }
    }

    for (j = 0; j < width; j++)
    {
        float average = 0;
        for (i = 0; i < extra_height; i++)
        {
            average += orig[i][j];
        }

        average /= extra_height;

        for (i = 0; i < extra_height; i++)
        {
            img_r[i][j] = average;
            img[i][j] = MAX;
        }
    }

    //Interpolation

    for (i = 0; i < width; i++)
    {
        float counter = 0.4;
        for (j = 0; j < 15; j++)
        {
            loop_iter(img, img_r, length + extra_height, width,
                      i * 4, 0, fmin(14, width - i), 90, counter);
            counter /= 2;
        }
    }

    SaveImagePgm(NAME_IMG_OUT, img_r, length + extra_height, width);

    //Commande systeme: VISU
    strcpy(BufSystVisuImg, NAME_VISUALISER);
    strcat(BufSystVisuImg, NAME_IMG_OUT);
    strcat(BufSystVisuImg, ".pgm&");
    printf(" %s", BufSystVisuImg);
    system(BufSystVisuImg);

    //==End=========================================================

    //retour sans probleme
    printf("\n C'est fini ... \n\n");
    return 0;
}