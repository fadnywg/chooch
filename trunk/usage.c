#include <stdio.h>
#include "chooch.h"
//[-e <element>] [-c \"<cell dimensions>\"] [-I <beam intensity>] [-n <number heavy atoms>] [-v <sample volume>] 
//[-t <sample thickness>] [-E <X-ray energy>] [-m <heavy atom conc.>] [-O <crystal ang. velocity>] [-D <detector distance>]
void usage()
{
  (void)fprintf( stderr, "Usage:\n" );
  (void)fprintf( stderr, "  chooch [options]\n\n");
  (void)fprintf( stderr, "-help                          print this message\n");
  (void)fprintf( stderr, "-d <debug_flag>                1 = debug on, 0 = debug off (default = 0) \n");
  (void)fprintf( stderr, "-a <element>                   element symbol\n");
  (void)fprintf( stderr, "-e <edge>                      absorption edge (K, L1, L2, L3, M)\n");
  exit( EXIT_FAILURE );
}
