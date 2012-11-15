# CHOOCH Makefile
# Created on Oct 30 1999 by Gwyndaf Evans
#
# Edit the three first directory definitions to specify
# a) Edit the host type
# b)  the directory where GSL (Gnu Scientific Library) is kept
# c)  the directory where Cgraph (PS plotting library) is kept
# d)  and the directory where you would like you executables to go (BINDIR).
#
#ARCH   = Linux
#ARCH   = OSF1
#ARCH   = SunOS
ARCH   = osx
#
PGPLOTDIR  = /usr/local/pgplot
INCLUDE   = /usr/local/pgplot
PLPLOTLIBDIR = /usr/local/lib
GSLDIR = /usr/local/lib
X11LIBDIR  = /usr/X11R6/lib
#BINDIR    = /home/gwyndaf/bin
######################################
#
VERSION = 5.0.9
LIBS = -lgsl -lgslcblas -lX11
PGLIBS =  -lcpgplot -lpgplot
PLLIBS= -lplplotd
EXE    = chooch-$(VERSION).$(ARCH)
EXEPG    = chooch-$(VERSION)-pg.$(ARCH)
#
# How to compile and link
#
include Makefile.osx
#
# Basic definitions
#
RM    = /bin/rm
MV    = /bin/mv
CP    = /bin/cp
#
#
OBJECTS = chooch.o checks.o       copyright.o  fdprime.o    fits.o     \
          fluread.o      integrate.o  license.o        \
          minmax.o       mucal.o      normalize.o  plpng.o   \
          printbanner.o  savwin.o     selwavel.o \
          smooth.o       toplot.o     usage.o
#
#
chooch : clean ${OBJECTS} Makefile
	$(CC) -o ${EXE} ${OBJECTS} $(LDFLAGS)

chooch-pg : 
	make chooch-with-pgplot "CFLAGS = -I$(INCLUDE) $(CFLAGS) -DPGPLOT"

chooch-with-pgplot : clean ${OBJECTS} Makefile
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
