/***************************************************************************
                             copyright.c  -  prints copyright
                             -------------------
    begin                : Sat July  6 12:01:02 GMT 2002
    copyright            : (C) 1994--2003 by Gwyndaf Evans
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
#include <stdio.h>
#include "chooch.h"
//[-e <element>] [-c \"<cell dimensions>\"] [-I <beam intensity>] [-n <number heavy atoms>] [-v <sample volume>] 
//[-t <sample thickness>] [-E <X-ray energy>] [-m <heavy atom conc.>] [-O <crystal ang. velocity>] [-D <detector distance>]
void copyright()
{
  (void)fprintf( stderr, "==================================================\n" );
  (void)fprintf( stderr, "                chooch-1.0.2-beta                 \n");
  (void)fprintf( stderr, "      by Gwyndaf Evans Copyright (C) 1994--2003   \n" );
  (void)fprintf( stderr, "             gwyndaf@gwyndafevans.co.uk\n\n" );
  (void)fprintf( stderr, "        G. Evans & R. F. Pettifer (2001)\n");
  (void)fprintf( stderr, "           J. Appl. Cryst. 34, 82-86.\n");
  (void)fprintf( stderr, "==================================================\n\n" );
}
