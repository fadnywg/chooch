/***************************************************************************
                            integrate.c  -  description
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
  int i;
  double fp, error, E0, dE;
  double fElo, fEhi;

  fElo=fEdge/1000.0;
  fEhi=fEdge*50.0;

  acc=gsl_interp_accel_alloc();
  spline=gsl_spline_alloc(gsl_interp_akima, nDataPoints);
  gsl_spline_init(spline, fXraw, fYfpp, nDataPoints);

  dE=(fXraw[nDataPoints-1]-fXraw[0])/(nDataPoints-1);
  printf("Energy interval = %f\n", dE);
  for (i=0, E0=fXraw[1]; E0<fXraw[nDataPoints-1]; E0+=dE, i++)
    {
      //      printf("\n\n=====\nPoint No. i=%d     x=%f   y=%f\n", i, E0, fYfpp[i]);
      fYfp[i]=0.0;
      //      printf("Integration 1  E0=%f     a=%f   b=%f \n", E0, fElo, fXraw[i-1]);
      fYfp[i]+=IntegrateExtrap(nDataPoints, E0, fElo, fXraw[0], error);
      //      printf("tmp  %f    Sum of fp so far  = %f \n", tmp, fYfp[i]);
      //      printf("Integration 2  E0=%f     a=%f   b=%f \n", E0, fXraw[nDataPoints-1], fEhi);
      fYfp[i]+=IntegrateExtrap(nDataPoints, E0, fXraw[nDataPoints-1], fEhi, error);
      //      printf("tmp  %f    Sum of fp so far  = %f \n", tmp, fYfp[i]);
      //      printf("Integration 3  E0=%f     a=%f   b=%f \n", E0, fXraw[0], fXraw[nDataPoints-1]);
      fYfp[i]+=IntegrateCurve(nDataPoints, E0, fXraw[0], E0-dE);
      //      printf("tmp  %f    Sum of fp so far  = %f \n", tmp, fYfp[i]);
      //      printf("Integration 4  E0=%f     a=%f   b=%f \n", E0, fXraw[i+1], fXraw[nDataPoints-1]);
      fYfp[i]+=IntegrateCurve(nDataPoints, E0, E0+dE, fXraw[nDataPoints-1]);
      //      printf("tmp  %f    Sum of fp so far  = %f \n", tmp,fYfp[i]);
      //      printf("Singularity\n");
      fYfp[i]+=Singularity(E0, E0-dE, E0+dE, fYfpp[i], fYfpp[i-1], fYfpp[i+1], fD1[i], fD2[i], fD3[i]);
      //      printf("tmp  %f    Final SUM of fp so far  = %f \n", tmp, fYfp[i]);
      fXfpp[i]=E0;
      fYspline[i]=gsl_spline_eval(spline, E0, acc);
    }
  *nPoints=i-1;
  /*
  */
  gsl_spline_free (spline);
  gsl_interp_accel_free(acc);
}

double f(double E, void * params) {
  double ratio, answer;
  double E0 = *(double *) params;
  answer = E*get_fpp(sElement, E/1000.0)/(E0*E0-E*E);
  return answer;
}

double fc(double E, void * params) {
  extern gsl_spline *spline;
  double ratio, answer,fdp;
  double E0 = *(double *) params;
  fdp = gsl_spline_eval(spline, E, acc);
  answer = E*fdp/(E0*E0-E*E);
  return answer;
}

double fcp(double E, void * params) {
  extern gsl_spline *spline;
  double ratio, answer,fdp;
  double E0 = *(double *) params;
  fdp = gsl_spline_eval(spline, E, acc);
  answer = -1.0*E*fdp/(E0+E);
  return answer;
}

double fs(double E, void * params) {
  extern gsl_spline *spline;
  double ratio, answer,fdp;
  double E0 = *(double *) params;
  fdp = gsl_spline_eval(spline, E, acc);
  answer = -1.0*fdp/(E0+E);
  return answer;
}

double IntegrateExtrap(int N, double E0, double a, double b, double error)
{
  double result;
  gsl_integration_workspace * w=gsl_integration_workspace_alloc(1000);
  gsl_function F;
  F.function = &f;
  F.params = &E0;

  gsl_integration_qag(&F, a, b, 0.0, 1e-7, 1000, 3, w, &result, &error); 
  /*
  printf("result          = % .18f\n", result);
  printf("estimated error = % .18f\n", error);
  printf("intervals =  %d\n", w->size);
  */
  gsl_integration_workspace_free(w);
  return 2.0*result/PI;
}

double CauchyCurve(int N, double E0, double a, double b){
  double result, error;
  gsl_integration_workspace * w 
    = gsl_integration_workspace_alloc(500);
  gsl_function F;
  F.function = &fcp;
  F.params = &E0;
  gsl_integration_qawc (&F, a, b, E0, 0.0, 1e-3, 500, w, &result, &error); 
  printf("result          = % .18f\n", result);
  /*
  printf("estimated error = % .18f\n", error);
  printf("intervals =  %d\n", w->size);
  */
  gsl_integration_workspace_free(w);
  return result;
}

double IntegrateCurve(int N, double E0, double a, double b){
  double result, error;
  gsl_integration_workspace * w 
    = gsl_integration_workspace_alloc(500);
  gsl_function F;
  F.function = &fc;
  F.params = &E0;
  gsl_integration_qag (&F, a, b, 1e-4, 1e-6, 500, 5, w, &result, &error); 
  /*
  printf("result          = % .18f\n", result);
  printf("estimated error = % .18f\n", error);
  printf("intervals =  %d\n", w->size);
  */
  gsl_integration_workspace_free(w);
  return 2.0*result/PI;
}

double Singularity(double E0, double a, double b, 
                   double fppE0, double fppa, double fppb, 
                   double fD1, double fD2, double fD3){
  double result, error;
  gsl_integration_workspace * w 
    = gsl_integration_workspace_alloc(50);
  double d1, d2;
  double term1, term2, term3, term4, term5;
  gsl_function F;
  F.function = &fs;
  F.params = &E0;
  gsl_integration_qag (&F, a, b, 0.0, 1e-6, 50, 6, w, &term1, &error); 
  
  d1 = a-E0;
  d2 = b-E0;
  term2 = -1.0*(log(fabs(d2)) - log(fabs(d1)));
  term3= -1.0*fD1*(b-a);
  term4 = -0.25*fD2*(d2*d2-d1*d1);
  term5= -1.0*fD3*(d2*d2*d2-d1*d1*d1)/18.0;
  result = (term1+term2+term3+term4+term5)/PI;
  //  printf("%f + %f + %f + %f + %f / PI = %f\n", term1, term2, term3, term4, term5, result);
  return result;
}








