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
  char  *sEdge="L";           // Letter symbol for absorption edge K, L1, L2, L3, M
  char opt;
  //
  int nDataPoints, nFit, nPoints, verbose;
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
  while( ( opt = getopt( argc, argv, "h:e:a:o:l:v:" ) ) != (char)(-1) )
/*   while (--argc > 0 && (*++argv)[0] == '-') */
/*      while (c = *++argv[0]) */
     switch( opt ) {
     case 'h' :
	(void)usage();
	exit(EXIT_SUCCESS);
	break;
     case 'e' :
	sElement = optarg;
	break;
     case 'v' :
	verbose = 1;
	break;
     case 'a' :
	sEdge = optarg;
	break;
     case 'l' :
	fE1 = atof(optarg);
	printf("Fit below edge from %f\n", fE1);
	break;
     }
  
  if(argc < 2){
    printf("\nUsage: chooch <filename>\n");
    exit(EXIT_FAILURE);
  }
  sFilename = argv[optind];
  printf("Fluorescence scan filename: %s\n", sFilename);
  //
  cpgbeg(0, "/xw", 1, 1);
  //
  printbanner();
  //
  /*
   * Read in raw spectrum and plot
   */
  fluread(sFilename, fXraw, fYraw, &nDataPoints);
  err = checks(nDataPoints, fXraw, fYraw, &dE);
  strcpy(label,"Raw data");
  fMid=(fXraw[nDataPoints-1]+fXraw[0])/2.0;
  printf("Mid point of spectrum = %f\n", fMid);
  sEdge=get_Edge(sElement, fMid, &fEdge);
  printf("\nSpectrum over %s %s edge at theoretocal energy of %8.2f eV\n", sElement, sEdge, fEdge);
  toplot(nDataPoints, fXraw, fYraw, label, YELLOW);
  //  for (i = 0; i < nDataPoints; i++)
  //    fXpoint[i] = (double) i;
  /*
   * Smooth with Savitzky-Golay filter
   */ 
  err = smooth(nDataPoints, fYraw, fYsmooth, 8, 8, 4, 0);
  //  strcpy(label,"Result");  
  //  cpgsci(RED);
  //  addline(nDataPoints, fXraw, fYsmooth, RED);
  /*
   * Normalise data
   */
  err=normalize(nDataPoints, fEdge, fXraw, fYraw, fYnorm);
  /*
  strcpy(label,"Normalized spectrum");
  toplot(nDataPoints, fXraw, fYnorm, label, RED);
  */
      
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
  strcpy(label,"Fit and derivatives");
  err = smooth(nDataPoints, fYfpp, fYfpps, 8, 8, 4, 0); 
  strcpy(label,"f''");
  printf("Press <SPACE> to continue\n");
  cpgband(0, 0, fXref, fYref, &fXcur, &fYcur, ch);
  toplot(nDataPoints, fXraw, fYfpps, label, RED);
  //  toplot(nDataPoints, fXraw, fYfpp, label, RED);
  err = smooth(nDataPoints, fYfpp, fYDeriv1, 8, 8, 4, 1);
  strcpy(label,"1st and 2nd derivative");  
  addline(nDataPoints, fXraw, fYDeriv1, YELLOW);
  err = smooth(nDataPoints, fYfpp, fYDeriv2, 8, 8, 4, 2);
  cpgsci(BLUE);
  addline(nDataPoints, fXraw, fYDeriv2, BLUE);
  err = smooth(nDataPoints, fYfpp, fYDeriv3, 8, 8, 4, 2);
  cpgsci(BLUE);
  addline(nDataPoints, fXraw, fYDeriv3, BLUE);
  /*
   * Perform Kramer-Kronig transform
   */
  printf("Press <SPACE> to continue\n");
  cpgband(0, 0, fXref, fYref, &fXcur, &fYcur, ch);
  Integrate(nDataPoints, &nPoints, fEdge, fXraw, fXfpp, fYspline, fYfpps, fYDeriv1, fYDeriv2, fYDeriv3, fYfp);
  addline(nPoints, fXfpp, fYspline, BLUE);

  /*
  for(i=0; i<nPoints; i++){
    printf("%f %f %f\n", fXfpp[i], fYspline[i], fYfp[i]);
  }
  */

  /*
   * Plot resulting f' and f'' spectra
   */
  err=efswrite("out.efs", fXfpp, fYspline, fYfp, nPoints);
  efsplot(nPoints, fXfpp, fYspline, fYfp);

}











