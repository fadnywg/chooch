/***************************************************************************
                            checks.c  - do checks on input data
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
#include "chooch.h"

int checks (int nDataPoints, real *fXraw, real *fYraw, real *dStep)
{
  extern int verbose;
  int i, err=0;
  real Steps[MAXREG];
  real runtot, avstep, conv=1.0;
  real tmp[MAXSIZE];

  *dStep=100000;
  /*
   * If input energy looks like it is in keV then convert to eV and
   * print a warning message
   */
  if(fXraw[0] < 300.0) {
     conv=1000.0;
     printf("Warning: Input data automatically converted from keV to eV\n");
  }
  for (i = 0; i < nDataPoints; i++) {
     fXraw[i] *= conv; // Convert from KeV to eV if conv != 1.0
     if(i != 0) {
	tmp[i-1] = fXraw[i] - fXraw[i-1];
        if(verbose>1)printf("points %d and %d : Energy step  %8.4f\n", i-1, i, tmp[i-1]);
     }
  }
  for(i = 1; i < nDataPoints; i++) { 
    if (tmp[i-1] < 0.0) {
      err = 1;
      printf("Error in input data: energy does not increase monotonically");
      exit(0);
    }
    /* Now calculate a 5 point moving average to get minimum average step size */
    runtot+=tmp[i-1];
    if(i > 3){
      avstep=runtot/5.0;
      if(verbose>1)printf("average no. %d : avstep  %8.4f\n", i, avstep);
      *dStep = (avstep < *dStep) ? avstep : *dStep;
      runtot-=tmp[i-4];
    }
  }
  return err;
}
