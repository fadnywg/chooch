
#include <stdio.h>
#include <math.h>
#include <string.h>
/*#include <streams/streams.h>*/
#include <cgraph.h>
#include "chooch.h"
#include <dislin.h>
       
void pngplt(int nDataPoints, double *dX, double *dY1, double *dY2, char *filename) {
   extern double fpInfl, fppInfl, fpPeak, fppPeak, EInfl, EPeak;
   extern char cScanTitle[];
   float fMinX, fMaxX, fMinY, fMaxY, fLenX, fLenY, fStepX, fStepY, fDum;
   float fXplot[MAXSIZE], fY1plot[MAXSIZE], fY2plot[MAXSIZE];
   int i;
   int numoff, ticsep;
   int ninside, rot, expand;
   int marktype;
   int ntotal;
   float scale;
   float markersize = 8.0;
   char cfpInfl[30], cfppPeak[30], tablehead[40];

   
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
   fMinY = (float) ((int) fMinY) - 1; 
   fMaxY = (fMaxY >= 0.0)? (fMaxY * 1.15) : (fMaxY - fMinY * 0.1);
   fMaxY = (float) ((int) fMaxY) + 1;
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

   /* 
    *  Start DISLIN calls
    */

   scrmod("revers");
   metafl("png");
   setfil(filename);
   filmod("delete");
   disini();
   pagera();
   complx();
   axspos(450,1800);
   axslen(2200,1200);
   
   //   bmpfnt("Simplex");
   name("X-ray energy (eV)","x");
   name("f\' and f\'\' (e)","y");
   
   labdig(-1,"x");
   ticks(2,"xy");
   
   sprintf(tablehead,"       energy      f\'\'    f\'");
   sprintf(cfppPeak, "peak  %8.2f   %5.2f  %5.2f", EPeak, fppPeak, fpPeak);
   sprintf(cfpInfl,  "infl  %8.2f   %5.2f  %5.2f", EInfl, fppInfl, fpInfl);
   titlin(cScanTitle,1);
   titlin(tablehead,2);
   titlin(cfppPeak,3);
   titlin(cfpInfl,4);
   
   
   axsbgd(0);
   graf(fMinX,fMaxX,fMinX,fStepX,fMinY,fMaxY,fMinY,fStepY);
   title();
   
   color("white");
   curve(fXplot,fY1plot,nDataPoints);
   color("white");
   curve(fXplot,fY2plot,nDataPoints);
   
   dash();
   xaxgit();
   
   disfin();
}

 


 

