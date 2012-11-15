/***************************************************************************
                             usage.c  -  prints usage opions
                             -------------------
    begin                : Sat July  6 12:01:02 GMT 2002
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
#include <stdlib.h>
#include <stdio.h>
#include "chooch.h"
void usage()
{
  (void)fprintf( stderr, "Usage:\n" );
  (void)fprintf( stderr, " chooch -e <element> [-h] [-s] [-r <resol>] [-k] [-1 <e1>] [-2 <e2>] [-3 <e3>] [-4 <e4>] [-x] [-p ps filename] [-g png filename] [-o <efs filename>] [-z] [-v <verbose level>] [-w] [-c] [-l] [<inputfile>]\n\n");
  (void)fprintf( stderr, "-h                    print this message\n");
  (void)fprintf( stderr, "-s                    run silently\n");
  (void)fprintf( stderr, "-e <element>          element symbol (compulsory input!)\n");
  (void)fprintf( stderr, "-a <edge>             absorption edge (K, L1, L2, L3, M) (this is automatically determined for you)\n");
  (void)fprintf( stderr, "-r <resol>            energy resolution (dE/E) (default is Si(111) 1.4x10-4)\n");
  (void)fprintf( stderr, "-k                    Input data will be converted from keV to eV\n");
  (void)fprintf( stderr, "-1 <e1>               Below edge fit lower energy limit (eV)\n");
  (void)fprintf( stderr, "-2 <e2>               Below edge fit upper energy limit (eV)\n");
  (void)fprintf( stderr, "-3 <e3>               Above edge fit lower energy limit (eV)\n");
  (void)fprintf( stderr, "-4 <e4>               Above edge fit upper energy limit (eV)\n");
  (void)fprintf( stderr, "-p <PS file>          output to PostScript file\n");
  (void)fprintf( stderr, "-g <PNG file>         output to Portable Network Graphics (PNG) file\n");
  (void)fprintf( stderr, "-o <efs file>         filename for efs output (default output.efs)\n");
#if defined(PGPLOT)
  (void)fprintf( stderr, "-x                    plot to Xwindows\n");
#endif
  (void)fprintf( stderr, "-d                    dump intermediate data for use with pUI\n");
  (void)fprintf( stderr, "-z                    output splinor file for RADDOSE\n");
  (void)fprintf( stderr, "-v <level>            verbosity level (0 -- 3) (default 0)\n");
  (void)fprintf( stderr, "-w                    show warranty information\n");
  (void)fprintf( stderr, "-c                    show redistribution information\n");
  (void)fprintf( stderr, "-l                    show license information\n");
  exit( EXIT_FAILURE );
}
