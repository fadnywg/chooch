# CHOOCH Makefile
# Created on Oct 30 1999 by Gwyndaf Evans
#
# Edit the three first directory definitions to specify
# a)  the directory where GSL (Gnu Scientific Library) is kept
# b)  the directory where Cgraph (PS plotting library) is kept
# c)  and the directory where you would like you executables to go (BINDIR).
#
GSLDIR = /usr/local/lib
CGRAPHDIR = /usr/local/lib
BINDIR    = /home/gwyndaf/bin/linux_exe
INCLUDE   = /usr/local/include
PGPLOTDIR = /usr/local/pgplot
X11LIBDIR  = /usr/X11R6/lib
#GSLDIR = /users/opd14/Gwyndaf/lib
#BINDIR    = /users/opd14/Gwyndaf/bin
#INCLUDE   = /users/opd14/Gwyndaf/include
######################################
#
CGRAPH = -lcgraph
LIBS = -lgsl -lgslcblas -lX11
PGLIBS =  -lcpgplot -lpgplot
EXE    = chooch-1.0.2
EXEPG    = chooch-1.0.2-pg
#
# How to compile and link
#
#include Makefile.SunOS
include Makefile.Linux
#include Makefile.OSF1
#
# Basic definitions
#
RM    = /bin/rm
MV    = /bin/mv
CP    = /bin/cp
#
#
OBJECTS = main.o fluread.o printbanner.o minmax.o spline.o \
        mucal-C/mucal.o fdprime.o savgol.o lubksb.o ludcmp.o nrutil.o\
        smooth.o fits.o normalize.o checks.o usage.o integrate.o \
	psplot.o selwavel.o copyright.o toplot.o
#
#
chooch : clean ${OBJECTS} Makefile
	$(CC) -v $(CFLAGS) -o ${EXE} ${OBJECTS} $(LDFLAGS)

chooch-pg : clean
	make chooch-with-pgplot "CFLAGS = -DPGPLOT"

chooch-with-pgplot : ${OBJECTS} Makefile
	$(FC) -v $(CFLAGS) -o ${EXEPG} ${OBJECTS} $(LDFLAGS)
#
all: chooch chooch-pg
#
mucal-C/mucal.o : mucal-C/mucal.c
	$(CC) $(CFLAGS)   -c -o $@ $?
#	$(MV) mucal.o $@
#
install :
	$(MV) $(EXE)   $(BINDIR)
	$(MV) $(EXEPG) $(BINDIR)
#
clean :
	${RM} -f *.o mucal-C/*.o
#
# End
#
