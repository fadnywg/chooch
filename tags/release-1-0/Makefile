# CHOOCH Makefile
# Created on Oct 30 1999 by Gwyndaf Evans
#
# Edit the three first directory definitions to specify
# a)  the directory where GSL (Gnu Scientific Library) is kept
# b)  the directory where Cgraph (PS plotting library) is kept
# c)  and the directory where you would like you executables to go (BINDIR).
#
GSLDIR = /home/gwyndaf/chooch/alpha/lib
CGRAPHDIR = /home/gwyndaf/chooch/alpha/lib
BINDIR    = /home/gwyndaf/bin/linux_exe
INCLUDE   = /home/gwyndaf/chooch/alpha/include
#GSLDIR = /users/opd14/Gwyndaf/lib
#BINDIR    = /users/opd14/Gwyndaf/bin
#INCLUDE   = /users/opd14/Gwyndaf/include
######################################
#
CGRAPH = -lcgraph
LIBS = -lgsl -lgslcblas 
#-lX11
EXE    = chooch
#
# How to compile and link
#
#include Makefile.SunOS
include Makefile.Linux
#include Makefile.OSF1
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
        smooth.o fits.o normalize.o checks.o usage.o integrate.o \
	psplot.o selwavel.o copyright.o
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
