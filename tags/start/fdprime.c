/***************************************************************************
                            get_fpp.c  -  description
                             -------------------
    begin                : Sat July  6 12:01:02 GMT 2002
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
#include <float.h>
#include <math.h>
#include "chooch.h"
#include "mucal.h"     

double get_CrossSection(char *sElement, double dEnergyKeV)
{
  double dEnergy[9], dXsec[11], dFluo[4];
  double dXsection;
  int    iZZ = 0, iPflag=0;
  int    err;
  char   sUnit='a', sErrmsg[80];
  
  //  printf("%s ",sElement);
  if((err=mucal(sElement, iZZ, dEnergyKeV, sUnit, iPflag, dEnergy, dXsec, dFluo, &sErrmsg[0])) != 0) {
    //    printf("MUCAL error in get_fpp %d %s\n", err, sErrmsg);
    //    exit(EXIT_FAILURE);
  }
  dXsection = dXsec[0];
  return dXsection;
}

double get_Edge(char *sElement, char *sEdge)
{
  double dEnergy[9], dXsec[11], dFluo[4];
  int    iZZ = 0, iPflag=0;
  int    err;
  char   sUnit='a', sErrmsg[80];
  if((err=mucal(sElement, iZZ, 0.0, sUnit, iPflag, dEnergy, dXsec, dFluo, &sErrmsg[0])) != 0) {
    printf("WARNING: Problem  getting edge energy from mucal");
  }
  return;
}

double get_fpp (char *sElement, double dEnergyKeV)
{
  double dFdprime;
  dFdprime = 143.10935e-10 * dEnergyKeV * 1000.0 * get_CrossSection(sElement, dEnergyKeV);
  return dFdprime;
}






