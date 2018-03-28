/***************************************************************************\
 *                        chooch.c  -  chooch main source file             *
 *                           -------------------                           *
 *  begin                : Sat Mar  9 09:51:02 GMT 2002                    *
 *  copyright            : (C) 1994 - 2012 by Gwyndaf Evans                *
 *  email                : gwyndaf@gwyndafevans.co.uk                      *
\***************************************************************************/

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
#include <gsl/gsl_errno.h>
#include <plplot.h>
//
#include "chooch.h"
int c;
char  *sElement="Se";           // Letter symbol for element name e.g. Au, Se, I
// Initialise sEdge as "X" so we know nothing has been set by user
char  *sEdge="X";           // Letter symbol for absorption edge K, L1, L2, L3, M
char cScanTitle[TITLE]="", device[6]="xwin";
char *psfile="", *pngfile="", *outfile="output.efs";
int id1=0, id2=0;
int verbose, status, silent, kev, aqt, raddose, getefs;
int plotX=0, psplot=0, display=0;
double fpInfl, fppInfl, fpPeak, fppPeak, EInfl, EPeak;
double fE1=0.0, fE2=0.0, fE3=0.0, fE4=0.0;
double fEres=0.00014;
double RemE;
/*
 */
int main(int argc, char *argv[])
{
  int i, err;
/*  float fXref, fYref, fXcur, fYcur; removed for v5.0.8*/
  char *sFilename;
/*  char  ch[1]; removed for v5.0.8*/
  //
  int nDataPoints, nPoints;
  int nSavWin;
  double dE, fEdge; /* fMonoRes removed for v5.0.8 */
  double fXraw[MAXSIZE], fYraw[MAXSIZE]; // Input data
  double fYfita[MAXSIZE], fYfitb[MAXSIZE]; // Output
  double fYspline[MAXSIZE], fXfpp[MAXSIZE]; // Output values of Energy and fpp
  double fYsmooth[MAXSIZE], fYnorm[MAXSIZE]; // Smoothed and normalised fluorescence spectra
  double fYfpp[MAXSIZE], fYfpps[MAXSIZE], fYfp[MAXSIZE]; // Raw f'', smoothed f''
  double fYDeriv1[MAXSIZE], fYDeriv2[MAXSIZE], fYDeriv3[MAXSIZE];
  double fMid;
  //
  FILE *ff, *fr;
  //
  gsl_set_error_handler_off();

  verbose=silent=kev=aqt=raddose=getefs=0;
  /***************************************************
   * Output author, copyright and license information
   ***************************************************/
  if(!silent)copyright();
  if(!silent)license();

  /******************************
   * Parse command-line switches
   ******************************/
  
  parse(argc, argv);
  
  sFilename = argv[optind];
  if(!silent)printf("Fluorescence scan filename: %s\n", sFilename);

  /********************************
   * Start output and calculations
   ********************************/

 (void)fprintf( stderr, "Chooch output\n");
 (void)fprintf( stderr, "-------------\n");

  /* 
   *
   * If -x option used 
   * Initialise PLplot xwin output for linux and OSX
   * It happens that they are the same but there is also a
   * possibility to use Aquaterm for OSX - option "aqt" 
   *
   */
  if(plotX){
#if defined(i386)
    sprintf(device, "xwin");
    printf("PLplot device is %s\n", device);
#elif defined(x86_64)
    sprintf(device, "xwin");
    printf("PLplot device is %s\n", device);
#endif
    plsdev(device);
    plscol0(0, 255,255,255); // Set background color to White
    plscol0(1, 0, 0, 0); // Set foreground color to Black
    plinit();
    plfont(1); // Simple=1; Roman=2; Italic=3; Script=4
    pladv(0); // Increment subpage number
  }

  //printbanner();

  if(getefs){
	  printf("Returning f\' and f\'\' for .....");
  }


  /********************************
   * Read in raw spectrum and plot
   ********************************/

  fluread(sFilename, fXraw, fYraw, &nDataPoints);

  /*
   * Check input data for common errors
   */

  err=checks(nDataPoints, fXraw, fYraw, &dE);

  /* 
   * Auto-detect edge
   */
  printf("No edge specified so working it out automatically ... \n");
  fMid=(fXraw[nDataPoints-1]+fXraw[0])/2.0;
  sEdge=get_Edge(sElement, fMid, &fEdge);
  if(!silent)printf("\nSpectrum over %s %s edge at theoretical energy of %8.2f eV\n", sElement, sEdge, fEdge);
  
  /**********************************
   * Determine Savitzky-Golay window
   **********************************/ 

  savwin(fEres, fEdge, dE, &nSavWin);

  if(plotX){
    toplot(nDataPoints, fXraw, fYraw, "Raw data", RED); //actually BLACK
  }


  /******************
   * Normalise data
   ******************/
  err=normalize(nDataPoints, fEdge, fXraw, fYraw, fYnorm, plotX, fYfita, fYfitb);
  if(plotX)spacebar();

  /**************************************************
   * Impose on theoretical spectrum of f'' to obtain 
   * experimental equivalent
   **************************************************/
  if(verbose>0)printf(" Converting spectrum to f''\n");
  err=impose(nDataPoints, fEdge, fXraw, fYnorm, fYfpp);

  /*************************************************************************
   * Determine zeroth, first, second and third derivatives of smoothed data
   * and plot them on top of one another.
   *************************************************************************/
  err = smooth(nDataPoints, fYfpp, fYfpps, nSavWin, nSavWin, 4, 0);
  if(plotX){
    toplot(nDataPoints, fXraw, fYfpp, "f\'\' and derivatives", BLUE);
    addline(nDataPoints, fXraw, fYfpps, BROWN);
  }
  err = smooth(nDataPoints, fYfpp, fYDeriv1, nSavWin, nSavWin, 4, 1);

  if(plotX){
     addline(nDataPoints, fXraw, fYDeriv1, YELLOW);
  }
  err = smooth(nDataPoints, fYfpp, fYDeriv2, nSavWin, nSavWin, 4, 2);

  if(plotX){
    addline(nDataPoints, fXraw, fYDeriv2, MAGENTA);
  }
  err = smooth(nDataPoints, fYfpp, fYDeriv3, nSavWin, nSavWin, 4, 3);

  if(plotX){
    addline(nDataPoints, fXraw, fYDeriv3, BLUE);
    spacebar();
  }

  if(verbose>2){
    for(i=0; i<nDataPoints; i++){
      printf("%f  %f  %f  %f \n", fYfpp[i], fYDeriv1[i], fYDeriv2[i], fYDeriv3[i]);
    }
  }

  /**********************************
   * Perform Kramer-Kronig transform
   **********************************/
  Integrate(nDataPoints, &nPoints, fEdge, fXraw, fXfpp, fYspline, fYfpps, fYDeriv1, fYDeriv2, fYDeriv3, fYfp);
  err=selwavel(nPoints, fXfpp, fYspline, fYfp);
  
  if(verbose>0){
    for(i=0;i<nPoints;i++){
      printf("i fXfpp fYspline  %d %10.5f  %10.5f \n", i, fXfpp[i], fYspline[i]);
    }
  }
  if(plotX) {
     addline(nPoints, fXfpp, fYspline, RED);
     spacebar();
     plend();
  }

  /*************************************
   * OUTPUT RESULTS (f' and f'' spectra)
   *************************************/

  /* To ascii file */
  err=efswrite(outfile, fXfpp, fYspline, fYfp, nPoints);

  if(plotX) {
    plpng(nPoints, fXfpp, fYspline, fYfp, "", device, 0);
  }

  /* To PostScript file if requested */
  if(psplot){
    if(!silent)printf("Writing PostScript file %s\n", psfile);
    plpng(nPoints, fXfpp, fYspline, fYfp, psfile, "ps", 1);
  }

  /* To PNG file if requested
  if(pngplot){
    if(!silent)printf("Writing PNG file %s\n", pngfile);
    plpng(nPoints, fXfpp, fYspline, fYfp, pngfile, "png", 0);
  }
  */
  /* output to AquaTerm window on Mac if -i option is invoked */
  if(aqt){
    plpng(nPoints, fXfpp, fYspline, fYfp, "", "aqt", 0);
  }

  /***************************************
   * Stuff for Python GUI when it arrives
   ***************************************/

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

  /***************************************************
   * Outputting SPLINOR style file for use in RADDOSE
   ***************************************************/

  /* Old file used to contain the 1st, 2nd and 3rd derivatives of the normalised curve
   * This turned out to lead to small errors in the calculations because the derivative should have
   * been of the f'' values. In any case I have padded the new splinor file out with three columns of
   * zeros so that the format is consistent with the old file.
   * The new file is now of the form
   *      Energy      Normalised Val.              Padding
   * 1234567890123456789012345678901234567890123456789012345678901234567890
   *start
   *     12620.000        0.006        0.000        0.000        0.000
   *     12622.000       -0.002        0.000        0.000        0.000
   *     12623.000        0.011        0.000        0.000        0.000
   * ...
   * end
   */

  if(raddose){
	  if(!silent)printf("\nWriting splinor file for use with RADDOSE ...");
	  if ((fr = fopen("splinor", "w")) == NULL) {
		  printf("Cannot open splinor for write\n");
		  exit(EXIT_FAILURE);
	  }
	  for (i = 0; i < nDataPoints; i++) {
    	 fprintf(fr,"%13.3f %12.3f %12.3f %12.3f %12.3f\n", fXraw[i], fYnorm[i], 0.0, 0.0, 0.0);
	  }
	  if(!silent)printf(" ..... Done\n\n");
	  fclose(fr);
  }

  /******************************
   * Print text table of results
   ******************************/

  if(!silent){
     printf("\n Table of results\n");
     printf("------------------------------------\n");
     printf("|      |  energy  |    f\'\' |   f\'  |\n");
     printf("| peak | %8.2f |  %5.2f | %5.2f |\n", EPeak, fppPeak, fpPeak);
     printf("| infl | %8.2f |  %5.2f | %5.2f |\n", EInfl, fppInfl, fpInfl);
     printf("------------------------------------\n");
  }
  return(0);
}
