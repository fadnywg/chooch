/* Program to plot scatter diagram of L and R Opt TF determined by Spect Amplitude */
/* Izumi Ohzawa 94-10-10 */
/*
cc -O -Wall -s -object -o TFopt-lrLog TFopt-lrLog.c -lcgraph

Define USE_STDOUT to send output to stdout (see Makefile)

*/


char *comment= "% chooch PS file [ Gwyndaf Evans]\n\
% Comparison of L-R optimal TF [Log axes] from XT spectrum peaks.\n";

#include <stdio.h>
#include <math.h>
#include <string.h>
/*#include <streams/streams.h>*/
#include <cgraph.h>
#include "chooch.h"

void psplt(int nDataPoints, double *dX, double *dY1, double *dY2, char *filename) {
   extern double fpInfl, fppInfl, fpPeak, fppPeak, EInfl, EPeak;
   extern char cScanTitle[];
   float fMinX, fMaxX, fMinY, fMaxY, fLenX, fLenY, fDum;
   float fXplot[MAXSIZE], fY1plot[MAXSIZE], fY2plot[MAXSIZE];
   int i;
   int numoff, ticsep;
   int ninside, rot, expand;
   int marktype;
   int ntotal;
   float scale;
   float markersize = 8.0;
   char cfpInfl[30], cfppPeak[30], tablehead[40];

   for (i = 0 ; i < nDataPoints; i++) {
      fXplot[i] = (float) dX[i];
      fY1plot[i] = (float) dY1[i];
      fY2plot[i] = (float) dY2[i];
      /*      printf("%d %f %f %f\n",i, fXplot[i], fY1plot[i], fY2plot[i]);*/
   }
   minmax(nDataPoints, fXplot, &fMinX, &fMaxX);
   minmax(nDataPoints, fY1plot, &fDum, &fMaxY);
   minmax(nDataPoints, fY2plot, &fMinY, &fDum);
   fMinY = (fMinY >= 0.0)? (fMinY - fMaxY * 0.05) : (fMinY * 1.05);
   fMinY = (float) ((int) fMinY) - 1; 
   fMaxY = (fMaxY >= 0.0)? (fMaxY * 1.15) : (fMaxY - fMinY * 0.1);
   fMaxY = (float) ((int) fMaxY) + 1;
   fLenX=fMaxX-fMinX;
   fLenY=fMaxY-fMinY;
   rot = 1;		/* portrait */
   expand = 1;		/* expand to fill screen */
   scale = 1.0;		/* scaling factor */
 

#ifdef USE_STDOUT
   cg_use_stdout(1);		/* output to stdout */
#endif

   /*   cg_setcreator(argv[0]);*/
   cg_launch_preview(0);
   cg_set_output_filename(filename);
   cg_settitle(cScanTitle);
   cg_setboundingbox("0 0 600 520");
   cg_set_top_comments(comment); 	/* Comments that go directly after prolog. */
					/* These should describe the data source and plotting */
					/* parameters in detail. */
   cg_init(rot, expand, scale);	/* expand is not used in cg_init() but for compatibility */
   
   
   cg_aorigin(1.0, 1.0);
   cg_font(18.0, 20);

/* ## Do X and Y axes ## */
   cg_grayrgbcolor(0.0,  0.0, 0.0, 0.0);
   /*
    *
    */

   /* X-AXIS */
   cg_linax_style(3, 0, 5, 3, 8);
   cg_xaxis(6.5, fMinX, fMaxX, 0.0, fLenX/5, 1);
   cg_xlabel("X-ray energy (eV)");
   /* Y-AXIS */ 
   numoff = (((int)fMinY)%2 == 0) ? 0 : 1;
   cg_linax_style(3, numoff, 5, 3, 8);
   ticsep=((int)fLenY)/10;
   cg_yaxis(5.0, fMinY, fMaxY, 0.0, ticsep, 2);
   cg_ylabel("f\' and f\'\' (e)");
   /*
    *
    */
   sprintf(tablehead,"       energy      f\'\'    f\'");
   sprintf(cfppPeak, "peak  %8.2f   %5.2f  %5.2f", EPeak, fppPeak, fpPeak);
   sprintf(cfpInfl,  "infl  %8.2f   %5.2f  %5.2f", EInfl, fppInfl, fpInfl);
   cg_font(20.0, 20);
   cg_centerlabel(cScanTitle, 3.0, 5.8, 0.0, 0);
   cg_font(14.0, 20);
   cg_textalign(1.0,5.6,0.0,tablehead, 0, 1, 0);
   cg_textalign(1.0,5.4,0.0,cfppPeak, 0, 1, 0);
   cg_textalign(1.0,5.2,0.0,cfpInfl, 0, 1, 0);

   cg_mesh();
   cg_font(22.0, 20);

   /* Draw exact correspondence, and 0.0 horizontal line */
   cg_grayrgbcolor(0.0, 0.0, 0.0, 0.0);	/* black for B/W, and gray for color */
   cg_linewidth(0.6);
   cg_move(0.5, 0.5);
   cg_line(10.0, 10.0);
   cg_stroke();
   cg_linewidth(1.0);

   /* Marker color is specified by a different function. */
   cg_markergraycolor(0.0, 1.0, 1.0, 1.0,
		      1.0, 1.0, 0.0, 0.0);	/* white in black, or red in white */
   marktype = 0;	/* open circle */
   
   for (i=0; i<nDataPoints; i++) {
      if(i==0)
	 cg_move(fXplot[i], fY1plot[i]);
      else
	 cg_line(fXplot[i], fY1plot[i]);
   }
   cg_stroke();
   for (i=0; i<nDataPoints; i++) {
      if(i==0)
	 cg_move(fXplot[i], fY2plot[i]);
      else
	 cg_line(fXplot[i], fY2plot[i]);
   }   
   cg_stroke();
   /* showpage() is always necessary, or nothing will come out. */
   cg_showpage();
}
