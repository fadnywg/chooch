/***************************************************************************
                          fluread.c  -  description
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
//
#include "chooch.h"
//
int fluread(char *filename, double *x, double *y, int *nDataPoints)
{
  int    i;
  int    len;
  float  fXread[MAXSIZE], fYread[MAXSIZE];
  char   cScanTitle[TITLE];
  FILE   *ff;
  //
  ff = fopen(filename, "r");
  // Read in header of raw fluorescence data file
/*    len = getline(cScanTitle, 80); */
  fgets(cScanTitle, 80, ff);
  fscanf(ff, "%d", nDataPoints);
  printf("Title: %s\nNo. data points: %d\n", cScanTitle, *nDataPoints);
  //
  for (i = 0; i < *nDataPoints; i++) {
    fscanf(ff, "%f%f\n", &fXread[i], &fYread[i]);
    x[i] = (double) fXread[i];
    y[i] = (double) fYread[i];
    //    printf("%10.3f  %10.3f\n", x[i], y[i]);
    }
  fclose(ff);
  return EXIT_SUCCESS;
}



