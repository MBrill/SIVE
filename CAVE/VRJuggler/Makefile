# ----------------------------------------------------------------
#  Author : mb
#  History: 13/01/2004 created 
# ----------------------------------------------------------------
CXX = g++
# 
DEBUG         = 
VISLABHEADER    = -I/usr/local/include
#XXFLAGS =  -I. ${DEBUG} -mwin32 
# no-deprecated wird bei VTK benötigt!
CXXFLAGS =  -I. -I/usr/local/include/vtk ${VISLABHEADER} ${DEBUG} -Wno-deprecated
RM = rm -f

VISLABDIR    = -L/usr/local/lib
VISLABLIB = ${VISLABDIR} -lvlg -lvl
VTK_LIBDIR    = -L/usr/local/lib/vtk
VTKLIBS = ${VTK_LIBDIR} -lvtkRendering -lvtkGraphics -lvtkImaging -lvtkIO -lvtkFiltering -lvtkCommon 
OGL_LIBS   = -lglut32 -lglu32 -lopengl32 

all : siveMain

siveMain.o : siveMain.cpp
	${CXX} -c ${CXXFLAGS} $<

siveMain : siveMain.o SiveEngine.o InterrogationObject.o LightLine.o LightVector.o LightCage.o TopParallelLightCage.o InterrogationLines.o Isophotes.o
	${CXX} -o $@ ${CXXFLAGS} $< SiveEngine.o InterrogationObject.o LightLine.o  LightVector.o  LightCage.o  TopParallelLightCage.o  InterrogationLines.o  Isophotes.o ${VISLABLIB} ${VTKLIBS} ${OGL_LIBS} -lgdi32 -lm

SiveEngine.o : SiveEngine.cpp SiveEngine.h
	${CXX} -c ${CXXFLAGS} $<

InterrogationObject.o : InterrogationObject.cpp InterrogationObject.h
	${CXX} -c ${CXXFLAGS} $<

LightLine.o : LightLine.cpp LightLine.h
	${CXX} -c ${CXXFLAGS} $<

LightVector.o : LightVector.cpp LightVector.h
	${CXX} -c ${CXXFLAGS} $<

LightCage.o : LightCage.cpp LightCage.h
	${CXX} -c ${CXXFLAGS} $<

TopParallelLightCage.o : TopParallelLightCage.cpp TopParallelLightCage.h LightCage.h LightCage.cpp
	${CXX} -c ${CXXFLAGS} $<

InterrogationLines.o : InterrogationLines.cpp InterrogationLines.h
	${CXX} -c ${CXXFLAGS} $<

Isophotes.o : Isophotes.cpp Isophotes.h InterrogationLines.h InterrogationLines.cpp
	${CXX} -c ${CXXFLAGS} $<

clean:
	/bin/rm -f *.o *~

viclean:
	/bin/rm -f *~
