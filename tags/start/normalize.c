/***************************************************************************
                          normalize.c  -  description
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
extern char *sElement;

int normalize(int nDataPoints, double *fXraw, double *fYraw, double *fYnorm)
{
  int i, j, nFit, err;
  double fXtemp[MAXSIZE], fYtemp[MAXSIZE];
  double fYfita[MAXSIZE], fYfitb[MAXSIZE];
  double SeK = 12665.0;
  double fC, fM;
  char label[10];

  i = 0;
  while (fXraw[i] < SeK-20) {
    fXtemp[i] = fXraw[i];
    fYtemp[i] = fYraw[i];
    i++;
  }
  nFit = i-2;
  err = linear_fit(nFit, fXtemp, fYtemp, &fC, &fM);
  for (i = 0; i < nDataPoints; i++) {
    fYfitb[i] = fC + fM * fXraw[i];
  }
  //  addline(nDataPoints, fXraw, fYfitb, BLUE);

  j = 0;
  for (i = 0; i < nDataPoints; i++) {
    if (fXraw[i] > SeK+20) {
      fXtemp[j] = fXraw[i];
      fYtemp[j] = fYraw[i];
      j++;
    }
  }
  nFit = j-2;
  err = linear_fit(nFit, fXtemp, fYtemp, &fC, &fM);
  for (i = 0; i < nDataPoints; i++) {
    fYfita[i] = fC + fM * fXraw[i];
  }
  //  addline(nDataPoints, fXraw, fYfita, BLUE);
  for (i = 0; i < nDataPoints; i++) {
    fYnorm[i] = (fYraw[i] - fYfitb[i]) / (fYfita[i] - fYfitb[i]);
  }
  
  //  strcpy(label,"Normalized data");
  //  toplot(nDataPoints, fXraw, fYnorm, label, GREEN);
}


int impose(int nDataPoints, double *fXraw, double *fYnorm, double *fYfpp) {
  int i, j, nFit, err;
  double fXtemp[MAXSIZE], fYtemp[MAXSIZE];
  double fYfita[MAXSIZE], fYfitb[MAXSIZE];
  double SeK = 12665.0;
  double C[3];
  char label[10];

  i = 0;
  while (fXraw[i] < SeK-10) {
    fXtemp[i] = fXraw[i];
    fYtemp[i] = get_fpp(sElement, fXraw[i]/1000.0);
    printf("%10.3f %10.3f\n", fXtemp[i], fYtemp[i]);
    i++;
  }
  nFit = i-2;
  err = quad(nFit, fXtemp, fYtemp, C);
/*    printf("# best fit: Y = %g + %g X + %g X^2\n",  */
/*             C[0], C[1], C[2]); */
  for (i = 0; i < nDataPoints; i++) {
    fYfitb[i] = C[0] + C[1] * fXraw[i] + C[2] * fXraw[i] * fXraw[i];
  }
  //  addline(nDataPoints, fXraw, fYfitb, BLUE);

  j = 0;
  for (i = 0; i < nDataPoints; i++) {
    if (fXraw[i] > SeK+10) {
      fXtemp[j] = fXraw[i];
      fYtemp[j] = get_fpp(sElement, fXraw[i]/1000.0);
      j++;
    }
  }
  nFit = j-2;
  err = quad(nFit, fXtemp, fYtemp, C);
  for (i = 0; i < nDataPoints; i++) {
    fYfita[i] = C[0] + C[1] * fXraw[i] + C[2] * fXraw[i] * fXraw[i];
  }
  //  addline(nDataPoints, fXraw, fYfita, BLUE);
  for (i = 0; i < nDataPoints; i++) {
    fYfpp[i] = (fYnorm[i] * (fYfita[i] - fYfitb[i])) + fYfitb[i];
  }
}



