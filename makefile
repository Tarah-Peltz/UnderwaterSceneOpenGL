# Final Project
EXE=FinalProject

# Main target
all: $(EXE)

#  MinGW
ifeq "$(OS)" "Windows_NT"
CFLG=-O3 -Wall
LIBS=-lglut32cu -lglu32 -lopengl32
CLEAN=del *.exe *.o *.a
else
#  OSX
ifeq "$(shell uname)" "Darwin"
CFLG=-O3 -Wall -Wno-deprecated-declarations
LIBS=-framework GLUT -framework OpenGL
#  Linux/Unix/Solaris
else
CFLG=-O3 -Wall
LIBS=-lglut -lGLU -lGL -lm
endif
#  OSX/Linux/Unix/Solaris
CLEAN=rm -f $(EXE) *.o *.a
endif

# Dependencies
FinalProject.o: FinalProject.cpp CSCIx229.h
fatal.o: fatal.cpp CSCIx229.h
loadtexbmp.o: loadtexbmp.cpp CSCIx229.h
print.o: print.cpp CSCIx229.h
project.o: project.cpp CSCIx229.h
errcheck.o: errcheck.cpp CSCIx229.h
object.o: object.cpp CSCIx229.h
BasicShapes.o: BasicShapes.cpp BasicShapes.h



#  Create archive
CSCIx229.a:fatal.o loadtexbmp.o print.o project.o errcheck.o object.o
	ar -rcs $@ $^
BasicShapes.a:BasicShapes.o
	ar -rcs $@ $^

# Compile rules
.c.o:
	gcc -c $(CFLG) $<
.cpp.o:
	g++ -c $(CFLG) $<

#  Link
FinalProject:FinalProject.o CSCIx229.a BasicShapes.a
	g++ -O1 -o $@ $^   $(LIBS)

#  Clean
clean:
	$(CLEAN)
