/***************************************************************************
                          main.c  -  chooch main source file
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
char cScanTitle[TITLE]="";
int id1=0, id2=0;
int verbose, silent;
double fpInfl, fppInfl, fpPeak, fppPeak, EInfl, EPeak;
double fE1=0.0, fE2=0.0, fE3=0.0, fE4=0.0;
double fEres=0.00014;
/*
 */
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
  int nDataPoints, nFit, nPoints, plotX=0, psplot=0, display=0;
  int nSavWin;
  double dE, tmp, fEdge, fMonoRes;
  double fXraw[MAXSIZE], fYraw[MAXSIZE];
  double fYfita[MAXSIZE], fYfitb[MAXSIZE];
  double fYspline[MAXSIZE], fXfpp[MAXSIZE];
  double fYsmooth[MAXSIZE], fYnorm[MAXSIZE];
  double fYfpp[MAXSIZE], fYfpps[MAXSIZE], fYfp[MAXSIZE];
  double fYDeriv1[MAXSIZE], fYDeriv2[MAXSIZE], fYDeriv3[MAXSIZE];
  double C[3], result, error, fMid;
  //
  double fC, fM;
  FILE *ff;
  //
  verbose=silent=0;
  optarg = NULL;
  while((opt = getopt(argc, argv, "she:a:r:xo:p:v:1:2:3:4:d")) != (char)(-1))
     switch( opt ) {
     case 's' :
	silent = 1;
	break;
     case 'h' :
	(void)usage();
	exit(EXIT_SUCCESS);
	break;
     case 'e' :
	sElement = optarg;
	if(!silent)printf("-e     Atomic element = %s\n", sElement);
	break;
     case 'a' :
	sEdge = optarg;
	if(!silent)printf("-a:   Absorption edge = %s\n", sEdge);
	break;
     case 'r' :
	fEres = atof(optarg);
	if(!silent)printf("-r: Energy resolution = %f\n", fEres);
	break;	
#if defined(PGPLOT)
     case 'x' :	
	plotX = 1;
	if(!silent)printf("-x: with X plotting \n");
	break;
#endif
     case 'o' :	
	outfile = optarg;
	if(!silent)printf("-o: Output file name = %s\n", outfile);
	break;
     case 'p' :	
	psplot = 1;
	psfile = optarg;
	if(!silent)printf("-p: PS output file = %s\n", psfile);
	break;
     case 'v' :
	verbose = atoi(optarg);
	if(!silent)printf("-v: Verbosity level %d", verbose);
	break;
     case '1' :
	fE1 = atof(optarg);
	if(!silent)printf("-1: Below edge fit lower limit = %f\n", fE1);
	break;
     case '2' :
	fE2 = atof(optarg);
	if(!silent)printf("-2: Below edge fit upper limit = %f\n", fE2);
	break;
     case '3' :
	fE3 = atof(optarg);
	if(!silent)printf("-3: Above edge fit lower limit = %f\n", fE3);
	break;
     case '4' :
	fE4 = atof(optarg);
	if(!silent)printf("-4: Above edge fit upper limit = %f\n", fE4);
	break;
     case 'd' :
	display = 1;
	if(!silent)printf("-d: Dump data file for pooch\n", fE4);
	break;
     }  
  if(argc < 2){
    printf("Usage: chooch -e <element> -a <edge>\n");
    printf("Try chooch -h to show all options\n");
    exit(EXIT_FAILURE);
  }
  copyright();
  sFilename = argv[optind];
  if(!silent)printf("Fluorescence scan filename: %s\n", sFilename);
  //
#if defined(PGPLOT)
  if(plotX){
     id1=cpgopen("/xw");
     if(id1 < 0)exit (EXIT_FAILURE);
     cpgslct(id1);
  }
#endif
  //printbanner();
  /*
   * Read in raw spectrum and plot
   */
  fluread(sFilename, fXraw, fYraw, &nDataPoints);
  err=checks(nDataPoints, fXraw, fYraw, &dE);
  fMid=(fXraw[nDataPoints-1]+fXraw[0])/2.0;
  sEdge=get_Edge(sElement, fMid, &fEdge);
  if(!silent)printf("\nSpectrum over %s %s edge at theoretical energy of %8.2f eV\n", sElement, sEdge, fEdge);
  /*
   * Smooth with Savitzky-Golay filter
   */ 
  fMonoRes = fEres * fEdge;
  nSavWin = 2 * (int) fMonoRes / dE;
  if(verbose>0)printf("dE = %f Resol = %f\n", dE, fMonoRes);
  if(verbose>0)printf("Savitsky-Golay window value = %d\n", nSavWin);
  /*
  err = smooth(nDataPoints, fYraw, fYsmooth, nSavWin, nSavWin, 4, 0);
  */
#if defined(PGPLOT)
  if(plotX){
     toplot(nDataPoints, fXraw, fYraw, "Raw and smoothed data", YELLOW);
     /*
       addline(nDataPoints, fXraw, fYsmooth, BLUE);
     */
  }
#endif
  /*
   * Normalise data
   */
  err=normalize(nDataPoints, fEdge, fXraw, fYraw, fYnorm, plotX, fYfita, fYfitb);
#if defined(PGPLOT)
  if(plotX)spacebar();
#endif
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
  err = smooth(nDataPoints, fYfpp, fYfpps, nSavWin, nSavWin, 4, 0);
#if defined(PGPLOT)
  if(plotX)
     toplot(nDataPoints, fXraw, fYfpps, "f'' and derivatives", RED);
#endif
  err = smooth(nDataPoints, fYfpp, fYDeriv1, nSavWin, nSavWin, 4, 1);
#if defined(PGPLOT)
  if(plotX)
     addline(nDataPoints, fXraw, fYDeriv1, YELLOW);
#endif
  err = smooth(nDataPoints, fYfpp, fYDeriv2, nSavWin, nSavWin, 4, 2);
#if defined(PGPLOT)
  if(plotX){
     cpgsci(BLUE);
     addline(nDataPoints, fXraw, fYDeriv2, BLUE);
  }
#endif
  err = smooth(nDataPoints, fYfpp, fYDeriv3, nSavWin, nSavWin, 4, 2);
#if defined(PGPLOT)
  if(plotX){
     cpgsci(BLUE);
     addline(nDataPoints, fXraw, fYDeriv3, BLUE);
     spacebar();
  }
#endif
  /*
   * Perform Kramer-Kronig transform
   */
  Integrate(nDataPoints, &nPoints, fEdge, fXraw, fXfpp, fYspline, fYfpps, fYDeriv1, fYDeriv2, fYDeriv3, fYfp);
  err=selwavel(nPoints, fXfpp, fYspline, fYfp);

#if defined(PGPLOT)
  if(plotX) {
     addline(nPoints, fXfpp, fYspline, GREEN);
     spacebar();
  }
#endif
  /*
   * Plot resulting f' and f'' spectra
   */
  err=efswrite(outfile, fXfpp, fYspline, fYfp, nPoints);
#if defined(PGPLOT)
  if(plotX) {
     efsplot(nPoints, fXfpp, fYspline, fYfp, 0, NULL);
     spacebar();
  }
#endif
  if(psplot){
     /*     efsplot(nPoints, fXfpp, fYspline, fYfp, 1, psfile);*/
     psplt(nPoints, fXfpp, fYspline, fYfp, psfile);
  }
  /*
   *
   */
  if(display){
     if ((ff = fopen(".pooch", "w")) == NULL) {
	printf("Cannot open .pooch for write\n");
	exit(EXIT_FAILURE);
     }
     for (i = 0; i < nDataPoints; i++) {
	fprintf(ff,"%10.3f %7.2f %7.2f %7.2f %7.3f %7.3f %7.3f %7.3f\n", fXraw[i], fYraw[i], fYsmooth[i], fYfita[i], fYfitb[i], fYDeriv1[i], fYDeriv2[i], fYDeriv3[i]);
     }
     fclose(ff);
  }
  if(!silent){
     printf("\n Table of results\n");
     printf("------------------------------------\n");
     printf("|      |  energy  |    f\'\' |   f\'  |\n");
     printf("| peak | %8.2f |  %5.2f | %5.2f |\n", EPeak, fppPeak, fpPeak);
     printf("| infl | %8.2f |  %5.2f | %5.2f |\n", EInfl, fppInfl, fpInfl);
     printf("------------------------------------\n");
  }
}
