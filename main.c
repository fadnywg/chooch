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
  int nDataPoints, nFit, nPoints;
  double dE, tmp;
  double fXraw[MAXSIZE], fYraw[MAXSIZE];
  double fYspline[MAXSIZE], fXfpp[MAXSIZE];
  double fYsmooth[MAXSIZE], fYnorm[MAXSIZE], fYfpp[MAXSIZE], fYfp[MAXSIZE];
  double fYDeriv1[MAXSIZE], fYDeriv2[MAXSIZE], fYDeriv3[MAXSIZE];
  double C[3], result, error;
  //
  double SeK = 12665.0;
  double fC, fM;
  //
  optarg = NULL;
  while( ( opt = getopt( argc, argv, "h:a:e:o:" ) ) != (char)(-1) )
    switch( opt ) {
    case 'h' :
      (void)usage();
      exit(EXIT_SUCCESS);
      break;
    case 'a' :
      sElement = optarg;
      break;
    case 'e' :
      sEdge = optarg;
      break;
    case 'o' :
      break;
    }

  if(argc < 2){
    printf("\nUsage: gexen <filename>\n");
    exit(EXIT_FAILURE);
  }
  sFilename = argv[optind];
  printf("Fluorescence scan filename: %s\n", sFilename);
  printf("Element: %s    Absorption Edge: %s\n", sElement, sEdge);
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
  err=normalize(nDataPoints, fXraw, fYsmooth, fYnorm);
  /*
  strcpy(label,"Normalized spectrum");
  toplot(nDataPoints, fXraw, fYnorm, label, RED);
  */
      
  /*
   * Impose on theoretical spectrum of f'' to obtain 
   * experimental equivalent
   */
  err=impose(nDataPoints, fXraw, fYnorm, fYfpp);
  strcpy(label,"f''");
  toplot(nDataPoints, fXraw, fYfpp, label, RED);
  /*
   * Determine first and second derivatives of smoothed data
   * and plot them on top of one another.
   */
  strcpy(label,"Fit and derivatives");
  err = smooth(nDataPoints, fYfpp, fYsmooth, 8, 8, 4, 0);
  toplot(nDataPoints, fXraw, fYsmooth, label, RED);
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
  Integrate(nDataPoints, &nPoints, fXraw, fXfpp, fYspline, fYsmooth, fYDeriv1, fYDeriv2, fYDeriv3, fYfp);
  addline(nPoints, fXfpp, fYspline, BLUE);

  /*
  for(i=0; i<nPoints; i++){
    printf("%f %f %f\n", fXfpp[i], fYspline[i], fYfp[i]);
  }
  */

  /*
   * Plot resulting f' and f'' spectra
   */
  efsplot(nPoints, fXfpp, fYspline, fYfp);
  
}











