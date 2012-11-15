/***************************************************************************
                          plpng.c  -  output png or PostScript plot
                             -------------------
    begin                : Sat Mar  9 09:51:02 GMT 2002
    copyright            : (C) 1994 - 2012 by Gwyndaf Evans
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

#include <stdio.h>
#include <math.h>
#include <string.h>
#include "chooch.h"
#include <plplot.h>
       
void plpng(int nDataPoints, double *dX, double *dY1, double *dY2, char *filename, char *device, int orien) {
   extern double fpInfl, fppInfl, fpPeak, fppPeak, EInfl, EPeak;
   extern char cScanTitle[TITLE];
   float fMinX, fMaxX, fMinY, fMaxY, fLenX, fLenY, fStepX, fStepY, fDum;
   float fXplot[MAXSIZE], fY1plot[MAXSIZE], fY2plot[MAXSIZE];
   int i;
   PLFLT Xmin, Xmax, Ymin, Ymax;
   PLFLT fX[MAXSIZE], fY1[MAXSIZE], fY2[MAXSIZE];
   PLINT plNpoints, ori;

/*   int numoff, ticsep;
   int ninside, rot, expand;
   int marktype;
   int ntotal;
   float scale;
   float markersize = 8.0; */
   char cfpInfl[40], cfppPeak[40], tablehead[40];

   plNpoints = (PLINT) nDataPoints;
   ori = (PLINT) orien;
   for (i = 0 ; i < nDataPoints; i++) {
     fXplot[i] = (float) dX[i];
     fY1plot[i] = (float) dY1[i];
     fY2plot[i] = (float) dY2[i];
     /*      printf("%d %f %f %f\n",i, fXplot[i], fY1plot[i], fY2plot[i]);*/
   }
   minmax(nDataPoints, fXplot, &fMinX, &fMaxX);
   minmax(nDataPoints, fY1plot, &fDum, &fMaxY);
   minmax(nDataPoints, fY2plot, &fMinY, &fDum);
   fMinY = (fMinY >= 0.0)? (fMinY - fMaxY * 0.05) : (fMinY * 1.05);
   fMinY = (PLFLT) ((int) fMinY) - 1; 
   fMaxY = (fMaxY >= 0.0)? (fMaxY * 1.15) : (fMaxY - fMinY * 0.1);
   fMaxY = (PLFLT) ((int) fMaxY) + 1;
   fLenX=fMaxX-fMinX;
   fLenY=fMaxY-fMinY;
   fStepX=2;
   while(fLenX/fStepX > 5.0) {
     fStepX+=2.0;
   }
   fStepY=2;
   while(fLenY/fStepY > 8.0) {
     fStepY+=2.0;
   }

   for (i = 0 ; i < nDataPoints; i++) {
     fX[i] = (PLFLT) fXplot[i];
     fY1[i] = (PLFLT) fY1plot[i];
     fY2[i] = (PLFLT) fY2plot[i];
   }

   Xmin = fMinX;
   Xmax = fMaxX;
   Ymin = fMinY;
   Ymax = fMaxY;


   /*
    * Start PLPlot calls
    */

   sprintf(tablehead,"       energy      f\'\'    f\'");
   sprintf(cfppPeak, "peak  %8.2f   %5.2f  %5.2f", EPeak, fppPeak, fpPeak);
   sprintf(cfpInfl,  "infl  %8.2f   %5.2f  %5.2f", EInfl, fppInfl, fpInfl);
   printf("infl  %8.2f   %5.2f  %5.2f", EInfl, fppInfl, fpInfl);
   
   plsdev(device);
   plsdiori(ori);
   plsfnam(filename); 
   plscol0(0, 255,255,255); // Set background color to White
   plscol0(1, 0, 0, 0); // Set foreground color to Black

   plinit();
   plfont(1); // Simple=1; Roman=2; Italic=3; Script=4

   pladv(0); // Increment subpage number
   plvpor(0.15,0.80,0.15,0.8); // Define viewport as fraction of whole page
   plscol0(1, 0, 0, 0);
   plwind(Xmin, Xmax, Ymin, Ymax); // Define world or plot range within viewport

   plbox("bcnstf", 0.0, 0, "bcnstvf", 0.0, 0); // Define axes and graph label options
   plline(plNpoints, fX, fY1);
   plline(plNpoints, fX, fY2);
   //   pllab("X-ray energy (eV)", "f\' and f\'\' (e)", "Title");
   plmtex("t", 7.0, 0.5, 0.5, cScanTitle);
   plmtex("t", 5.5, 0.5, 0.5, tablehead);
   plmtex("t", 3.5, 0.5, 0.5, cfppPeak);
   plmtex("t", 1.5, 0.5, 0.5, cfpInfl);
   plend();
}

 


 

