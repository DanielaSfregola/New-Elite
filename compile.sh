#!/bin/bash
CC="g++"
#COMPILERFLAGS="-Wall"
COMPILERFLAGS="-w" 
INCLUDE="-I/usr/include/GL -I."
LIBDIR="-L. -L/usr/X11R6/lib"
LDLIBS="-lglut -lGL -lGLU -lX11 -lm"
# -lXmu
rm -f melite
$CC $COMPILERFLAGS $INCLUDE $LIBDIR elite.c lib/eliteutils.c -o elite $LDLIBS
