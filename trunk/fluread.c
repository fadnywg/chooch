/***************************************************************************
                          fluread.c  -  read and write data files
                : Sat Mar  9 09:51:02 GMT 2002
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
//
#include "chooch.h"
//

int fluread(char *filename, double *x, double *y, int *nDataPoints)
{
  extern int verbose, silent;
  extern char cScanTitle[TITLE];
  int    i,n;
/*  int    len; removed for v5.0.8*/
  float  fXread[MAXSIZE], fYread[MAXSIZE];
  char   line[TITLE], cLine[TITLE];
  FILE   *ff;
  //
  if ((ff = fopen(filename, "r")) == NULL) {
     printf("Error: File %s doesn't exist.\n", filename);
     exit(EXIT_FAILURE);
  }
  /*
   * Read in header of raw fluorescence data file
   */ 
  i=0;
  while (fgets(line, sizeof(line), ff) != NULL) {
     if ((n=sscanf(line, "%f%f\n", &fXread[i], &fYread[i])) == 2) {
	x[i] = (double) fXread[i];
	y[i] = (double) fYread[i];
       	if(verbose>1)printf("%10.3f  %10.3f\n", x[i], y[i]);
	i++;
	/* Read header lines if present
	 * Will always try and read two lines if the first is a title
	 */
     } else if (sscanf(line, "%80c", cLine) > 0) {

       /* The next check gives a warning and not an error because it
	  was unneccesary I thought. The analysis will still work if
	  you put a blank line in the file somewhere. As long as the
	  data is OK then who cares if there are inserted lines
	  between the points.
       */
	if(i != 0){
	  printf("WARNING: Inserted lines of text in data file\n");
	  printf("         (maybe blank lines at end of file)\n\n");
	}
	if(i == 0){
	  strcpy(cScanTitle,cLine);
	  if(!silent)printf("Input file title: %s\n", cScanTitle);
	  if(fgets(line, sizeof(line), ff) != NULL){
	    if ((n=sscanf(line, "%d", nDataPoints)) == 1) {
	      if(!silent)printf("Number of data points expected = %d\n", *nDataPoints);
	    }
	  }
	}
     }
  }
  if(!silent)printf("Number of data points read = %d\n", i);
  if (*nDataPoints != i) {
     printf("Warning: No. data points expected from header does not equal number\n");
     printf("         present in file or has not been specified in header\n");
     *nDataPoints=i;
     if(!silent)printf("Number of actual data points is %d\n", *nDataPoints);
  }
  if (strlen(cScanTitle) == 0){
     if(!silent)printf("Warning: No title supplied so using data filename instead\n");
     strcpy(cScanTitle,filename);
     if(!silent)printf("Input file title: %s\n", cScanTitle);
  }
  fclose(ff);
  return EXIT_SUCCESS;
}

int efswrite(char *filename, double *x, double *y1, double *y2, int n)
{
  extern int verbose, silent;
  extern char cScanTitle[TITLE];
  int    i;
  FILE   *ff;
  //
  if ((ff = fopen(filename, "w")) == NULL) {
     printf("Cannot open %s for write\n", filename);
     exit(EXIT_FAILURE);
  }

  //  printf("Title: %s\nNo. data points: %d\n", cScanTitle, *nDataPoints);
  if(!silent)printf("Writing anomalous scattering factors to %s\n", filename);
  for (i = 0; i < n; i++) {
     fprintf(ff, "%10.4f  %7.2f  %7.2f\n", x[i], y1[i], y2[i]);
     if(verbose>0)printf("%10.4f  %7.2f  %7.2f\n", x[i], y1[i], y2[i]);
     //    printf("%10.3f  %10.3f\n", x[i], y[i]);
  }
  fclose(ff);
  return EXIT_SUCCESS;
}



