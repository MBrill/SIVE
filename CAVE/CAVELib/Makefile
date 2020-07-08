# ----------------------------------------------------------------
#  Makefile for the sive project
# ----------------------------------------------------------------
#  $RCSfile: Makefile,v $
#  $Revision: 1.17 $
#  $Date: 2002/06/11 22:06:48 $
# ----------------------------------------------------------------
CC = CC
CXX = CC
# cut and paste from system.make
DEBUG         = -O2
USER_CXXFLAGS =  -I. ${DEBUG} 

X_PRE_LIBS    =  -lSM -lICE
X_EXTRA_LIBS  =  -lXi
XLIBS         =

GRAPHICS_API_FLAGS   =   
GRAPHICS_API_LIBS    =   -lGL

PERFORMER_LIBS       = -lpfdu -lpfutil -lpf

VTK_LIB_DIR          = -L/usr/local/vtk/lib
#VTK_LIB_DIR         = -L../../lib

# 
#VTK_LIBS = -lVTKImaging -lVTKGraphics -lVTKCommon
VTK_LIBS = -lVTKGraphics -lVTKCommon

# The cave and performer stuff
CAVE_LIB_DIR = -L/usr/local/CAVE/lib32
CAVE_LIB = -lpfcave_ogl_mips3
CAVE_INCS = -I/usr/local/CAVE/include

VTK_TO_PF_INC = -I.
VTK_TO_PF_LIB_DIR = -L.
VTK_TO_PF_LIB = -lvtkActorToPF

CPPFLAGS = ${USER_CXXFLAGS} ${CAVE_INCS} ${VTK_TO_PF_INC} \
${GRAPHICS_API_FLAGS} -I/usr/local/vtk/src/common \
-I/usr/local/vtk/src/graphics -I/usr/local/vtk/src/contrib -I/usr/local/vtk/src/imaging

CXXFLAGS = ${CPPFLAGS}

INTERLIB = interrogation.a
INTERLIBNAME = lib${INTERLIB}
INTERLIBFLAG = -L. -v -linterrogation 

all : 

make : Makefile

mains : sive computeIso 

# -----------------------------------------------------------------------------
#    library
# -----------------------------------------------------------------------------
lib :
	${AR} -rv ${INTERLIBNAME} ${CLASSOBJECTS}

computeIso : computeIso.o  ${INTERLIBNAME}
	${CC} -v -o computeIso ${CPPFLAGS} computeIso.o \
	${INTERLIBFLAG} ${VTK_LIB_DIR} ${VTK_TO_PF_LIB_DIR} ${PFDB_LIBDIR} \
	${VTK_TO_PF_LIB} ${VTK_LIBS} ${PFDB} ${PERFORMER_LIBS} \
	${XLDFLAGS} ${GRPHICS_API_LIBS} ${XLIBS} -lXext -lXt \
	${X_PRE_LIBS} -lX11 -lm -lC -lpthread

sive.o : sive.C

sive : sive.o ${INTERLIBNAME} 
	${CC} -v -o sive ${DEBUG} sive.o \
        ${INTERLIBFLAG} \
	${CAVE_LIB_DIR} ${CAVE_LIB} \
	${VTK_LIB_DIR} ${VTK_LIBS} \
	${PFDB_LIBDIR} ${PFDB} ${PERFORMER_LIBS} ${GRAPHICS_API_LIBS} ${XLIBS} \
	${X_PRE_LIBS} ${X_EXTRA_LIBS} -lX11 -lm -lC -lpthread 

textures : textures.o ${INTERLIBNAME} 
	${CC} -v -o textures ${CPPFLAGS} textures.o \
        ${INTERLIBFLAG} \
	${VTK_TO_PF_LIB_DIR} ${VTK_LIB_DIR} \
	${VTK_LIBS} \
	${PFDB_LIBDIR} ${PFDB} ${PERFORMER_LIBS} ${GRAPHICS_API_LIBS} ${XLIBS} \
	${X_PRE_LIBS} ${X_EXTRA_LIBS} -lX11 -lm -lC -lpthread

computeScalars : computeScalars.o ${INTERLIBNAME}
	${CC} -v -o computeScalars ${CPPFLAGS} computeScalars.o \
        ${INTERLIBFLAG} ${VTK_TO_PF_LIB_DIR} ${PFDB_LIBDIR}\
	${PFDB} ${PERFORMER_LIBS}
	${GRAPHICS_API_LIBS} ${XLIBS} -lXext -lXt \
	${X_PRE_LIBS} -lX11 -lm -lC -lpthread


displayCar : displayCar.o ${INTERLIBNAME}
	${CC} -v -o displayCar ${CPPFLAGS} displayCar.C rwScalars.o \
        ${INTERLIBFLAG} \
	${GRAPHICS_API_LIBS} ${XLIBS} -lXext -lXt \
	${X_PRE_LIBS} -lX11 -lm -lC -lpthread

display : display.o ${INTERLIBNAME}
	${CC} -v -o displaySurface ${CPPFLAGS} display.o \
	rwScalars.o \
        ${INTERLIBFLAG} \
	${GRAPHICS_API_LIBS} ${XLIBS} -lXext -lXt \
	${X_PRE_LIBS} -lX11 -lm -lC -lpthread

caveCar : caveCar.o ${INTERLIBNAME} 
	${CC} -v -o caveCar ${CPPFLAGS} caveCar.C \
	${VTK_TO_PF_LIB_DIR} ${CAVE_LIB_DIR} ${VTK_LIB_DIR} \
	${INTERLIBFLAG} ${VTK_TO_PF_LIB} ${CAVE_LIB} ${VTK_LIBS} \
	${PERFORMER_LIBS} ${GRAPHICS_API_LIBS} ${XLIBS} \
	${X_PRE_LIBS} ${X_EXTRA_LIBS} -lX11 -lm -lC -lpthread

# -----------------------------------------------------------------------------
#    class files 
# -----------------------------------------------------------------------------
CLASSOBJECTS = LightLine.o LightVector.o \
LightCage.o TopParallelLightCage.o TopCrissCrossLightCage.o \
InterrogationLines.o HighlightLines.o ReflectionLines.o \
Isophotes.o \
InterrogationObject.o \
Room.o GeometryRoom.o TexturedRoom.o

classes : ${CLASSOBJECTS}

LightLine.o : LightLine.C LightLine.h

LightVector.o : LightVector.C LightVector.h

LightCage.o : LightCage.C LightCage.h LightLine.h LightLine.C

TopParallelLightCage.o : TopParallelLightCage.C TopParallelLightCage.h LightCage.h LightCage.C

TopCrissCrossLightCage.o : TopCrissCrossLightCage.C TopCrissCrossLightCage.h LightCage.h LightCage.C

InterrogationLines.o : InterrogationLines.C InterrogationLines.h LightCage.C LightCage.h InterrogationObject.C InterrogationObject.h

InterrogationObject.o : InterrogationObject.C InterrogationObject.h InterrogationLines.h InterrogationLines.C

HighlightLines.o : HighlightLines.C HighlightLines.h InterrogationLines.C InterrogationLines.h LightCage.h

ReflectionLines.o : ReflectionLines.C ReflectionLines.h InterrogationLines.C InterrogationLines.h  LightCage.h

Isophotes.o : Isophotes.C Isophotes.h InterrogationLines.C InterrogationLines.h LightCage.h

Room.o : Room.C Room.h InterrogationLines.C InterrogationLines.h InterrogationObject.h InterrogationObject.C HighlightLines.C HighlightLines.h ReflectionLines.C ReflectionLines.h

GeometryRoom.o : GeometryRoom.C GeometryRoom.h Room.h Room.C InterrogationLines.C InterrogationLines.h InterrogationObject.h InterrogationObject.C HighlightLines.C HighlightLines.h ReflectionLines.C ReflectionLines.h

TexturedRoom.o : TexturedRoom.C TexturedRoom.h Room.h Room.C InterrogationLines.C InterrogationLines.h InterrogationObject.h InterrogationObject.C HighlightLines.C HighlightLines.h ReflectionLines.C ReflectionLines.h

demo:
	cp sive ../../Demo
	cp libinterrogation.a ../../lib

demodata:
	cp *.vtk ../../Demo

.DEFAULT :
	echo "No rule found"

clean:
	touch _.o _~ 
	/bin/rm -f *.o 
