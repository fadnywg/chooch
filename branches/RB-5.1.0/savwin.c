/***************************************************************************
                    license.c  -  shows license information
                             -------------------
    begin                : Mon Dec  1 20:26:00 CET 2003
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
#include <math.h>
#include "chooch.h"

void savwin(double fEres, double fEdge, double dE, int *nSavWin) {
  
  double fMonoRes;
  extern int verbose; 
  
  fMonoRes = fEres * fEdge;
  *nSavWin = (int) (fMonoRes / dE);
  if(*nSavWin > 29){ 
    *nSavWin=29;
  }
  if(*nSavWin < 2){ 
     *nSavWin=2;
  }
  if(verbose>0)printf("dE = %f Resol = %f\n", dE, fMonoRes);
  if(verbose>0)printf("Savitsky-Golay window value = %d\n", *nSavWin);
}
