/***************************************************************************
                          selwavel.c  -  select wavelengths
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

int selwavel(int n, double *fXfpp, double *fpp, double *fp)
{
   extern double fpInfl, fppInfl, fpPeak, fppPeak, EInfl, EPeak;
   int i;
   fpInfl = 10000000;
   fppPeak = -10000000;
   for (i = 0; i < n; i++) {
      if(fp[i] < fpInfl){
	 fpInfl = fp[i];
	 fppInfl = fpp[i];
	 EInfl = fXfpp[i];
      }
      if(fpp[i] > fppPeak){
	 fpPeak = fp[i];
	 fppPeak = fpp[i];
	 EPeak = fXfpp[i];
      }
   }
   return EXIT_SUCCESS;
}
