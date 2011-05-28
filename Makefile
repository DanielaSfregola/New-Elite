# This sample Makefile allows you to make an OpenGL application
#   whose source is exactly one .c file.
#
#
# To use this Makefile, you type:
#
#        make xxxx
#                  
# where
#       xxxx.c is the name of the file you wish to compile 
#       
# A binary named xxxx will be produced
# Libraries are assumed to be in the default search paths
# as are any required include files

CC = g++

# COMPILERFLAGS = -Wall (massimo livello di warning)
COMPILERFLAGS= -w 
# inibire tutti i messaggi warning: -w 
INCLUDE = -I/usr/include/GL -I.
# Any directories you specify with `-I' options before  the  `-I-' option are searched only for the case of `#include "file"'; 
# they are not searched for `#include <file>'.
# With  `-I.'  you  can  specify searching the directory which was current when the compiler was invoked.  
# That is not exactly the same as what the preprocessor does by default, but it is often satisfactory.


LIBDIR = -L. -L/usr/X11R6/lib
LDLIBS = -lglut -lGL -lGLU -lXmu -lX11 -lm


.c:
	$(CC) $(COMPILERFLAGS) $(INCLUDE) $(LIBDIR) $@.c lib/eliteutils.c -o $@ $(LDLIBS)

clear:
	rm -f elite

clearall:
	rm -f *~
	rm -f lib/*~
	rm -f doc/*~; rm -f doc/*.aux; rm -f doc/*.log; rm -f doc/*.out; rm -f doc/*.toc
	rm -f doc/Enrico/*~  
	rm -f doc/Daniela/*~
	rm -f doc/Alberto/*~
