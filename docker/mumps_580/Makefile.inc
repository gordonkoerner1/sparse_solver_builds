#
#  This file is part of MUMPS 5.8.0, released
#  on Tue May  6 08:27:40 UTC 2025
#
################################################################################
#
#   Makefile.inc.generic
#
#   This defines some parameters dependent on your platform; you should
#   look for the approriate file in the directory ./Make.inc/ and copy it
#   into a file called Makefile.inc. For example, from the MUMPS root
#   directory, use 
#   "cp Make.inc/Makefile.inc.generic ./Makefile.inc"
#   (see the main README file for details)
#
#   If you do not find any suitable Makefile in Makefile.inc, use this file:
#   "cp Make.inc/Makefile.inc.generic ./Makefile.inc" and modify it according
#   to the comments given below. If you manage to build MUMPS on a new platform,
#   and think that this could be useful to others, you may want to send us
#   the corresponding Makefile.inc file.
#
################################################################################


########################################################################
#Begin orderings
#
# NOTE that PORD is distributed within MUMPS by default. It is recommended to
# install other orderings. For that, you need to obtain the corresponding package
# and modify the variables below accordingly.
# For example, to have Metis available within MUMPS:
#          1/ download Metis and compile it
#          2/ uncomment (suppress # in first column) lines
#             starting with LMETISDIR,  LMETIS
#          3/ add -Dmetis in line ORDERINGSF
#             ORDERINGSF  = -Dpord -Dmetis
#          4/ Compile and install MUMPS
#             make clean; make   (to clean up previous installation)
#
#          Metis/ParMetis and SCOTCH/PT-SCOTCH (ver 6.0 and later) orderings are recommended.
#

#SCOTCHDIR  = ${HOME}/scotch_6.0
#ISCOTCH    = -I$(SCOTCHDIR)/include
#
# You have to choose one among the following two lines depending on
# the type of analysis you want to perform. If you want to perform only
# sequential analysis choose the first (remember to add -Dscotch in the ORDERINGSF
# variable below); for both parallel and sequential analysis choose the second 
# line (remember to add -Dptscotch in the ORDERINGSF variable below)

#LSCOTCH    = -L$(SCOTCHDIR)/lib -lesmumps -lscotch -lscotcherr
#LSCOTCH    = -L$(SCOTCHDIR)/lib -lptesmumps -lptscotch -lptscotcherr

topdir = /usr/src/MUMPS_5.8.0
LPORDDIR = $(topdir)/PORD/lib/
IPORD    = -I$(topdir)/PORD/include/
LPORD    = -L$(LPORDDIR) -lpord$(PLAT)

LMETISDIR = /usr/local/lib
IMETIS    = -I/usr/local/include/

# You have to choose one among the following two lines depending on
# the type of analysis you want to perform. If you want to perform only
# sequential analysis choose the first (remember to add -Dmetis in the ORDERINGSF
# variable below); for both parallel and sequential analysis choose the second 
# line (remember to add -Dparmetis in the ORDERINGSF variable below)

LMETIS    = -L$(LMETISDIR) -lmetis
#LMETIS    = -L$(LMETISDIR) -lparmetis -lmetis

# The following variables will be used in the compilation process.
# Please note that -Dptscotch and -Dparmetis imply -Dscotch and -Dmetis respectively.
# If you want to use Metis 4.X or an older version, you should use -Dmetis4 instead of -Dmetis
# or in addition with -Dparmetis (if you are using parmetis 3.X or older).
#ORDERINGSF = -Dscotch -Dmetis -Dpord -Dptscotch -Dparmetis
ORDERINGSF  = -Dpord -Dmetis
ORDERINGSC  = $(ORDERINGSF)

LGKLIBDIR = /usr/local/lib
IGKLIB    = -I/usr/local/include
LGKLIB    = -L$(LGKLIBDIR) -lGKlib

LORDERINGS     = $(LMETIS) $(LGKLIB) $(LPORD)
IORDERINGSF    = $(IMETIS) $(IGKLIB)
IORDERINGSC    = $(IMETIS) $(IGKLIB) $(IPORD)

#End orderings
########################################################################

########################################################################
# DEFINE HERE SOME COMMON COMMANDS, THE COMPILER NAMES, ETC...

# PLAT : use it to add a default suffix to the generated libraries
PLAT    = 
# Suffix for libraries, -soname and -fPIC options, C and Fortran "-o" option
# may be adapted
LIBEXT_SHARED  = .so
SONAME = -soname
SHARED_OPT = -shared
FPIC_OPT = -fPIC
# Adapt/uncomment RPATH_OPT to avoid modifying
# LD_LIBRARY_PATH in case of shared libraries
# RPATH_OPT = -Wl,-rpath,/path/to/MUMPS_x.y.z/lib/
LIBEXT  = .a
OUTC    = -o 
OUTF    = -o 
# RM : remove files
RM      = /bin/rm -f
# CC : C compiler
CC      = gcc
# FC : Fortran 90 compiler
FC      = gfortran
# FL : Fortran linker
FL      = gfortran
# AR : Archive object in a library
#      keep a space at the end if options have to be separated from lib name
AR      = ar vr 
# RANLIB : generate index of an archive file
#   (optionnal use "RANLIB = echo" in case of problem)
RANLIB  = ranlib
#RANLIB  = echo

# DEFINE HERE YOUR LAPACK LIBRARY

LAPACK = /usr/local/lib/liblapack.a

# GK commented these lines because this is the sequential version

# # SCALAP should define the SCALAPACK and  BLACS libraries.
# SCALAP  = -lscalapack -lblacs

# # INCLUDE DIRECTORY FOR MPI
# INCPAR  = -I/usr/include

# # LIBRARIES USED BY THE PARALLEL VERSION OF MUMPS: $(SCALAP) and MPI
# LIBPAR  = $(SCALAP) $(LAPACK) -L/usr/lib -lmpi

# The parallel version is not concerned by the next two lines.
# They are related to the sequential library provided by MUMPS,
# to use instead of ScaLAPACK and MPI.
INCSEQ  = -I$(topdir)/libseq
LIBSEQ  = $(LAPACK) -L$(topdir)/libseq -lmpiseq$(PLAT)

# DEFINE HERE YOUR BLAS LIBRARY

LIBBLAS = /usr/local/lib/libblas.a

# DEFINE HERE YOUR PTHREAD LIBRARY
LIBOTHERS = -lpthread

# FORTRAN/C COMPATIBILITY:
#  Use:
#    -DAdd_ if your Fortran compiler adds an underscore at the end
#              of symbols,
#     -DAdd__ if your Fortran compiler adds 2 underscores,
#
#     -DUPPER if your Fortran compiler uses uppercase symbols
#
#     leave empty if your Fortran compiler does not change the symbols.
#

CDEFS = -DAdd_

# Critical flags
INT64FLAG = -DINTSIZE64
PORD64FLAG = -DPORD_INTSIZE64
OPENMP64FLAG = -DWORKAROUNDINTELILP64OPENMPLIMITATION
GEMMTFLAG = -DGEMMT_AVAILABLE

# Compiler flags
OPTC = -O2 -fopenmp -fPIC -g $(INT64FLAG) $(PORD64FLAG)
OPTF = -O2 -fopenmp -fPIC -fdefault-integer-8 -fallow-argument-mismatch -cpp \
        $(INT64FLAG) $(OPENMP64FLAG) $(GEMMTFLAG)
OPTL = -O2 -fopenmp -fPIC -fdefault-integer-8 $(INT64FLAG)


# CHOOSE BETWEEN USING THE SEQUENTIAL OR THE PARALLEL VERSION.

#Sequential:
INCS = $(INCSEQ)
# LIBS = $(LIBSEQ)

# OpenBLAS ILP64 link line
LIBS = -L/usr/local/openblas-ilp64/lib -lopenblas \
       -L$(topdir)/libseq -lmpiseq$(PLAT) \
       $(LORDERINGS) -lpthread -lgomp -lm -ldl

LIBSEQNEEDED = libseqneeded

# #Parallel:
# INCS = $(INCPAR)
# LIBS = $(LIBPAR)
# LIBSEQNEEDED = 
