/***************************************************************************
                            fft.c  -  description
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
#include <rfftw.h>
#include "chooch.h"
     
int dofft(int N, double *yin, double *yout, int dir)
{
  int i;
  double norm;
  fftw_real in[N], out[N];
  rfftw_plan p, pinv;
  //  
  norm = 1/sqrt((double) N);
  
  for (i=0;i<N;i++) {
    in[i] = (fftw_real) yin[i];
  }

  if (dir == FORWARD)
    p = rfftw_create_plan(N, FFTW_REAL_TO_COMPLEX, FFTW_ESTIMATE);
  if (dir == REVERSE)
    p = rfftw_create_plan(N, FFTW_COMPLEX_TO_REAL, FFTW_ESTIMATE);

  rfftw_one(p, in, out);
  for (i = 0; i < N; i++) {
    yout[i] = norm * (double) out[i];
  }
  rfftw_destroy_plan(p);
  return 0;
}

