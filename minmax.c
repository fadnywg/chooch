/***************************************************************************
                          minmax.c  -  description
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

int minmax(int n, float *fArray, float *fMin, float *fMax)
{
  int i;
  *fMin = 10000000;
  *fMax = -10000000;
  for (i = 0; i < n; i++) {
    *fMin = (fArray[i] < *fMin) ? fArray[i] : *fMin;
    *fMax = (fArray[i] > *fMax) ? fArray[i] : *fMax;
  }
  return EXIT_SUCCESS;
}
