/***************************************************************************
                          spline.c  -  description
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
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>
#include "chooch.h"
     
int DoSpline(int nDataPoints, double *x, double *y, double *xi, double *yi)
{
  int i;
  
  printf ("#m=0,S=2\n");
  
/*    for (i = 0; i < nDataPoints; i++) */
/*      { */
/*        printf ("%g %g\n", x[i], y[i]); */
/*      } */
  
  printf ("#m=1,S=0\n");
  
  {
    gsl_interp_accel *acc
      = gsl_interp_accel_alloc ();
    gsl_spline *spline
      = gsl_spline_alloc (gsl_interp_akima, nDataPoints);
    //      = gsl_spline_alloc (gsl_interp_cspline, nDataPoints);
    
    gsl_spline_init (spline, x, y, nDataPoints);
    
    for (i = 0; i < nDataPoints; i++)
      {
	xi[i]=x[i]-5;	
	yi[i] = gsl_spline_eval (spline, x[i], acc);
	//	printf ("%g %g\n", xi[i], yi[i]);
      }
    gsl_spline_free (spline);
    gsl_interp_accel_free(acc);
  }
  return 0;
}











