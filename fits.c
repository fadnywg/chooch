/***************************************************************************
                            fits.c  -  description
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
#include <gsl/gsl_fit.h>
#include <gsl/gsl_multifit.h>
#include "chooch.h"

int linear_fit(int n, double *x, double *y, double *c, double *m)
{
  int i;
  double w[MAXSIZE];
  double c0, c1, cov00, cov01, cov11, chisq;
  
  for (i = 0; i < n; i++) {
    w[i] = 1.0;
  }
  
  gsl_fit_wlinear (x, 1, w, 1, y, 1, n, 
                   &c0, &c1, &cov00, &cov01, &cov11, 
                   &chisq);
  *c = c0;
  *m = c1; 
  printf("# best fit: Y = %g + %g X\n", c0, c1);
  printf("# covariance matrix:\n");
  printf("# [ %g, %g\n#   %g, %g]\n", 
         cov00, cov01, cov01, cov11);
  printf("# chisq = %g\n", chisq);

/*    for (i = 0; i < n; i++) */
/*      printf("data: %g %g %g\n",  */
/*                    x[i], y[i], 1/sqrt(w[i])); */

/*    printf("\n"); */
  return 0;
}

int quad (int n, double *fX, double *fY, double *fCoef)
{
  int i;
  double xi, yi, ei, chisq;
  gsl_matrix *X, *cov;
  gsl_vector *y, *w, *c;


  X = gsl_matrix_alloc (n, 3);
  y = gsl_vector_alloc (n);
  w = gsl_vector_alloc (n);

  c = gsl_vector_alloc (3);
  cov = gsl_matrix_alloc (3, 3);

  for (i = 0; i < n; i++)
    {
      xi = fX[i];
      yi = fY[i];
      ei = 1.0;

      printf("%g %g +/- %g\n", xi, yi, ei);
      
      gsl_matrix_set (X, i, 0, 1.0);
      gsl_matrix_set (X, i, 1, xi);
      gsl_matrix_set (X, i, 2, xi*xi);
      
      gsl_vector_set (y, i, yi);
      gsl_vector_set (w, i, 1.0/(ei*ei));
    }

  {
    gsl_multifit_linear_workspace * work 
      = gsl_multifit_linear_alloc (n, 3);
    gsl_multifit_wlinear (X, w, y, c, cov,
                          &chisq, work);
    gsl_multifit_linear_free (work);
  }

#define C(i) (gsl_vector_get(c,(i)))
#define COV(i,j) (gsl_matrix_get(cov,(i),(j)))

  {
    fCoef[0] = C(0);
    fCoef[1] = C(1);
    fCoef[2] = C(2);
  }

  {
    printf("# best fit: Y = %g + %g X + %g X^2\n", 
           C(0), C(1), C(2));

    printf("# covariance matrix:\n");
    printf("[ %+.5e, %+.5e, %+.5e  \n",
              COV(0,0), COV(0,1), COV(0,2));
    printf("  %+.5e, %+.5e, %+.5e  \n", 
              COV(1,0), COV(1,1), COV(1,2));
    printf("  %+.5e, %+.5e, %+.5e ]\n", 
              COV(2,0), COV(2,1), COV(2,2));
    printf("# chisq = %g\n", chisq);
  }
  return 0;
}








