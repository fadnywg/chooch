/***************************************************************************
                          main.c  -  description
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <float.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
//
#include "chooch.h"
int c;
char  *sElement="Se";           // Letter symbol for element name e.g. Au, Se, I
//typedef struct{double d1; double d2; double d3;} deriv; 
//
//
int main(int argc, char *argv[])
{
  int i, j, err;
  float fXref, fYref, fXcur, fYcur;
  char *sFilename;
  char label[10];
  char  ch[1];
  char  *sEdge="K";           // Letter symbol for absorption edge K, L1, L2, L3, M
  char opt;
  //
  int nDataPoints, nFit, nPoints, verbose, plotX=0;
  double dE, tmp, fE1, fE2, fE3, fE4, fEdge;
  double fXraw[MAXSIZE], fYraw[MAXSIZE];
  double fYspline[MAXSIZE], fXfpp[MAXSIZE];
  double fYsmooth[MAXSIZE], fYnorm[MAXSIZE];
  double fYfpp[MAXSIZE], fYfpps[MAXSIZE], fYfp[MAXSIZE];
  double fYDeriv1[MAXSIZE], fYDeriv2[MAXSIZE], fYDeriv3[MAXSIZE];
  double C[3], result, error, fMid;
  //
  double SeK = 12665.0;
  double fC, fM;
  //
  optarg = NULL;
  while( ( opt = getopt( argc, argv, "he:a:xl:v:" ) ) != (char)(-1) )
     switch( opt ) {
     case 'h' :
	(void)usage();
	exit(EXIT_SUCCESS);
	break;
     case 'e' :
	sElement = optarg;
	break;
     case 'a' :
	sEdge = optarg;
	break;
     case 'x' :	
	printf("-X: X windows plotting requested\n");
	plotX = 1;
	break;
     case 'l' :
	fE1 = atof(optarg);
	printf("Fit below edge from %f\n", fE1);
	break;
     case 'v' :
	verbose = 1;
	break;
     }
  
  if(argc < 2){
    printf("\nUsage: chooch <filename>\n");
    exit(EXIT_FAILURE);
  }
  sFilename = argv[optind];
  printf("Fluorescence scan filename: %s\n", sFilename);
  //
  if(plotX) 
     cpgbeg(0, "/xw", 1, 1);
  //
  printbanner();
  //
  /*
   * Read in raw spectrum and plot
   */
  fluread(sFilename, fXraw, fYraw, &nDataPoints);
  err=checks(nDataPoints, fXraw, fYraw, &dE);
  fMid=(fXraw[nDataPoints-1]+fXraw[0])/2.0;
  printf("Mid point of spectrum = %f\n", fMid);
  sEdge=get_Edge(sElement, fMid, &fEdge);
  printf("\nSpectrum over %s %s edge at theoretocal energy of %8.2f eV\n", sElement, sEdge, fEdge);
  //  for (i = 0; i < nDataPoints; i++)
  //    fXpoint[i] = (double) i;
  /*
   * Smooth with Savitzky-Golay filter
   */ 
  err = smooth(nDataPoints, fYraw, fYsmooth, 8, 8, 4, 0);
  //  strcpy(label,"Result");  
  //  cpgsci(RED);
  if(plotX){
     toplot(nDataPoints, fXraw, fYraw, "Raw and smoothed data", YELLOW);
     addline(nDataPoints, fXraw, fYsmooth, RED);
  }
  /*
   * Normalise data
   */
  err=normalize(nDataPoints, fEdge, fXraw, fYraw, fYnorm, plotX);
  spacebar();
  /*
   * Impose on theoretical spectrum of f'' to obtain 
   * experimental equivalent
   */
  printf(" Converting spectrum to f''\n");
  err=impose(nDataPoints, fEdge, fXraw, fYnorm, fYfpp);
  /*
   * Determine first and second derivatives of smoothed data
   * and plot them on top of one another.
   */
  err = smooth(nDataPoints, fYfpp, fYfpps, 8, 8, 4, 0); 
  if(plotX)
     toplot(nDataPoints, fXraw, fYfpps, "f'' and derivatives", RED);
  err = smooth(nDataPoints, fYfpp, fYDeriv1, 8, 8, 4, 1);
  if(plotX)
     addline(nDataPoints, fXraw, fYDeriv1, YELLOW);
  err = smooth(nDataPoints, fYfpp, fYDeriv2, 8, 8, 4, 2);
  if(plotX){
     cpgsci(BLUE);
     addline(nDataPoints, fXraw, fYDeriv2, BLUE);
  }
  err = smooth(nDataPoints, fYfpp, fYDeriv3, 8, 8, 4, 2);
  if(plotX){
     cpgsci(BLUE);
     addline(nDataPoints, fXraw, fYDeriv3, BLUE);
     spacebar();
  }
  /*
   * Perform Kramer-Kronig transform
   */
  Integrate(nDataPoints, &nPoints, fEdge, fXraw, fXfpp, fYspline, fYfpps, fYDeriv1, fYDeriv2, fYDeriv3, fYfp);
  if(plotX)
     addline(nPoints, fXfpp, fYspline, BLUE);
  /*
   * Plot resulting f' and f'' spectra
   */
  err=efswrite("out.efs", fXfpp, fYspline, fYfp, nPoints);
  if(plotX){
     efsplot(nPoints, fXfpp, fYspline, fYfp);
     spacebar();
  }
}











