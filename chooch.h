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
 * Colours for PGPLOT
 */

#define WHITE    0
#define BLACK    1
#define RED      2
#define GREEN    3
#define BLUE     4
#define CYAN     5
#define MAGENTA  6
#define YELLOW   7
#define ORANGE   8
#define LIME     9
#define LIGHTGREEN 10
#define LIGHTBLUE  11
#define PURPLE     12
#define PINK       13
#define DARKGREY   14
#define LIGHTGREY  15

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

#if defined(PGPLOT)
#include <cpgplot.h>
#endif

#include "mucal.h"

void printbanner();
int  fluread(char *, real *, real *, int *);
int  efswrite(char *, real *, real *, real *, int);
void normfit(int, float *, float *);
int DoSpline(int, real *, real *, real *, real *);
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
void pngplt(int, double *, double *, double *, char *);

/*
 * toplot
 */
#if defined(PGPLOT)
void toplot(int , real *, real *, char *, int);
void spacebar();
void addline(int, real *, real *, int);
void efsplot(int, double *, double *, double *, int, char *);
#endif
/*
 * usage
 */
void usage();

/*
 * copyright
 */
void copyright();

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
 *
 */
void psplt(int, double *, double *, double *, char *);

/*
 *
 */
int selwavel(int, double *, double *, double *);


