/***************************************************************************
                          smooth.c  -  Smoothing with Savitsky-Golay filter
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "chooch.h"



int smooth(int np, double *f, double *g, int nl, int nr, int m, int ld) {
  // 
  extern int verbose;
  int i, n, err;
  float fpad[MAXSIZE];
  float cn[MAXCOEF];
  // Required by savgol
  float c[MAXSIZE];
  //
  if(verbose>1)printf("nl = %d     nr = %d     ld = %d     m = %d\n", nl, nr, ld, m);
  pad(np, nl, nr, f, fpad);
  savgol(c, np, nl, nr, ld, m);
  //  apply_response(np, f, g, c, (nl+nr+1));
  unwrap(c, cn, np, nl, nr);
  apply_coeffs(fpad, g, cn, nl, nr, np);
  if(verbose>0)printf("Done smoothing\n");
  return 0;
}

/*
int apply_response(int np, double *f, double *g, double *respns, int mm) {
  int p=1, i;
  unsigned long n, m;
  double data[MAXSIZE], ans[MAXSIZE];
  m = (unsigned long) mm;
  while ((int) pow(2,p) < np) {
    p++;
  }
  n = (int) pow(2,p);
  printf("Size of original and zero padded data arrays: %d   %d\n", np, n);
  for (i = 1; i <= np; i++) {
    data[i] = (double) f[i];
  }
  for (i = np+1; i <= n; i++) {
    data[i] = 0.0;
  }  
  convlv(data, n, respns, m, 1, ans);
  for (i = 1; i <= np; i++) {
    g[i-1] = (double) ans[i];
  }
  return 0;
}
*/

void pad(int np, int nl, int nr, double *fin, float *fout) {
  extern int verbose;
  int i;
  if(verbose>0)printf("Padding input spectrum\n");
  for (i = 0; i < nl; i++) {
    fout[i] = (float) fin[0];
  }
  for (i = nl; i < (np+nl); i++) {
    fout[i] = (float) fin[i-nl];
  }
  for (i = (np+nl); i < (nl+np+nr); i++) {
    fout[i] = (float) fin[np-1];
  }
/*    for (i = 0; i < (nl+np+nr); i++) { */
/*      printf("%d  %10.3f\n", i, fout[i]); */
/*    } */
}

int apply_coeffs(float *fpad, double *g, float *cn, int nl, int nr, int np) {
  extern int verbose;
  int i, n;
  if(verbose>0)printf("Applying SavGol coeffs\n");
  for (i = 0; i < np; i++) {
    g[i] = 0.0;
    for (n = 0; n < (nl+nr+1); n++) {
      g[i] += (double) (cn[n] * fpad[i+n]);
    }
  }
  return 0;
}

int unwrap(float *cin, float *cout, int np, int nl, int nr) {
  int i, k = 0;
  int nc;
  float totc=0.0;

  // e.g. for nl=4 and nr=4 
  //out k: c[0]  c[1]  c[2]  c[3]  c[4]  c[5]  c[6]    c[7]    c[8]
  //co     c-4   c-3   c-2   c-1   c0    c1    c2      c3      c4
  //in  i: c[5]  c[4]  c[3]  c[2]  c[1]  c[np] c[np-1] c[np-2] c[n-3]

  nc = nl+nr+1;
/*    for (i = 1; i <= np; i++) { */
/*      printf("%s %7.3f %s ", (i==0)?"cin: ":"", cin[i], (i==(np))?"\n":""); */
/*    } */
  cout[k] = cin[nl+1];
  for (k = 1, i = (nl); i > 0; i--, k++) {
    cout[k] = cin[i];
  }
  for (k = (nl+1), i = np; i >= (np-nr+1); i--, k++) {
    cout[k] = cin[i];
  }
  for (k = 0; k < nc; k++) {
    //    printf("%s %7.3f %s ", (k==0)?"cout: ":"", cout[k], (k==(nc-1))?"\n":"");
    totc += cout[k];
  }
  /*
  if (!(totc == 1.000)) {
     printf("ERROR: Sum of cout should = 1.0;  actual value = %10.8f\n", totc);
     exit (EXIT_FAILURE);
  }
  */
  return 0;
}




