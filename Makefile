# CHOOCH Makefile
# Created on Oct 30 1999 by Gwyndaf Evans
#
# Edit the three first directory definitions to specify
# a)  the directory where the pgplot library is (PGPLOTLIB);
# b)  the directory where GSL (Gnu Scientific Library) is held
# c)  the directory where the X11 library is (LIBX11);
# d)  and the directory where you would like you executables to go (BINDIR).
#
PGPLOTDIR = /usr/local/pgplot
GSLDIR = /usr/local/lib
X11LIBDIR = /usr/X11R6/lib
BINDIR    = /home/gwyndaf/bin/linux_exe
INCLUDE   = /usr/local/include
######################################
#
# N.B. -ldl required to link successfully and avoid dynamic loading errors
#
PGLIBS = -lcpgplot -lpgplot -lcgraph
LIBS =  -lm -lgsl -lgslcblas -lX11 -ldl
#
EXE    = chooch
#
# How to compile and link
#
FC     = g77
CC     = gcc
FOPTIM = -O2
FFLAGS = $(FOPTIM)
LDFLAGS = -static -I $(INCLUDE) -L$(PGPLOTDIR) -L$(X11LIBDIR) -L$(GSLDIR) $(PGLIBS) $(LIBS)
F      = f
LNS    = ln -s
#
# Basic definitions
#
SHELL = /bin/sh
RM    = /bin/rm
MV    = /bin/mv
CP    = /bin/cp
# 
# 
OBJECTS = main.o fluread.o printbanner.o toplot.o minmax.o spline.o \
	mucal-C/mucal.o fdprime.o savgol.o lubksb.o ludcmp.o nrutil.o\
	smooth.o convlv.o twofft.o realft.o four1.o fits.o normalize.o \
	checks.o usage.o integrate.c psplot.o selwavel.o
#
#
chooch : ${OBJECTS}
	$(FC) -o ${EXE} ${OBJECTS} $(FFLAGS) $(LDFLAGS)
#
install :
	$(MV) $(EXE) $(BINDIR)
#
clean :
	${RM} *.o mucal-C/*.o
# 
# End
#









