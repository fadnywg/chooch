/***************************************************************************
                          normalize.c  -  normalise spectrum - find f''
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include "chooch.h"
void SetConst(int , double , double *);
int  DoFit(int, double *, double*, double*, double, double);
extern char *sElement;
extern double fE1, fE2, fE3, fE4;
/* 
 *
 */
int  normalize(int nDataPoints, double fEdge, double *fXraw, double *fYraw, double *fYnorm, int plotX, double *fYfita, double *fYfitb)
{
  extern int verbose;
  int i, j, nFit, err;
  double fXtemp[MAXSIZE], fYtemp[MAXSIZE];
  double fC, fM;
  char label[10];
  if(verbose>0)printf(" Plot switch:   %d\n", plotX);
  if(fE1==0.0)fE1=fXraw[0];
  if(fE2==0.0)fE2=fEdge-20.0;
  if(fE3==0.0)fE3=fEdge+25.0;
  if(fE4==0.0)fE4=fXraw[nDataPoints-1];
  /* BELOW EDGE */
  if((fEdge-fE1) > 30.0) {
     if(verbose>1)printf("Using linear fit to below edge region\n");
     if(DoFit(nDataPoints, fXraw, fYraw, fYfitb, fE1, fE2) == 1){
	SetConst(nDataPoints, fYraw[0], fYfitb);
     }
  } else {
     SetConst(nDataPoints, fYraw[0], fYfitb);
  }
  /*
  if(plotX)
     addline(nDataPoints, fXraw, fYfitb, BLUE);
  */
  /* ABOVE EDGE */
  if((fE4-fEdge) > 30.0) {
     if(verbose>1)printf("Using linear fit to above edge region\n");
     if(DoFit(nDataPoints, fXraw, fYraw, fYfita, fE3, fE4) == 1){
	SetConst(nDataPoints, fYraw[nDataPoints-1], fYfita);
     }
  } else {
     SetConst(nDataPoints, fYraw[nDataPoints-1], fYfita);
  }
  /*
  if(plotX)
     addline(nDataPoints, fXraw, fYfita, BLUE);
  */
  /* DO THE NORMALISATION */
  for (i = 0; i < nDataPoints; i++) {
     fYnorm[i] = (fYraw[i] - fYfitb[i]) / (fYfita[i] - fYfitb[i]);
  }
}

void SetConst(int n, double f, double *Array)
{
   int i;
   printf("Warning: Insufficient data - assuming constant value of %f in normalisation\n", f);
   for (i = 0; i < n; i++) {
      Array[i] = f;
   }
}

/*
 * Function to do linear regression to a set of data points {X,Y} using only data
 * between limits x1 and x2 on X.
 * Returns array fFit with linear fit over whole range of data
 */
int DoFit(int nDataPoints, double *fX, double *fY, double *fFit, double x1, double x2 )
{
  double fXtemp[MAXSIZE], fYtemp[MAXSIZE];
  double fC, fM;
  int err, i, j = 0;
  for (i = 0; i < nDataPoints; i++) {
     if(fX[i] > x1 && fX[i] < x2) {
	fXtemp[j] = fX[i];
	fYtemp[j] = fY[i];
	 j++;
     }
  }
  if(j < 3){
     return 1;
  }
  if(linear_fit(j, fXtemp, fYtemp, &fC, &fM) != 0){
     printf("Error in linear regression routine\n");
  }
  for (i = 0; i < nDataPoints; i++) {
     fFit[i] = fC + fM * fX[i];
  }
  return 0;
}

int impose(int nDataPoints, double fEdge, double *fXraw, double *fYnorm, double *fYfpp)
{
  extern int verbose;
  int i, nFit, err;
  double fXtemp[MAXSIZE], fYtemp[MAXSIZE];
  double fYfita[MAXSIZE], fYfitb[MAXSIZE];
  double C[3];
  char label[10];

  /* BELOW EDGE */
  for(i=0; i<50; i++){
    fXtemp[i] = fEdge-60+(float)i;
    fYtemp[i] = get_fpp(sElement, fXtemp[i]/1000.0);
    /*
    printf("%10.3f %10.3f\n", fXtemp[i], fYtemp[i]);
    */
  }
  nFit = i-2;
  err = quadfit(nFit, fXtemp, fYtemp, C);
  if(verbose>1)printf("# best qaud fit: Y = %g + %g X + %g X^2\n", C[0], C[1], C[2]);
  for (i = 0; i < nDataPoints; i++) {
    fYfitb[i] = C[0] + C[1] * fXraw[i] + C[2] * fXraw[i] * fXraw[i];
  }
  /*
  addline(nDataPoints, fXraw, fYfitb, YELLOW);
  */
  /* ABOVE EDGE */
  for (i = 0; i < 50; i++) {
     fXtemp[i] = fEdge+10+(float)i;
     fYtemp[i] = get_fpp(sElement, fXtemp[i]/1000.0);
  }
  nFit = i-2;
  err = quadfit(nFit, fXtemp, fYtemp, C);
  if(verbose>1)printf("# best quad fit: Y = %g + %g X + %g X^2\n", C[0], C[1], C[2]);
  for (i = 0; i < nDataPoints; i++) {
    fYfita[i] = C[0] + C[1] * fXraw[i] + C[2] * fXraw[i] * fXraw[i];
  }
  /*
  addline(nDataPoints, fXraw, fYfita, GREEN);
  */
  /*
   * Apply fits to normalized data to produce f''
   */
  for (i = 0; i < nDataPoints; i++) {
    fYfpp[i] = (fYnorm[i] * (fYfita[i] - fYfitb[i])) + fYfitb[i];
  }
}



