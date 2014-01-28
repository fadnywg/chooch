/***************************************************************************
                          parse.c  -  parse command line switches
                : Fri Nov 30 17:26:00 GMT 2012
    copyright            : (C) 2012 by Gwyndaf Evans
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
//
#include "chooch.h"
//
void parse(int argc, char *argv[])
{
  extern int verbose, status, silent, kev, aqt, raddose, getefs;
  extern int plotX, psplot, pngplot, display;
  extern char *sElement, *sEdge;
  extern char *psfile, *pngfile, *outfile;
  extern double fE1, fE2, fE3, fE4;
  extern double fEres;
  extern double RemE;
  char opt;

  optarg = NULL;
  while((opt = getopt(argc, argv, "she:a:r:kxo:p:g:v:1:2:3:4:idwclzf:")) != (char)(-1))
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
	if(!silent)printf("-e: Atomic element = %s\n", sElement);
	break;
     case 'a' :
        sEdge = optarg;
        if(!silent)printf("-a: Absorption edge entered but will be auto-determined anyway\n");
        break;
     case 'r' :
	fEres = atof(optarg);
	if(!silent)printf("-r: Energy resolution = %f\n", fEres);
	break;
     case 'k' :
        kev = 1;
	if(!silent)printf("-k: Input data will be converted from keV to eV\n");
        break;
     case 'x' :	
	plotX = 1;
	if(!silent)printf("-x: display graphics window\n");
	break;
     case 'o' :	
	outfile = optarg;
	if(!silent)printf("-o: Output file name = %s\n", outfile);
	break;
     case 'i' :	
	aqt = 1;
	if(!silent)printf("-i: plot in window\n");
	break;
     case 'p' :	
	psplot = 1;
	psfile = optarg;
	if(!silent)printf("-p: PS output file = %s\n", psfile);
	break;
     case 'g' :	
	pngplot = 1;
	pngfile = optarg;
	if(!silent)printf("-g: PNG output file = %s\n", pngfile);
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
	if(!silent)printf("-d: Dump data file for pooch\n");
	break;
     case 'w' :
	(void)nowarranty();
	exit(EXIT_SUCCESS);
	break;
     case 'c' :
	(void)distribution();
	exit(EXIT_SUCCESS);
	break;
     case 'l' :
	(void)license();
	exit(EXIT_SUCCESS);
	break;
     case 'z' :
        raddose=1;
	if(!silent)printf("-z: Output splinor file for raddose\n");
	break;
	/*
	case 'f' :
        getefs=1;
	RemE = atof(optarg);
	if(!silent)printf("-f: return anom. scattering factors for %f\n", RemE);
	break;
	*/
     }  
  if(argc < 3){
    printf("Usage: chooch -e <element> <filename>\n");
    printf("Try chooch -h to show all options\n");
    exit(EXIT_FAILURE);
  }
}
