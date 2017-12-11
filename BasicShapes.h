#ifndef BasicShapes
#define BasicShapes

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


#ifdef  MAIN_FILE
float normalX;
float normalY;
float normalZ;
int causticsPass;
int diffuse;
int specular;
#else
extern float normalX;
extern float normalY;
extern float normalZ;
extern int causticsPass;
extern int diffuse;
extern int specular;
#endif


#ifdef __cplusplus
extern "C" {
#endif

void normalV(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3,  double z3);
void shinyMetal(void);
void notShiny(void);
void Sphere(int textureIndex, int scaleFactor, unsigned int tex, float degrees);
void drawCylinder(float height, float radius, unsigned int crabTexture);
void eyeball(double x,double y,double z,double r, unsigned int eyeTexture);
void disc(float radius, unsigned int texture);

#ifdef __cplusplus
}
#endif

#endif