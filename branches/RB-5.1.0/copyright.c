/***************************************************************************
                             copyright.c  -  prints copyright
                             -------------------
    begin                : Sat July  6 12:01:02 GMT 2002
    copyright            : (C) 1994--2012 by Gwyndaf Evans
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
void copyright()
{
  (void)fprintf( stderr, "==================================================\n");
  (void)fprintf( stderr, "                     chooch-5.1.0                 \n");
  (void)fprintf( stderr, "      by Gwyndaf Evans Copyright (C) 1994--2013   \n");
  (void)fprintf( stderr, "             gwyndaf@gwyndafevans.co.uk\n\n" );
  (void)fprintf( stderr, "        G. Evans & R. F. Pettifer (2001)\n");
  (void)fprintf( stderr, "           J. Appl. Cryst. 34, 82-86.\n");
  (void)fprintf( stderr, "==================================================\n\n");
}
