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

#if defined(PGPLOT)
void toplot(int nDataPoints, double *dX, double *dY, char *sLab, int nColor)
{
  int i;
  float fXplot[nDataPoints], fYplot[nDataPoints];
  float fXref, fYref, fXcur, fYcur;
  float fMinX, fMaxX, fMinY, fMaxY;
  char  ch[1];
  char  label[10];
  //
  for (i = 0 ; i < nDataPoints; i++)
    {
      fXplot[i] = (float) dX[i];
      fYplot[i] = (float) dY[i];
      //      printf("%10.3f   %10.3f \n",fXplot[i], fYplot[i]);
    }
  strcpy(label,sLab);
  minmax(nDataPoints, fXplot, &fMinX, &fMaxX);
  minmax(nDataPoints, fYplot, &fMinY, &fMaxY);
  fMinY = (fMinY >= 0.0)? (fMinY - fMaxY * 0.1) : (fMinY * 1.15);
  fMaxY = (fMaxY >= 0.0)? (fMaxY * 1.15) : (fMaxY - fMinY * 0.1);
  printf("Plotting!\n");
  cpgpage();
  cpgpap(9.0,0.7);
  cpgask(0);
  cpgscf(2);
  cpgslw(2);
  cpgsch(1.6);
  cpgsci(BLACK);
  cpgsvp(0.2,0.90,0.15,0.9);
  cpgswin(fMinX,fMaxX,fMinY,fMaxY);
  cpgbox("bncts1",0.0,0,"bncts",0.0,0);
  cpgsci(BLACK);
  cpgmtxt("b",2.5,0.5,0.5,"X-ray Energy (eV)");
  cpgmtxt("l",4.0,0.5,0.5,label);
  cpgsci(nColor);
  cpgline(nDataPoints, fXplot, fYplot);
}

void spacebar(){
  float fXref, fYref, fXcur, fYcur;
  char  ch[1];
  printf("Press <SPACE> to continue\n");
  cpgband(0, 0, fXref, fYref, &fXcur, &fYcur, ch);
}

void addline(int nDataPoints, double *dX, double *dY, int nColor)
{
  int i;
  float fXref, fYref, fXcur, fYcur;
  float fXplot[nDataPoints], fYplot[nDataPoints];
  char  ch[1];
  for (i = 0 ; i < nDataPoints; i++)
    {
      fXplot[i] = (float) dX[i];
      fYplot[i] = (float) dY[i];
    }

  cpgsci(nColor);
  cpgline(nDataPoints, fXplot, fYplot);
}

void efsplot(int nDataPoints, double *dX, double *dY1, double *dY2, int psplot, char *psfile)
{
  int i;
  extern int id1, id2;
  float fXplot[nDataPoints], fY1plot[nDataPoints], fY2plot[nDataPoints];
  float fXref, fYref, fXcur, fYcur;
  float fMinX, fMaxX, fMinY, fMaxY, fDum;
  char  ch[1];
  char  label[10];
  char  *psinit;
  //
  for (i = 0 ; i < nDataPoints; i++)
    {
      fXplot[i] = (float) dX[i];
      fY1plot[i] = (float) dY1[i];
      fY2plot[i] = (float) dY2[i];
    }
  minmax(nDataPoints, fXplot, &fMinX, &fMaxX);
  minmax(nDataPoints, fY1plot, &fDum, &fMaxY);
  minmax(nDataPoints, fY2plot, &fMinY, &fDum);
  fMinY = (fMinY >= 0.0)? (fMinY - fMaxY * 0.1) : (fMinY * 1.15);
  fMaxY = (fMaxY >= 0.0)? (fMaxY * 1.15) : (fMaxY - fMinY * 0.1);
  if(psplot){
     psinit=strcat(psfile,"/cps");
     id2=cpgopen(psinit);
     if(id2 < 0) exit (EXIT_FAILURE);
     cpgslct(id2);
  }
  cpgpage();
  cpgpap(9.0,0.7);
  cpgask(0);
  cpgscf(2);
  cpgslw(2);
  cpgsch(1.6);
  cpgsci(BLACK);
  cpgsvp(0.2,0.90,0.15,0.9);
  cpgswin(fMinX,fMaxX,fMinY,fMaxY);
  cpgbox("bncts1",0.0,0,"bncts",0.0,0);
  cpgsci(BLACK);
  cpgmtxt("b",2.5,0.5,0.5,"X-ray Energy (eV)");
  cpgmtxt("l",4.0,0.5,0.5,"f' and f'' (e)");
  cpgsci(RED);
  cpgline(nDataPoints, fXplot, fY1plot);
  cpgline(nDataPoints, fXplot, fY2plot);
  if(psplot)cpgpage();
  if(id1 != 0)cpgslct(id1);
}
#endif
