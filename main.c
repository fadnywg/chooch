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
int id1=0, id2=0;
int verbose;
double fE1=0.0, fE2=0.0, fE3=0.0, fE4=0.0;
double fEres=0.00014;
//typedef struct{double d1; double d2; double d3;} deriv; 
//
//
int main(int argc, char *argv[])
{
  int i, j, err;
  float fXref, fYref, fXcur, fYcur;
  char *sFilename;
  char *psfile, *outfile="output.efs";
  char  ch[1];
  char  *sEdge="K";           // Letter symbol for absorption edge K, L1, L2, L3, M
  char opt;
  //
  int nDataPoints, nFit, nPoints, plotX=0, psplot=0;
  double dE, tmp, fEdge;
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
  verbose = 0;
  optarg = NULL;
  while( ( opt = getopt( argc, argv, "he:a:r:xo:p:v:1:2:3:4:" ) ) != (char)(-1) )
     switch( opt ) {
     case 'h' :
	(void)usage();
	exit(EXIT_SUCCESS);
	break;
     case 'e' :
	sElement = optarg;
	printf("-e     Atomic element = %s\n", sElement);
	break;
     case 'a' :
	sEdge = optarg;
	printf("-a:   Absorption edge = %s\n", sEdge);
	break;
     case 'r' :
	fEres = atof(optarg);
	printf("-r: Energy resolution = %f\n", fEres);
	break;	
     case 'x' :	
	plotX = 1;
	printf("-x: with X plotting \n");
	break;
     case 'o' :	
	outfile = optarg;
	printf("-o:  Output file name = %s\n", outfile);
	break;
     case 'p' :	
	psplot = 1;
	psfile = optarg;
	printf("-p:    PS output file = %s\n", psfile);
	break;
     case 'v' :
	verbose = atoi(optarg);
	printf("Verbosity level %d", verbose);
	break;
     case '1' :
	fE1 = atof(optarg);
	printf("Below edge fit lower limit = %f\n", fE1);
	break;
     case '2' :
	fE2 = atof(optarg);
	printf("Below edge fit upper limit = %f\n", fE2);
	break;
     case '3' :
	fE3 = atof(optarg);
	printf("Above edge fit lower limit = %f\n", fE3);
	break;
     case '4' :
	fE4 = atof(optarg);
	printf("Above edge fit upper limit = %f\n", fE4);
	break;
     }  
  if(argc < 2){
    printf("Usage: chooch -e <element> -a <edge>\n");
    printf("Try chooch -h to show all options\n");
    exit(EXIT_FAILURE);
  }
  sFilename = argv[optind];
  printf("Fluorescence scan filename: %s\n", sFilename);
  //
  if(plotX){
     id1=cpgopen("/xw");
     if(id1 < 0)exit (EXIT_FAILURE);
     cpgslct(id1);
  }
  //printbanner();
  /*
   * Read in raw spectrum and plot
   */
  fluread(sFilename, fXraw, fYraw, &nDataPoints);
  err=checks(nDataPoints, fXraw, fYraw, &dE);
  fMid=(fXraw[nDataPoints-1]+fXraw[0])/2.0;
  sEdge=get_Edge(sElement, fMid, &fEdge);
  printf("\nSpectrum over %s %s edge at theoretical energy of %8.2f eV\n", sElement, sEdge, fEdge);
  /*
   * Smooth with Savitzky-Golay filter
   */ 
  err = smooth(nDataPoints, fYraw, fYsmooth, 8, 8, 4, 0);
  //  cpgsci(RED);
  if(plotX){
     toplot(nDataPoints, fXraw, fYraw, "Raw and smoothed data", YELLOW);
     addline(nDataPoints, fXraw, fYsmooth, RED);
  }
  /*
   * Normalise data
   */
  err=normalize(nDataPoints, fEdge, fXraw, fYraw, fYnorm, plotX);
  if(plotX)spacebar();
  /*
   * Impose on theoretical spectrum of f'' to obtain 
   * experimental equivalent
   */
  if(verbose>0)printf(" Converting spectrum to f''\n");
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
  err=efswrite(outfile, fXfpp, fYspline, fYfp, nPoints);
  if(plotX){
     efsplot(nPoints, fXfpp, fYspline, fYfp, 0, NULL);
     spacebar();
  }
  if(psplot){
     efsplot(nPoints, fXfpp, fYspline, fYfp, 1, psfile);
  }
}
