/***************************************************************************
                            tools.c  -  description
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

int mirror (int N, double *in, double *out) 
{
  int i;
  for (i = 0; i < N; i++) {
    out[i] = in[i];
    out[N+i] = in[N-i-1];
  }
}


int fermifilter (int N, int M, double *out)
{
  int i;
  for (i = 0; i < N; i++) {
    out[i] = 1 / (1 + exp(i-M));
  }
}
