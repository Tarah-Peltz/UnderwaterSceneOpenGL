#ifndef FishRender
#define FishRender

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

void fishBody(double x,double y,double z, double r, unsigned int fishScales);
void lips(double x, double y, double z, double r, unsigned int lipTexture);
void fin(unsigned int finTexture);
void dorsal(unsigned int finTexture);
void fish(unsigned int scaleTexture, unsigned int eyeTexture, unsigned int lipTexture, unsigned int finTexture);

#ifdef __cplusplus
}
#endif

#endif
