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
#GSLDIR = /users/opd14/Gwyndaf/lib
#X11LIBDIR = /usr/X/lib
#BINDIR    = /users/opd14/Gwyndaf/bin
#INCLUDE   = /users/opd14/Gwyndaf/include
######################################
#
# N.B. -ldl required to link successfully and avoid dynamic loading errors
#
CGRAPH = -lcgraph
LIBS =  -lm -lgsl -lgslcblas -lX11
#-ldl
#
EXE    = chooch
#
# How to compile and link
#
#include Makefile.SunOS
include Makefile.Linux
#
# Basic definitions
#
SHELL = /bin/sh
RM    = /bin/rm
MV    = /bin/mv
CP    = /bin/cp
#
#
OBJECTS = main.o fluread.o printbanner.o minmax.o spline.o \
        mucal-C/mucal.o fdprime.o savgol.o lubksb.o ludcmp.o nrutil.o\
        smooth.o convlv.o twofft.o realft.o four1.o fits.o normalize.o \
        checks.o usage.o integrate.o psplot.o selwavel.o
#toplot.o
#
#
chooch : ${OBJECTS} Makefile
	$(CC) -v $(CFLAGS) -o ${EXE} ${OBJECTS} $(LDFLAGS)
#
mucal-C/mucal.o : mucal-C/mucal.c
	$(CC) $(CFLAGS) -c -o $@ $?
	$(MV) mucal.o $@
#
install :
	$(MV) $(EXE) $(BINDIR)
#
clean :
	${RM} *.o mucal-C/*.o
#
# End
#
