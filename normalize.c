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
int normalize(int nDataPoints, double fEdge, double *fXraw, double *fYraw, double *fYnorm, int plotX)
{
  int i, j, nFit, err;
  double fXtemp[MAXSIZE], fYtemp[MAXSIZE];
  double fYfita[MAXSIZE], fYfitb[MAXSIZE];
  double fC, fM;
  char label[10];
  printf(" Plot switch:   %d\n", plotX);
  i = 0;
  if((fEdge-fXraw[i]) > 30.0) {
     printf("Using linear fit to below edge region\n");
     while (fXraw[i] < fEdge-20) {
	fXtemp[i] = fXraw[i];
	fYtemp[i] = fYraw[i];
	i++;
     }
     nFit = i-2;
     err = linear_fit(nFit, fXtemp, fYtemp, &fC, &fM);
     for (i = 0; i < nDataPoints; i++) {
	fYfitb[i] = fC + fM * fXraw[i];
     }
     if(plotX)
	addline(nDataPoints, fXraw, fYfitb, BLUE);
  } else {
     fC=fYraw[0];
     printf("Assuming below edge constant value of %f in normalisation\n", fC);
     for (i = 0; i < nDataPoints; i++) {
	fYfitb[i] = fC;
     }
  }
  j = 0;
  for (i = 0; i < nDataPoints; i++) {
    if (fXraw[i] > fEdge+20) {
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
  if(plotX){
     addline(nDataPoints, fXraw, fYfita, BLUE);
  }
  for (i = 0; i < nDataPoints; i++) {
    fYnorm[i] = (fYraw[i] - fYfitb[i]) / (fYfita[i] - fYfitb[i]);
  }
  
  //  strcpy(label,"Normalized data");
  //  toplot(nDataPoints, fXraw, fYnorm, label, GREEN);
}


int impose(int nDataPoints, double fEdge, double *fXraw, double *fYnorm, double *fYfpp) {
  int i, nFit, err;
  double fXtemp[MAXSIZE], fYtemp[MAXSIZE];
  double fYfita[MAXSIZE], fYfitb[MAXSIZE];
  double C[3];
  char label[10];

  /* BELOW EDGE */
  for(i=0; i<50; i++){
    fXtemp[i] = fEdge-60+(float)i;
    fYtemp[i] = get_fpp(sElement, fXtemp[i]/1000.0);
    //    printf("%10.3f %10.3f\n", fXtemp[i], fYtemp[i]);
  }
  nFit = i-2;
  err = quad(nFit, fXtemp, fYtemp, C);
  printf("# best qaud fit: Y = %g + %g X + %g X^2\n", C[0], C[1], C[2]);
  for (i = 0; i < nDataPoints; i++) {
    fYfitb[i] = C[0] + C[1] * fXraw[i] + C[2] * fXraw[i] * fXraw[i];
  }
  //  addline(nDataPoints, fXraw, fYfitb, YELLOW);

  /* ABOVE EDGE */
  for (i = 0; i < 50; i++) {
     fXtemp[i] = fEdge+10+(float)i;
     fYtemp[i] = get_fpp(sElement, fXtemp[i]/1000.0);
  }
  nFit = i-2;
  err = quad(nFit, fXtemp, fYtemp, C);
  printf("# best quad fit: Y = %g + %g X + %g X^2\n", C[0], C[1], C[2]);
  for (i = 0; i < nDataPoints; i++) {
    fYfita[i] = C[0] + C[1] * fXraw[i] + C[2] * fXraw[i] * fXraw[i];
  }
  //  addline(nDataPoints, fXraw, fYfita, GREEN);
  /*
   * Apply fits to normalized data to produce f''
   */
  for (i = 0; i < nDataPoints; i++) {
    fYfpp[i] = (fYnorm[i] * (fYfita[i] - fYfitb[i])) + fYfitb[i];
  }
}



