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
#include "mucal-C/mucal.h"     

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

/*
 * Return the name of absorption edge given the array index of dEnergy
 */
char *edge_name(int n)
{
   static char *name[] = {
      "K", "L1", "L2", "L3", "M", "Bad edge"
   };
   return (n < 0 || n > 4) ? name[5] : name[n];
}

char *get_Edge(char *sElement, double fMidE, double *fE)
{
   /*
    * Given the mid point of recorded spectrum and element, guess the edge energy
    */
  extern int verbose;
  double dEnergy[9], dXsec[11], dFluo[4];
  double diff, mindiff=1e10, E;
  int    i, iE, iZZ = 0, iPflag=0;
  int    err;
  char   sUnit='a', sErrmsg[80];
  fMidE/=1000.0;
  if((err=mucal(sElement, iZZ, 0.0, sUnit, iPflag, dEnergy, dXsec, dFluo, &sErrmsg[0])) != 0) {
    printf("WARNING: Problem  getting edge energy from mucal");
  }
  if(verbose>0)printf(" Mid point of spectrum = %f\n", fMidE);
  if(verbose>0)printf(" E edge      E diff\n");
  for(i=0; i<5; i++){
     diff=fabs(fMidE-dEnergy[i]);
     if(diff < mindiff){
	mindiff=diff;
	iE=i;
	*fE=dEnergy[i]*1000.0;
     }
     if(verbose>0)printf(" %8.4f       %8.4f\n", dEnergy[i], diff);
  }
  return edge_name(iE);
}

double get_fpp (char *sElement, double dEnergyKeV)
{
  double dFdprime;
  dFdprime = 143.10935e-10 * dEnergyKeV * 1000.0 * get_CrossSection(sElement, dEnergyKeV);
  return dFdprime;
}






