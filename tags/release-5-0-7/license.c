/***************************************************************************
                    license.c  -  shows license information
                             -------------------
    begin                : Mon Dec  1 20:26:00 CET 2003
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
#include <stdio.h>
void license()
{
   (void)fprintf( stderr, "License information\n");
   (void)fprintf( stderr, "-------------------\n");
   (void)fprintf( stderr, "Chooch comes with ABSOLUTELY NO WARRANTY; for details\n");
   (void)fprintf( stderr, "type `chooch -w'.  This is free software, and you are\n");
   (void)fprintf( stderr, "welcome to redistribute it under certain conditions; \n");
   (void)fprintf( stderr, "type `chooch -c' for details.\n\n");
   (void)fprintf( stderr, "You should have received a copy of the GNU General Public License\n");
   (void)fprintf( stderr, "along with this program; if not, write to the Free Software\n");
   (void)fprintf( stderr, "Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.\n\n");
}
void nowarranty()
{
   (void)fprintf( stderr, "\nThis program is distributed in the hope that it will be useful,\n");
   (void)fprintf( stderr, "but WITHOUT ANY WARRANTY; without even the implied warranty of\n");
   (void)fprintf( stderr, "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n");
   (void)fprintf( stderr, "GNU General Public License for more details.\n\n");
}
void distribution()
{
   (void)fprintf( stderr, "\nThis program is free software; you can redistribute it and/or\n");
   (void)fprintf( stderr, "modify it under the terms of the GNU General Public License\n");
   (void)fprintf( stderr, "as published by the Free Software Foundation; either version 2\n");
   (void)fprintf( stderr, "of the License, or (at your option) any later version.\n\n");
}
