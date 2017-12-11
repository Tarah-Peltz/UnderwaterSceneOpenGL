#ifndef CrabRender
#define CrabRender

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

#ifdef USEGLEW
#include <GL/glew.h>
#endif
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif

void crabBody(double x,double y,double z,double r, unsigned int crabShell); 
void claw(unsigned int crabBody);
void crab(unsigned int crabShell, unsigned int eyeTexture);

#ifdef __cplusplus
}
#endif

#endif
