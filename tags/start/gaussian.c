/***************************************************************************
                            gaussian.c  -  description
                             -------------------
    begin                : Sat July  6 12:01:02 GMT 2002
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
#include <math.h>
#include <rfftw.h>
#include "chooch.h"
     
int prep_gaussian(int N, double sigma, double *y)
{
  int i;
  double x[MAXSIZE];
  double term1, term2;
  char label[10];

  printf("Making a Gaussian of sigma %f\n", sigma);
  for (i = 0; i < N; i++) {
    x[i] = (double) i;
    term1 = -1.0 * x[i] * x[i];
    term2 = 2.0 * sigma * sigma;
    //    y[i] = ( 1/sqrt(2*PI) ) * exp(term1/term2);
    y[i] = exp(term1/term2) / (sqrt(2*PI)*sigma);
    printf("%f %f %f %f\n",term1, term2, x[i], y[i]);
  }
  //  strcpy(label,"Gaussian");
  //  toplot(N, x, y, label);
}











