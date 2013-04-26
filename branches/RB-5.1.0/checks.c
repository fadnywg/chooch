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

int checks (int nDataPoints, double *fXraw, double *fYraw, double *dStep)
{
  extern int verbose, kev;
  int i, err=0;
 /* double Steps[MAXREG];*/
  double runtot=0.0, avstep, conv=1.0;
  double tmp[MAXSIZE];

  *dStep=100000;
  /*
   * If -k option used then input data is converted from keV to eV
   */
  if(kev) {
    conv=1000.0;
  }
  for (i = 0; i < nDataPoints; i++) {
    fXraw[i] *= conv; // Convert from KeV to eV if conv != 1.0
    if(i != 0) {
      tmp[i-1] = fXraw[i] - fXraw[i-1];
      if(verbose>1)printf("points %d and %d : Energy step  %8.4f\n", i-1, i, tmp[i-1]);
    }
  }
  /*
   * Check that data increases monotonically
   */
  for(i = 1; i < nDataPoints; i++) { 
    if (tmp[i-1] < 0.0) {
      err = 1;
      printf("Error in input data: energy does not increase monotonically\n");
      printf("\n** Energy value of point %d is bigger than value of point %d **\n", i, i+1);
      exit(EXIT_FAILURE);
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
