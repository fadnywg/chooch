/***************************************************************************
                          chooch.h  - main header file for chooch
                             -------------------
    begin                : Sat Mar  9 09:51:02 GMT 2002
    copyright            : (C) 2002 by Gwyndaf Evans
    email                : gwyndaf@gwyndafevans.co.uk
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#define PI 3.141592653589793

#define MAX(x, y) (((x) > (y)) ? (x) : (y)) 
#define MIN(x, y) (((x) < (y)) ? (x) : (y)) 
#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif
#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#endif

/*
 * Array sizes
 */

#define MAXSIZE 5000
#define MAXCOEF 50
#define MAXREG  20
#define TITLE   80
/*
 * Colours for PLPLOT
 */

#define BLACK       0
#define RED         1
#define YELLOW      2
#define GREEN       3
#define AQUAMARINE  4
#define PINK        5
#define WHEAT       6
#define GREY        7
#define BROWN       8
#define BLUE        9
#define VIOLET     10
#define CYAN       11
#define TURQUOISE  12
#define MAGENTA    13
#define SALMON     14
#define WHITE      15

/*
 * FFT directions
 */

#define FORWARD 1
#define REVERSE 2
#define ODD  1
#define EVEN 0
#define NOHEADER 0
#define HEADER 1

typedef double real; // could be double.
/*typedef struct {real x; real y;} point;*/
/*
 *
 */

#include "mucal.h"

void printbanner();
int  fluread(char *, real *, real *, int *);
int  efswrite(char *, real *, real *, real *, int);
void normfit(int, float *, float *);
/*
 * int dofft(int , real *, real *, int);
 * int prep_gaussian(int, real, real *);
 * int mirror(int, real *, real *);
 * int fermifilter(int, int, real *);
*/
/*
 *
 */
int checks(int , real *, real *, real *);
void savwin(double, double, double, int *);
/*
 * fdprime
 */
real get_CrossSection(char *, real);
real get_fpp (char *, real);
real get_splinor (char *, real);
char *edge_name(int);
char *get_Edge(char *, double, double *);
/*
 * fits
 */
int linear_fit(int , real *, real *, real *, real *);
int quadfit (int , real *, real *, real *);

/*
 * normalize
 */
int normalize(int, double, real *, real *, real *, int, real *, real *);
int impose(int, double, real *, real *, real *);

/*
 * smooth
 */
int smooth(int , real *, real *, int, int, int, int);
int apply_coeffs(float *, double *, float *, int , int , int );
void pad(int , int , int , real *, float *);

/*
 * Dumping PS and PNG plots
 */

void psplt(int, double *, double *, double *, char *);
void plpng(int, double *, double *, double *, char *, char *, int);

/*
 * toplot
 */
void toplot(int , real *, real *, char *, int);
void spacebar();
void addline(int, real *, real *, int);
/*
 * usage
 */
void usage();

/*
 * copyright
 */
void copyright();

/*
 * license
 */
void license();
void nowarranty();
void distribution();

/*
 * integrate
 */
void Integrate(int, int *, double, double *, double *, double *, double *, double *, double *, double *, double *);
double f(double, void *);
double fc(double, void *);
double fcp(double, void *);
double fs(double, void *);
double IntegrateExtrap(int, double, double, double, double);
double CauchyCurve(int, double, double, double);
double IntegrateCurve(int, double, double, double);
double Singularity(double, double, double, double, double, double, double, double, double);
/*
 * minmax
 */
int minmax(int, float *, float *, float *);
/*
 *
 */
void psplt(int, double *, double *, double *, char *);
/*
 *
 */
void parse(int, char **);
/*
 *
 */
int selwavel(int, double *, double *, double *);


