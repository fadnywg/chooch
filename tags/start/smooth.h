/***************************************************************************
                          smooth.h  -  description
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

int smooth(int , double *, double *, int, int, int, int);
int apply_coeffs(double *, double *, float *, int , int , int );
int apply_response(int , double *, double *, float *, int );
int unwrap(float *, float *, int , int , int );
void pad(int , int , int , double *, double *);
