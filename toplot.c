/***************************************************************************
                          toplot.c  -  description
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
#include <string.h>
#include "chooch.h"
#include <plplot.h>
#include <plevent.h>


void toplot(int nDataPoints, double *dX, double *dY, char *sLab, int nColor)
{
  int i;
  float fXplot[nDataPoints], fYplot[nDataPoints];
  float fMinX, fMaxX, fMinY, fMaxY;
  char  label[30];
  PLFLT Xmin, Xmax, Ymin, Ymax;
  PLFLT fX[MAXSIZE], fY[MAXSIZE];
  PLINT plNpoints;
  //
  plNpoints = (PLINT) nDataPoints;
  for (i = 0 ; i < nDataPoints; i++)
    {
      fXplot[i] = (float) dX[i];
      fYplot[i] = (float) dY[i];
    }
  strcpy(label,sLab);
  minmax(nDataPoints, fXplot, &fMinX, &fMaxX);
  minmax(nDataPoints, fYplot, &fMinY, &fMaxY);
  fMinY = (fMinY >= 0.0)? (fMinY - fMaxY * 0.1) : (fMinY * 1.15);
  fMaxY = (fMaxY >= 0.0)? (fMaxY * 1.15) : (fMaxY - fMinY * 0.1);

  for (i = 0 ; i < nDataPoints; i++) {
    fX[i] = (PLFLT) fXplot[i];
    fY[i] = (PLFLT) fYplot[i];
  }  

  Xmin = (PLFLT) fMinX;
  Xmax = (PLFLT) fMaxX;
  Ymin = (PLFLT) fMinY;
  Ymax = (PLFLT) fMaxY;

  /*
   *
   */
  
  plcol0(1);
  plclear();
  plvpor(0.15,0.80,0.15,0.8); // Define viewport as fraction of whole page
  plwind(Xmin, Xmax, Ymin, Ymax); // Define world or plot range within viewport
  plbox("bcnstf", 0.0, 0, "bcnstvf", 0.0, 0); // Define axes and graph label options
  pllab("X-ray energy (eV)", label, "Title");
  plcol0(nColor);
  plline(plNpoints, fX, fY);
}

void addline(int nDataPoints, double *dX, double *dY, int nColor)
{
  int i;
  PLFLT fX[nDataPoints], fY[nDataPoints];
  PLINT plNpoints;
  plNpoints = (PLINT) nDataPoints;
  for (i = 0 ; i < nDataPoints; i++)
    {
      fX[i] = (PLFLT) dX[i];
      fY[i] = (PLFLT) dY[i];
    }
  plcol0(nColor);
  plline(plNpoints, fX, fY);
}

void spacebar(){
  float fXref=0, fYref=0, fXcur, fYcur;
  int iok;
  static PLGraphicsIn gin;
  printf("Click on graph to continue\n");
  iok = plGetCursor( &gin);
}

