# CHOOCH Makefile
# Created on Oct 30 1999 by Gwyndaf Evans
#
# Edit the three first directory definitions to specify
# a) Edit the host type
# b)  the directory where GSL (Gnu Scientific Library) is kept
# c)  the directory where Cgraph (PS plotting library) is kept
# d)  and the directory where you would like you executables to go (BINDIR).
#
ARCH   = Linux
#ARCH   = OSF1
#ARCH   = SunOS
#GSLDIR = /usr/local/lib
#CGRAPHDIR = /usr/local/lib
#BINDIR    = /home/$(USER)/bin/linux_exe
#INCLUDE   = /usr/local/include
PGPLOTDIR  = /usr/local/pgplot
#X11LIBDIR  = /usr/X11R6/lib
#GSLDIR = /users/opd14/Gwyndaf/lib
#BINDIR    = /users/opd14/Gwyndaf/bin
#INCLUDE   = /users/opd14/Gwyndaf/include
GSLDIR = /usr/local/lib
CGRAPHDIR = /usr/local/lib
BINDIR    = /home/ge73/bin/linux_exe
INCLUDE   = /usr/local/pgplot
#PGPLOTDIR = /home/sci/software/misc/pgplot
X11LIBDIR  = /usr/X11R6/lib
######################################
#
CGRAPH = -lcgraph
LIBS = -lgsl -lgslcblas -lX11
PGLIBS =  -lcpgplot -lpgplot
EXE    = chooch-1.0.4.$(ARCH)
EXEPG    = chooch-1.0.4-pg.$(ARCH)
#
# How to compile and link
#
include Makefile.$(ARCH)
#
# Basic definitions
#
RM    = /bin/rm
MV    = /bin/mv
CP    = /bin/cp
#
#
OBJECTS = main.o      fluread.o printbanner.o minmax.o  spline.o \
          mucal.o     fdprime.o smooth.o      fits.o    normalize.o \
          checks.o    usage.o   integrate.o   psplot.o  selwavel.o \
          copyright.o toplot.o  license.c
#
#
chooch : clean ${OBJECTS} Makefile
	$(CC) -v $(CFLAGS) -o ${EXE} ${OBJECTS} $(LDFLAGS)

chooch-pg : clean
	make chooch-with-pgplot "CFLAGS = $(CFLAGS) -DPGPLOT"

chooch-with-pgplot : ${OBJECTS} Makefile
	$(FC) -v $(CFLAGS) -o ${EXEPG} ${OBJECTS} $(LDFLAGS)
#
all: chooch chooch-pg
#
install :
	$(MV) $(EXE)   $(BINDIR)
	$(MV) $(EXEPG) $(BINDIR)
#
clean :
	${RM} -f *.o
#
# End
#
