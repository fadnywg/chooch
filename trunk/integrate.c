/***************************************************************************
                            integrate.c  -  do Kramers-Kronig integration
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
#include <math.h>
#include <stdlib.h>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>
#include "chooch.h"
extern char *sElement;
gsl_interp_accel *acc;
gsl_spline *spline;

void Integrate(int nDataPoints, int *nPoints, double fEdge, double *fXraw, double *fXfpp, 
               double *fYspline, double *fYfpp, double *fD1, double *fD2, 
               double *fD3, double *fYfp)
{
  extern int verbose;
  extern double fEres;
  int i;
/*  double fp, error=0.0, E0, dE; changed for next line for v5.0.8*/
  double error=0.0, E0, dE;
  double fElo, fEhi;

  fElo=fEdge/1000.0;
  fEhi=fEdge*50.0;

  acc=gsl_interp_accel_alloc();
  //  spline=gsl_spline_alloc(gsl_interp_akima, nDataPoints);
  spline=gsl_spline_alloc(gsl_interp_linear, nDataPoints);
  gsl_spline_init(spline, fXraw, fYfpp, nDataPoints);

  
  /*  
   *dE=(fXraw[nDataPoints-1]-fXraw[0])/(nDataPoints-1); 
   */
  if(verbose>0){
    printf("****************************\n");
    printf("        Integrate.c         \n");
    printf("****************************\n");
  }
  dE=fEres*fXraw[0]/5.0;
  if(verbose>0)printf("Energy interval = %f\n", dE);
  //  for (i=0, E0=fXraw[0]+dE; E0<=fXraw[nDataPoints-1]-dE; E0+=dE, i++)


  /******************************************************************
   * First calculate for all points on spectrum except first and last
   ******************************************************************/
  if(verbose>2)printf("/******************************************************************\n");
  if(verbose>2)printf(" * First calculate for all points on spectrum except first and last\n");
  if(verbose>2)printf(" ******************************************************************\n");

  for (i=1; i<nDataPoints-1; i++)
    {
      E0=fXraw[i];
      if(verbose>2)printf("\n\n=====\nPoint No. i=%d     x=%f   y=%f\n", i, E0, fYfpp[i]);
      fYfp[i]=0.0;

      /* Exrapolate to low energy */
      if(verbose>2)printf("\n**************\nIntegration 1  E0=%f     a=%f   b=%f \n**************\n", E0, fElo, fXraw[0]);
      fYfp[i]+=IntegrateExtrap(nDataPoints, E0, fElo, fXraw[0], error);
      if(verbose>2)printf(" Sum of fp so far  = %f \n", fYfp[i]);

      /* From first data point up to singularity E0 */
      if(verbose>2)printf("\n**************\nIntegration 2  E0=%f     a=%f   b=%f \n**************\n", E0, fXraw[0], E0-dE);
      fYfp[i]+=IntegrateCurve(nDataPoints, E0, fXraw[0], E0-dE);
      if(verbose>2)printf(" Sum of fp so far  = %f \n", fYfp[i]);

      /* Singularity */
      if(verbose>2)printf("**************\nSingularity\n**************\n");
      fYfp[i]+=Singularity(E0, E0-dE, E0+dE, fYfpp[i], fYfpp[i-1], fYfpp[i+1], fD1[i], fD2[i], fD3[i]);
      if(verbose>2)printf(" Final SUM of fp so far  = %f \n", fYfp[i]);

      /* From singularity E0 up to last data point */
      if(verbose>2)printf("\n**************\nIntegration 3  E0=%f     a=%f   b=%f \n**************\n", E0, E0+dE, fXraw[nDataPoints-1]);
      fYfp[i]+=IntegrateCurve(nDataPoints, E0, E0+dE, fXraw[nDataPoints-1]);
      if(verbose>2)printf(" Sum of fp so far  = %f \n",fYfp[i]);

      /* Extrapolate to high energy */
      if(verbose>2)printf("\n**************\nIntegration 4  E0=%f     a=%f   b=%f \n**************\n", E0, fXraw[nDataPoints-1], fEhi);
      fYfp[i]+=IntegrateExtrap(nDataPoints, E0, fXraw[nDataPoints-1], fEhi, error);
      if(verbose>2)printf(" Final value of fp = %f \n", fYfp[i]);
      fXfpp[i]=E0;
      fYspline[i]=gsl_spline_eval(spline, E0, acc);
    }


  /*******************************
   * Now calculate for first point
   *******************************/
  if(verbose>2)printf(" ******************************************************************\n");
  if(verbose>2)printf(" * First calculate for all points on spectrum except first and last\n");
  if(verbose>2)printf(" ******************************************************************\n");


  for (i=1; i<nDataPoints-1; i++)
    {
      E0=fXraw[i];
      if(verbose>2)printf("\n\n=====\nPoint No. i=%d     x=%f   y=%f\n", i, E0, fYfpp[i]);
      fYfp[i]=0.0;

      /* Exrapolate to low energy */
      if(verbose>2)printf("\n**************\nIntegration 1  E0=%f     a=%f   b=%f \n**************\n", E0, fElo, fXraw[0]);
      fYfp[i]+=IntegrateExtrap(nDataPoints, E0, fElo, fXraw[0], error);
      if(verbose>2)printf(" Sum of fp so far  = %f \n", fYfp[i]);

      /* From first data point up to singularity E0 */
      if(verbose>2)printf("**************\nIntegration 2  E0=%f     a=%f   b=%f \n**************\n", E0, fXraw[0], E0-dE);
      fYfp[i]+=IntegrateCurve(nDataPoints, E0, fXraw[0], E0-dE);
      if(verbose>2)printf(" Sum of fp so far  = %f \n", fYfp[i]);

      /* Singularity */
      if(verbose>2)printf("\n**************\nSingularity\n**************\n");
      fYfp[i]+=Singularity(E0, E0-dE, E0+dE, fYfpp[i], fYfpp[i-1], fYfpp[i+1], fD1[i], fD2[i], fD3[i]);
      if(verbose>2)printf(" Final SUM of fp so far  = %f \n", fYfp[i]);

      /* From singularity E0 up to last data point */
      if(verbose>2)printf("\n**************\nIntegration 3  E0=%f     a=%f   b=%f \n**************\n", E0, E0+dE, fXraw[nDataPoints-1]);
      fYfp[i]+=IntegrateCurve(nDataPoints, E0, E0+dE, fXraw[nDataPoints-1]);
      if(verbose>2)printf(" Sum of fp so far  = %f \n",fYfp[i]);

      /* Extrapolate to high energy */
      if(verbose>2)printf("\n**************\nIntegration 4  E0=%f     a=%f   b=%f \n**************\n", E0, fXraw[nDataPoints-1], fEhi);
      fYfp[i]+=IntegrateExtrap(nDataPoints, E0, fXraw[nDataPoints-1], fEhi, error);
      if(verbose>2)printf(" Final value of fp = %f \n", fYfp[i]);
      fXfpp[i]=E0;
      fYspline[i]=gsl_spline_eval(spline, E0, acc);
    }


  /*******************************
   * Now calculate for first point
   *******************************/
  if(verbose>2)printf(" *******************************\n");
  if(verbose>2)printf(" * Now calculate for first point\n");
  if(verbose>2)printf(" *******************************\n");

  i=0;
  fYfp[i]=0.0;
  E0=fXraw[0];
  /* Exrapolate to low energy */
  //  if(verbose>2)printf("Integration 1  E0=%f     a=%f   b=%f \n", E0, fElo, fXraw[0]-0.1);
  fYfp[i]+=IntegrateExtrap(nDataPoints, E0, fElo, fXraw[0]-0.1, error);
  //  if(verbose>2)printf(" Sum of fp so far  = %f \n", fYfp[i]);
  /* Singularity */
  //  if(verbose>2)printf("Singularity\n");
  fYfp[i]+=Singularity(E0, E0-0.1, E0+0.1, fYfpp[i], fYfpp[i], fYfpp[i], fD1[i], fD2[i], fD3[i]);
  //  if(verbose>2)printf(" Final SUM of fp so far  = %f \n", fYfp[i]);
  /* From singularity E0 up to last data point */
  //  if(verbose>2)printf("Integration 3  E0=%f     a=%f   b=%f \n", E0, E0+dE, fXraw[nDataPoints-1]);
  fYfp[i]+=IntegrateCurve(nDataPoints, E0, E0+0.1, fXraw[nDataPoints-1]);
  //  if(verbose>2)printf(" Sum of fp so far  = %f \n",fYfp[i]);
    /* Extrapolate to high energy */
  //  if(verbose>2)printf("Integration 4  E0=%f     a=%f   b=%f \n", E0, fXraw[nDataPoints-1], fEhi);
  fYfp[i]+=IntegrateExtrap(nDataPoints, E0, fXraw[nDataPoints-1], fEhi, error);
  //  if(verbose>2)printf(" Final value of fp = %f \n", fYfp[i]);
  fXfpp[i]=E0;
  fYspline[i]=gsl_spline_eval(spline, E0, acc);
  fXfpp[i]=E0;
  fYspline[i]=gsl_spline_eval(spline, E0, acc);


  /*********************************
   * Now calculate for first point
   *********************************/
  if(verbose>2)printf(" *******************************\n");
  if(verbose>2)printf(" * Now calculate for first point\n");
  if(verbose>2)printf(" *******************************\n");
  i=nDataPoints-1;
  fYfp[i]=0.0;
  E0=fXraw[i];
  /* Exrapolate to low energy */
  if(verbose>2)printf("Integration 1  E0=%f     a=%f   b=%f \n", E0, fElo, fXraw[0]-0.1);
  fYfp[i]+=IntegrateExtrap(nDataPoints, E0, fElo, fXraw[0], error);
  if(verbose>2)printf(" Sum of fp so far  = %f \n", fYfp[i]);
  
  /* From first data point up to singularity E0 */
  if(verbose>2)printf("Integration 2  E0=%f     a=%f   b=%f \n", E0, fXraw[0], E0-dE);
  fYfp[i]+=IntegrateCurve(nDataPoints, E0, fXraw[0], E0-0.1);
  if(verbose>2)printf(" Sum of fp so far  = %f \n", fYfp[i]);

  /* Singularity */
  if(verbose>2)printf("Singularity\n");
  fYfp[i]+=Singularity(E0, E0-0.1, E0+0.1, fYfpp[i], fYfpp[i], fYfpp[i], fD1[i], fD2[i], fD3[i]);
  if(verbose>2)printf(" Final SUM of fp so far  = %f \n", fYfp[i]);

  /* Extrapolate to high energy */
  if(verbose>2)printf("Integration 4  E0=%f     a=%f   b=%f \n", E0, fXraw[nDataPoints-1], fEhi);
  fYfp[i]+=IntegrateExtrap(nDataPoints, E0, E0+0.1, fEhi, error);
  if(verbose>2)printf(" Final value of fp = %f \n", fYfp[i]);
  fXfpp[i]=E0;
  fYspline[i]=gsl_spline_eval(spline, E0, acc);
  fXfpp[i]=E0;
  fYspline[i]=gsl_spline_eval(spline, E0, acc);

  
  /*  *nPoints=i-1;*/
  /* Changed 16/9/2004 to test bug from Chuck Farrah */
  *nPoints=i;
  /*
  */
  gsl_spline_free (spline);
  gsl_interp_accel_free(acc);
}

double IntegrateExtrap(int N, double E0, double a, double b, double error)
{
  extern int verbose, status;
  double result;
  gsl_integration_workspace * w=gsl_integration_workspace_alloc(3000);
  gsl_function F;
  F.function = &f;
  F.params = &E0;

  if(verbose>2)printf("Integrating Extrap\n");
  status=gsl_integration_qag(&F, a, b, 1e-3, 1e-3, 3000, 3, w, &result, &error); 

  if(status != 0){
    printf ("gsl error: %d : %s\n", status, gsl_strerror (status));
    exit(EXIT_FAILURE);
  }

  result=2.0*result/PI;
  if(verbose>2){
     printf("result          = % .18f\n", result);
     printf("estimated error = % .18f\n", error);
     printf("intervals =  %d\n", w->size);
  }
  gsl_integration_workspace_free(w);
  return result;
}

double IntegrateCurve(int N, double E0, double a, double b){
  extern int verbose, status;
  double result, error;
  gsl_integration_workspace * w 
    = gsl_integration_workspace_alloc(3000);
  gsl_function F;
  F.function = &fc;
  F.params = &E0;

  if(verbose>1)printf("Integrating curve\n");
  status=gsl_integration_qag (&F, a, b, 1e-3, 1e-3, 3000, 5, w, &result, &error); 

  if(status != 0){
    printf ("gsl error: %d : %s\n", status, gsl_strerror (status));
    exit(EXIT_FAILURE);
  }

  result=2.0*result/PI;
  if(verbose>2){
     printf("result          = % .18f\n", result);
     printf("estimated error = % .18f\n", error);
     printf("intervals =  %d\n", w->size);
  }
  gsl_integration_workspace_free(w);
  return result;
}



double Singularity(double E0, double a, double b, 
                   double fppE0, double fppa, double fppb, 
                   double fD1, double fD2, double fD3){
  extern int verbose, status;
  double result, error;
  gsl_integration_workspace * w 
    = gsl_integration_workspace_alloc(50);
  double d1, d2;
  double term1, term2, term3, term4, term5;
  gsl_function F;
  F.function = &fs;
  F.params = &E0;
  if(verbose>2)printf("Integrating Singularity\n");
  status=gsl_integration_qag (&F, a, b, 1.0e-3, 1.0e-3, 50, 6, w, &term1, &error); 

  if(status != 0){
    printf ("gsl error: %d : %s\n", status, gsl_strerror (status));
    exit(EXIT_FAILURE);
  }

  d1 = a-E0;
  d2 = b-E0;
  term2 = -1.0*(log(fabs(d2)) - log(fabs(d1)));
  term3= -1.0*fD1*(b-a);
  term4 = -0.25*fD2*(d2*d2-d1*d1);
  term5= -1.0*fD3*(d2*d2*d2-d1*d1*d1)/18.0;
  result = (term1+term2+term3+term4+term5)/PI;
  if(verbose>2)printf("%f + %f + %f + %f + %f / PI = %f\n", term1, term2, term3, term4, term5, result);
  return result;
}

/*
 * Functions for QAG integrator
 *
 *
 *
 */

double f(double E, void * params) {
  double answer;
  double E0 = *(double *) params;
  answer = E*get_fpp(sElement, E/1000.0)/(E0*E0-E*E);
  return answer;
}

double fc(double E, void * params) {
  extern gsl_spline *spline;
  extern gsl_interp_accel *acc;
  double answer,fdp;
  double E0 = *(double *) params;
  fdp = gsl_spline_eval(spline, E, acc);
  answer = E*fdp/(E0*E0-E*E);
  return answer;
}

double fs(double E, void * params) {
  extern gsl_spline *spline;
  extern gsl_interp_accel *acc;
  double answer,fdp;
  double E0 = *(double *) params;
  fdp = gsl_spline_eval(spline, E, acc);
  answer = -1.0*fdp/(E0+E);
  return answer;
}








