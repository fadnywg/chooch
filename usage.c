#include <stdio.h>
#include "chooch.h"
//[-e <element>] [-c \"<cell dimensions>\"] [-I <beam intensity>] [-n <number heavy atoms>] [-v <sample volume>] 
//[-t <sample thickness>] [-E <X-ray energy>] [-m <heavy atom conc.>] [-O <crystal ang. velocity>] [-D <detector distance>]
void usage()
{
  (void)fprintf( stderr, "Usage:\n" );
  (void)fprintf( stderr, " chooch -e <element> [-a <edge>] [-r <resol>] [-1 <e1>] [-2 <e2>] [-3 <e3>] [-4 <e4>] [-x] [-p] [-o <efs filename>] [-v <verbose level>] datafile\n\n");
  (void)fprintf( stderr, "-h                    print this message\n");
  (void)fprintf( stderr, "-e <element>          element symbol (default Se)\n");
  (void)fprintf( stderr, "-a <edge>             absorption edge (K, L1, L2, L3, M) (default is auto detect)\n");
  (void)fprintf( stderr, "-r <resol>            energy resolution (dE/E) (default is Si(111) 1.4x10-4)\n");
  (void)fprintf( stderr, "-1 <e1>               Below edge fit lower energy limit (eV)\n");
  (void)fprintf( stderr, "-2 <e2>               Below edge fit upper energy limit (eV)\n");
  (void)fprintf( stderr, "-3 <e3>               Above edge fit lower energy limit (eV)\n");
  (void)fprintf( stderr, "-4 <e4>               Above edge fit upper energy limit (eV)\n");
  (void)fprintf( stderr, "-p <PS file>          output to PostScript file\n");
  (void)fprintf( stderr, "-o <efs file>         filename for efs output (default output.efs\n");
  (void)fprintf( stderr, "-x                    plot to Xwindows\n");
  (void)fprintf( stderr, "-v <level>            verbosity level (0 -- 3) (default 0)\n");
  exit( EXIT_FAILURE );
}
