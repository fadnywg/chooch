/***************************************************************************
                            checks.c  -  description
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
  int i, j, err;
  real Steps[MAXREG];
  real tmp;

  *dStep=100000;
  j = 0;
  for (i = 0; i < nDataPoints; i++) {
    if(fXraw[i] < 1000.0)
      fXraw[i] *= 1000.0; // Convert from KeV to eV
    if(i != 0) {
      tmp = fXraw[i] - fXraw[i-1];
      if (tmp < 0.0) {
	err = 1;
	printf("Error in input data: energy does not increase monotonically");
	exit(0);
      }
      *dStep = (tmp < *dStep) ? tmp : *dStep;
    }
  }
  return err;
}
