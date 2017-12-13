/*
Referenced http://www.swiftless.com/tutorials/opengl/camera2.html heavily for first person
 */

#define MAIN_FILE
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "CSCIx229.h"
#include "FishRender.h"
#include "BasicShapes.h"
#include "CrabRender.h"


int mode=0;       //  Texture mode
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int axes=1;       //  Display axes
double PI = 3.14159;

int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=5.0;   //  Size of world
double t;

float posX = 0;
float posY = 0;
float posZ = 0;

float velocityX = 0;
float velocityY = 0;
float velocityZ = 0;
float TOP_SPEED = 0.00003;
float SPEED_DIVIDER = .001;

// Light values
int movingLight = 1;
int light=1;      //  Lighting
int one       =   1;  // Unit value
int distance  =   5;  // Light distance
int inc       =   3;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  30;  // Ambient intensity (%)
int shininess =   0;  // Shininess (power of two)
float shiny   =   1;  // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light

float causticScale = 1.5;

unsigned int texture[20]; // Texture names
unsigned int caustics[16];
unsigned int sky[6]; //skybox
unsigned int skybox2;
double worldDimensions = 15;

int causticIndex = 0;
float causticCounter = 0;

float coralPoints[ 45 ][ 45 ][3];//The Array For The Points On The Grid Of Our "Wave"
float seaweedPoints[ 45 ][ 45 ][3];//The Array For The Points On The Grid Of Our "Wave"
float hold;//Temporarily Holds A Floating Point Value

typedef struct bubble {
   float xPosition;
   float yPosition;
   float zPosition;
} Bubble;

#define NUMBER_OF_BUBBLES 30

Bubble bubbleArray[NUMBER_OF_BUBBLES];

int objFiles[5];

float lastTime = 0;

#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

float float_rand(float min, float max) //https://stackoverflow.com/questions/13408990/how-to-generate-random-float-number-in-c
{
    float scale = rand() / (float) RAND_MAX; /* [0, 1.0] */
    return min + scale * ( max - min );      /* [min, max] */
}

static void Sky(double D, int pass)
{
   glColor3f(1,1,1);
   //if (causticsPass == 0) {
   glEnable(GL_TEXTURE_2D);

   //  Sides
   if (causticsPass == 0) {
      glBindTexture(GL_TEXTURE_2D,sky[0]);
   }
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(-D,0,-D);
   glTexCoord2f(1,0); glVertex3f(+D,0,-D);
   glTexCoord2f(1,1); glVertex3f(+D*5,+3*D,-D*5);
   glTexCoord2f(0,1); glVertex3f(-D*5,+3*D,-D*5);
   glEnd();

   if (causticsPass == 0){
      glBindTexture(GL_TEXTURE_2D,sky[1]);
   }
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(+D,0,-D);
   glTexCoord2f(1,0); glVertex3f(+D,0,+D);
   glTexCoord2f(1,1); glVertex3f(+D*5,+3*D,+D*5);
   glTexCoord2f(0,1); glVertex3f(+D*5,+3*D,-D*5);
   glEnd();

   if (causticsPass == 0){
      glBindTexture(GL_TEXTURE_2D,sky[2]);
   }
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(+D,0,+D);
   glTexCoord2f(1,0); glVertex3f(-D,0,+D);
   glTexCoord2f(1,1); glVertex3f(-D*5,+3*D,+D*5);
   glTexCoord2f(0,1); glVertex3f(+D*5,+3*D,+D*5);
   glEnd();

   if (causticsPass == 0){
      glBindTexture(GL_TEXTURE_2D,sky[3]);
   }
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(-D,0,+D);
   glTexCoord2f(1,0); glVertex3f(-D,0,-D);
   glTexCoord2f(1,1); glVertex3f(-D*5,+3*D,-D*5);
   glTexCoord2f(0,1); glVertex3f(-D*5,+3*D,+D*5);
   glEnd();

   //  Top and bottom
   if (causticsPass == 0) {
      glBindTexture(GL_TEXTURE_2D,sky[4]);
      glBegin(GL_QUADS);
      glTexCoord2f(1,1); glVertex3f(+D*5,+3*D,-D*5);
      glTexCoord2f(1,0); glVertex3f(+D*5,+3*D,+D*5);
      glTexCoord2f(0,0); glVertex3f(-D*5,+3*D,+D*5);
      glTexCoord2f(0,1); glVertex3f(-D*5,+3*D,-D*5);
      glEnd();
   }


   if (causticsPass == 0){
      glBindTexture(GL_TEXTURE_2D,sky[5]);
   }

   glBegin(GL_QUADS);
   glTexCoord2f(1,1); glVertex3f(-D,0,+D);
   glTexCoord2f(0,1); glVertex3f(+D,0,+D);
   glTexCoord2f(0,0); glVertex3f(+D,0,-D);
   glTexCoord2f(1,0); glVertex3f(-D,0,-D);
   glEnd();
   if (causticsPass == 0)
      glDisable(GL_TEXTURE_2D);
}


void oyster(unsigned int outerTexture, unsigned int innerTexture, unsigned int pearlTexture) {
   glColor3f(.8, .8, .8);
   //Upper shell outside
   notShiny();
   glPushMatrix();
   glTranslatef(0.01, 0, -.01);
   glRotatef(-45, 0, 1, 0);
   glScalef(.35, .35, .95);
   Sphere(1, 1, outerTexture, 180);
   glPopMatrix();

   //Upper shell inside
   shinyMetal();
   glPushMatrix();
   glTranslatef(0, 0, -.01);
   glRotatef(-45, 0, 1, 0);
   glScalef(.3, .3, .9);
   Sphere(1, 1, innerTexture, 180);
   glPopMatrix();

   //Lower shell outside
   notShiny();
   glPushMatrix();
   glTranslatef(0.01, -0.3, .2);
   glRotatef(-45, 0, 1, 0);
   glRotatef(90, 0, 0, 1);
   glScalef(.15, .35, .85);
   Sphere(1, 1, outerTexture, 180);
   glPopMatrix();

   //Lower shell inside
   shinyMetal();
   glPushMatrix();
   glTranslatef(0, -0.3, .2);
   glRotatef(-45, 0, 1, 0);
   glRotatef(90, 0, 0, 1);
   glScalef(.10, .3, .8);
   Sphere(1, 1, innerTexture, 180);
   glPopMatrix();

   //Pearl
   shinyMetal();
   glPushMatrix();
   glTranslatef(0, -.3, .1);
   glScalef(.15, .15, .15);
   Sphere(1, 1, pearlTexture, 360);
   glPopMatrix();

   glColor3f(1, 1, 1);
 }

void getModelView() { //Billboarding code from here: http://www.lighthouse3d.com/opengl/billboarding/index.php?billCheat
   float modelview[16];
   int i,j;
   glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

   // undo all rotations
   // beware all scaling is lost as well 
   for( i=0; i<3; i++ ) 
      for( j=0; j<3; j++ ) {
         if ( i==j )
            modelview[i*4+j] = 1.0;
         else
            modelview[i*4+j] = 0.0;
      }

   // set the modelview with no rotations and scaling
   glLoadMatrixf(modelview);
}

void bubbles() {

   // save the current modelview matrix
   glPushMatrix();
   glEnable(GL_TEXTURE_2D);
   // get the current modelview matrix
   getModelView();
   glBindTexture(GL_TEXTURE_2D,texture[10]);
   glTexEnvf(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glDepthMask(GL_FALSE);
   glEnable(GL_BLEND);
   //glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE);
   int i = 0;

   for (i = 0; i < NUMBER_OF_BUBBLES; i++) {
      glBegin(GL_QUADS);
      glTexCoord2f(0,0); glVertex3f(bubbleArray[i].xPosition - .1, bubbleArray[i].yPosition - .1, bubbleArray[i].zPosition);
      glTexCoord2f(1,0); glVertex3f(bubbleArray[i].xPosition + .1, bubbleArray[i].yPosition - .1, bubbleArray[i].zPosition);
      glTexCoord2f(1,1); glVertex3f(bubbleArray[i].xPosition + .1, bubbleArray[i].yPosition + .1, bubbleArray[i].zPosition);
      glTexCoord2f(0,1); glVertex3f(bubbleArray[i].xPosition - .1, bubbleArray[i].yPosition + .1, bubbleArray[i].zPosition);
      glEnd();
   }
      glDisable(GL_TEXTURE_2D);
      glDisable(GL_BLEND);
      glDepthMask(GL_TRUE);
   

   // restores the modelview matrix
   glPopMatrix();

}


void seaweedInit() {
   for (int x = 0; x < 45; x++) {
      for(int y = 0; y < 45; y++) {
         seaweedPoints[x][y][0]= (x/5.0f)+4.5f;
         seaweedPoints[x][y][1]= (y/5.0f)-.2f;
         //seaweedPoints[x][y][2]= (x/5.0f)+4.5f;
         seaweedPoints[x][y][2]= sin((((x/5.0f)*40.0f)/360.0f)*3.141592654);

         coralPoints[x][y][0]= (x/8.0f)-6.5f;
         coralPoints[x][y][1]= (y/8.0f)-.2f;
         coralPoints[x][y][2]= sin((((x/8.0f)*40.0f)/360.0f)*3.141592654);
      }
   }
}

//Flag Effect from here: http://nehe.gamedev.net/tutorial/flag_effect_(waving_texture)/16002/
void seaweed() {
      float Intensity = 2.0;
   glPushMatrix();
   glTexEnvf(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glDepthMask(GL_FALSE);
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
   float random = 1.0;

   glBindTexture(GL_TEXTURE_2D,texture[11]);
   glBegin(GL_QUADS);
   for (int x = 0; x < 44; x++) {
      for (int y = 0; y < 44; y++) {
         /*glTexCoord2f(x/44.0, y/44.0); 
         glVertex3f(seaweedPoints[x][y][0] + Intensity * (cos( glutGet(GLUT_ELAPSED_TIME)/1.5 ) * (1.0 - y/44.0)) * 
            sin(seaweedPoints[x][y][0]), 
            seaweedPoints[x][y][1] + 0.25 * sin( 1.5*seaweedPoints[x][y][0] + 2.0*seaweedPoints[x][y][2] )* (1.0 - y/44.0), 
            seaweedPoints[x][y][2] + Intensity * (cos( glutGet(GLUT_ELAPSED_TIME)/2.0 ) * (1.0 - y/44.0)) * 
            sin( seaweedPoints[x][y][2] + 2.0*random));
         glTexCoord2f(x/44.0, (y+1)/44.0); 
         glVertex3f(seaweedPoints[x][y+1][0] + Intensity * (cos( glutGet(GLUT_ELAPSED_TIME)/1.5 ) * (1.0 - (y+1)/44.0)) * 
            sin(seaweedPoints[x][y + 1][0]), 
            seaweedPoints[x][y+1][1] + 0.25 * sin( 1.5*seaweedPoints[x][y+1][0] + 2.0*seaweedPoints[x][y+1][2] )* (1.0 - (y+1)/44.0), 
            seaweedPoints[x][y+1][2]  + Intensity * (cos( glutGet(GLUT_ELAPSED_TIME)/2.0 ) * (1.0 - (y+1)/44.0) * 
               sin( seaweedPoints[x][y+1][2] + 2.0*random)));
         glTexCoord2f((x+1)/44.0, (y+1)/44.0); 
         glVertex3f(seaweedPoints[x+1][y+1][0] + Intensity * (cos( glutGet(GLUT_ELAPSED_TIME)/1.5 ) * (1.0 - (y + 1)/44.0)) * 
            sin(seaweedPoints[x + 1][y + 1][0]), 
            seaweedPoints[x+1][y+1][1] + 0.25 * sin( 1.5*seaweedPoints[x+1][y+1][0] + 2.0*seaweedPoints[x+1][y+1][2] )* (1.0 - (y+1)/44.0), 
            seaweedPoints[x+1][y+1][2]  + Intensity * (cos( glutGet(GLUT_ELAPSED_TIME)/2.0 ) * (1.0 - (y+1)/44.0)) * 
            sin(seaweedPoints[x+1][y+1][2] + 2.0*random));
         glTexCoord2f((x+1)/44.0, (y)/44.0); 
         glVertex3f(seaweedPoints[x+1][y][0]+ Intensity * (cos( glutGet(GLUT_ELAPSED_TIME)/1.5 ) * (1.0 - y/44.0)) * 
            sin(seaweedPoints[x+1][y][0]), 
            seaweedPoints[x+1][y][1] + 0.25 * sin( 1.5*seaweedPoints[x+1][y][0] + 2.0*seaweedPoints[x+1][y][2] )* (1.0 - y/44.0), 
            seaweedPoints[x+1][1][2]  + Intensity * (cos( glutGet(GLUT_ELAPSED_TIME)/2.0 ) * (1.0 - y/44.0)) * 
            sin( seaweedPoints[x+1][y][2] + 2.0*random));*/
         //if (y > 10) {
            glTexCoord2f(x/44.0, y/44.0); glVertex3f(seaweedPoints[x][y][0], seaweedPoints[x][y][1], seaweedPoints[x][y][2] * y/44 * (abs(x-22) + 10)/30 * sin((((int)glutGet(GLUT_ELAPSED_TIME) * 2 * 3.14159 / 4000))));
            glTexCoord2f(x/44.0, (y+1)/44.0); glVertex3f(seaweedPoints[x][y+1][0], seaweedPoints[x][y+1][1], seaweedPoints[x][y+1][2] * (y+1)/44 * (abs(x-22) + 10)/30 * sin((((int)glutGet(GLUT_ELAPSED_TIME) * 2 * 3.14159 / 4000))));
            glTexCoord2f((x+1)/44.0, (y+1)/44.0); glVertex3f(seaweedPoints[x+1][y+1][0], seaweedPoints[x+1][y+1][1], seaweedPoints[x+1][y+1][2] * (y+1)/44 * (abs(x+1-22) + 10)/30 * sin((((int)glutGet(GLUT_ELAPSED_TIME) * 2 * 3.14159 / 4000))));
            glTexCoord2f((x+1)/44.0, (y)/44.0); glVertex3f(seaweedPoints[x+1][y][0], seaweedPoints[x+1][y][1], seaweedPoints[x+1][1][2] * y/44 * (abs(x+1-22) + 10)/30 * sin((((int)glutGet(GLUT_ELAPSED_TIME) * 2 * 3.14159 / 4000))));
      }
   }
   
   glEnd();

   glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   for (int x = 0; x < 44; x++) {
      for (int y = 0; y < 44; y++) {
         
         glTexCoord2f(x/44.0, y/44.0); glVertex3f(coralPoints[x][y][0], coralPoints[x][y][1], coralPoints[x][y][2]);
         glTexCoord2f(x/44.0, (y+1)/44.0); glVertex3f(coralPoints[x][y+1][0], coralPoints[x][y+1][1], coralPoints[x][y+1][2]);
         glTexCoord2f((x+1)/44.0, (y+1)/44.0); glVertex3f(coralPoints[x+1][y+1][0], coralPoints[x+1][y+1][1], coralPoints[x+1][y+1][2]);
         glTexCoord2f((x+1)/44.0, (y)/44.0); glVertex3f(coralPoints[x+1][y][0], coralPoints[x+1][y][1], coralPoints[x+1][1][2]);
      }
   }
   glEnd();
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_BLEND);
   glDepthMask(GL_TRUE);

   glPopMatrix();
}

/*void seaweedInit() {
   for (int x = 0; x < 45; x++) {
      for(int y = 0; y < 45; y++) {
         seaweedPoints[x][y][0]= (x/5.0f)+4.5f;
         seaweedPoints[x][y][1]= (y/5.0f)-.2f;
         seaweedPoints[x][y][2]= sin((((x/5.0f)*40.0f)/360.0f)*3.141592654);

         coralPoints[x][y][0]= (x/8.0f)-6.5f;
         coralPoints[x][y][1]= (y/8.0f)-.2f;
         coralPoints[x][y][2]= sin((((x/8.0f)*40.0f)/360.0f)*3.141592654);
      }
   }
}

//Flag Effect from here: http://nehe.gamedev.net/tutorial/flag_effect_(waving_texture)/16002/
void seaweed() {

   glPushMatrix();
   glTexEnvf(GL_TEXTURE_2D,GL_TEXTURE_ENV_MODE,GL_MODULATE);
   glDepthMask(GL_FALSE);
   glEnable(GL_BLEND);
   glBlendFunc(GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
   

   glBindTexture(GL_TEXTURE_2D,texture[11]);
   glBegin(GL_QUADS);
   for (int x = 0; x < 44; x++) {
      for (int y = 0; y < 44; y++) {
         glTexCoord2f(x/44.0, y/44.0); glVertex3f(seaweedPoints[x][y][0], seaweedPoints[x][y][1], seaweedPoints[x][y][2]);
         glTexCoord2f(x/44.0, (y+1)/44.0); glVertex3f(seaweedPoints[x][y+1][0], seaweedPoints[x][y+1][1], seaweedPoints[x][y+1][2]);
         glTexCoord2f((x+1)/44.0, (y+1)/44.0); glVertex3f(seaweedPoints[x+1][y+1][0], seaweedPoints[x+1][y+1][1], seaweedPoints[x+1][y+1][2]);
         glTexCoord2f((x+1)/44.0, (y)/44.0); glVertex3f(seaweedPoints[x+1][y][0], seaweedPoints[x+1][y][1], seaweedPoints[x+1][1][2]);
      }
   }
   glEnd();

   glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   for (int x = 0; x < 44; x++) {
      for (int y = 0; y < 44; y++) {
         
         glTexCoord2f(x/44.0, y/44.0); glVertex3f(coralPoints[x][y][0], coralPoints[x][y][1], coralPoints[x][y][2]);
         glTexCoord2f(x/44.0, (y+1)/44.0); glVertex3f(coralPoints[x][y+1][0], coralPoints[x][y+1][1], coralPoints[x][y+1][2]);
         glTexCoord2f((x+1)/44.0, (y+1)/44.0); glVertex3f(coralPoints[x+1][y+1][0], coralPoints[x+1][y+1][1], coralPoints[x+1][y+1][2]);
         glTexCoord2f((x+1)/44.0, (y)/44.0); glVertex3f(coralPoints[x+1][y][0], coralPoints[x+1][y][1], coralPoints[x+1][1][2]);
      }
   }
   glEnd();

   int x, y = 0;
   for(y = 0; y < 45; y++ )
   {
      hold=seaweedPoints[0][y][2];
      for(x = 0; x < 44; x++)
      {
         seaweedPoints[x][y][2] = seaweedPoints[x+1][y][2];
      }
      seaweedPoints[44][y][2]=hold;
   }
   glDisable(GL_TEXTURE_2D);
   glDisable(GL_BLEND);
   glDepthMask(GL_TRUE);

   glPopMatrix();
}*/

void coralTubes(int texture) {
   glPushMatrix();
   glTranslatef(2., 0, -2.);
   glRotatef(10, 1, 0, 0);
   glRotatef(40, 0, 1, 0);
   glScalef(.7, .8, .8);
   drawCylinder(1.0, .5, texture);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(2., 0, -2.);
   glRotatef(40, 0, 1, 0);
   glScalef(.7, 1, .8);
   disc(.5, texture);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(2.2, 0, -2.5);
   glScalef(.5, 1, .5);
   drawCylinder(1.0, .5, texture);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(2.2, 0, -2.5);
   glScalef(.5, 1, .5);
   disc(.5, texture);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(1.7, 0, -1.6);
   glRotatef(30, 1, 0, 0);
   glRotatef(10, 0, 0, 1);
   glScalef(.6, .3, .4);
   drawCylinder(1.0, .5, texture);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(1.7, .1, -1.6);
   glScalef(.55, 1, .35);
   disc(.5, texture);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(2.4, 0, -2.1);
   glRotatef(-10, 1, 0, 0);
   glRotatef(45, 0, 1, 0);
   glRotatef(-10, 0, 0, 1);
   glScalef(.4, .4, .6);
   drawCylinder(1.0, .5, texture);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(2.4, 0, -2.1);
   glRotatef(45, 0, 1, 0);
   glScalef(.4, 1, .6);
   disc(.5, texture);
   glPopMatrix();


   glPushMatrix();
   glTranslatef(2.3, 0, -1.7);
   glRotatef(10, 1, 0, 0);
   glRotatef(75, 0, 1, 0);
   glRotatef(-10, 0, 0, 1);
   glScalef(.3, .7, .4);
   drawCylinder(1.0, .5, texture);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(2.3, 0, -1.7);
   glRotatef(75, 0, 1, 0);
   glScalef(.3, 1, .4);
   disc(.5, texture);
   glPopMatrix();


   glPushMatrix();
   glTranslatef(1.7, 0, -2.4);
   glRotatef(10, 1, 0, 0);
   glRotatef(75, 0, 1, 0);
   glRotatef(-20, 0, 0, 1);
   glScalef(.3, .4, .4);
   drawCylinder(1.0, .5, texture);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(1.7, 0, -2.4);
   glRotatef(75, 0, 1, 0);
   glScalef(.3, 1, .4);
   disc(.5, texture);
   glPopMatrix();

}

void firstPass() { //referenced for caustics: https://www.opengl.org/archives/resources/code/samples/mjktips/caustics/
   causticsPass = 0;
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_MODULATE:GL_REPLACE);
   notShiny();

   glPushMatrix();
   glTranslatef(5, 1, -2);
   oyster(texture[7], texture[8], texture[9]);
   glPopMatrix();

   Sky(worldDimensions, 0);

   /*glPushMatrix();
   glTranslatef(7, 6, -1);
   glRotatef(40, 0, 1, 0);
   glRotatef(10, 0, 0, 1);
   glScalef(.5, .5, .5);
   fish(texture[4], texture[1], texture[6], texture[5]);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(-8, 5, -1.5);
   glRotatef(-90, 0, 1, 0);
   glRotatef(-40, 0, 0, 1);
   glScalef(.2, .2, .2);
   fish(texture[4], texture[1], texture[6], texture[5]);
   glPopMatrix();*/

   glPushMatrix();
   coralTubes(texture[12]);
   glPopMatrix();

   /*glPushMatrix();
   glTranslatef(1, 4, -.5);
   glRotatef(-10, 0, 1, 0);
   glScalef(.8, .8, .8);
   crab(texture[3], texture[1]);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(-3.5, 1, 0);
   glRotatef(-45, 0, 1, 0);
   glScalef(.5, .5, .5);
   crab(texture[3], texture[1]);
   glPopMatrix();*/
}


void secondPass() { //referenced for caustics: https://www.opengl.org/archives/resources/code/samples/mjktips/caustics/
   causticsPass = 1;
   
   /* Disable depth buffer update and exactly match depth
   buffer values for slightly faster rendering. */
   glDepthMask(GL_FALSE);
   glDepthFunc(GL_EQUAL);

   /* Multiply the source color (from the caustic luminance
   texture) with the previous color from the normal pass.  The
   caustics are modulated into the scene. */
   glEnable(GL_BLEND);
   //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glBlendFunc(GL_DST_COLOR, GL_ONE);
//glBlendEquation(GL_MAX);


   /* The 0.03 in the Y column is just to shift the texture coordinates
   a little based on Y (depth in the water) so that vertical faces
   (like on the cube) do not get totally vertical caustics. */
   GLfloat sPlane[4] = { 0.05, 0.03, 0.0, 0.0 };
   GLfloat tPlane[4] = { 0.0, 0.03, 0.05, 0.0 };

   /* The causticScale determines how large the caustic "ripples" will
   be.  See the "Increate/Decrease ripple size" menu options. */

   sPlane[0] = 0.05 * causticScale;
   sPlane[1] = 0.03 * causticScale;

   tPlane[1] = 0.03 * causticScale;
   tPlane[2] = 0.05 * causticScale;


   /* Set current color to "white" and disable lighting
   to emulate OpenGL 1.1's GL_REPLACE texture environment. */
   glColor3f(1.0, 1.0, 1.0);
   glDisable(GL_LIGHTING);

   /* Generate the S & T coordinates for the caustic textures
   from the object coordinates. */

   glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
   glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
   glTexGenfv(GL_S, GL_OBJECT_PLANE, sPlane);
   glTexGenfv(GL_T, GL_OBJECT_PLANE, tPlane);
   glEnable(GL_TEXTURE_GEN_S);
   glEnable(GL_TEXTURE_GEN_T);

   glColor3f(.8, .6, .6);
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_MODULATE:GL_REPLACE);
   glBindTexture(GL_TEXTURE_2D, caustics[causticIndex]);
   Sky(worldDimensions, 1);
   notShiny();

   glColor3f(1, 1, 1);
   glPushMatrix();
   glTranslatef(5, 1, -2);
   oyster(caustics[causticIndex], caustics[causticIndex], caustics[causticIndex]);
   glPopMatrix();

   /*glPushMatrix();
   glTranslatef(7, 6, -1);
   glRotatef(-40, 0, 1, 0);
   glRotatef(10, 0, 0, 1);
   glScalef(.5, .5, .5);
   fish(caustics[causticIndex], caustics[causticIndex], caustics[causticIndex], caustics[causticIndex]);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(-8, 5, -1.5);
   glRotatef(-90, 0, 1, 0);
   glRotatef(-40, 0, 0, 1);
   glScalef(.2, .2, .2);
   fish(caustics[causticIndex], caustics[causticIndex], caustics[causticIndex], caustics[causticIndex]);
   glPopMatrix();*/

   glPushMatrix();
   coralTubes(caustics[causticIndex]);
   glPopMatrix();

   /*glPushMatrix();
   glTranslatef(1, 4, -.5);
   glRotatef(-10, 0, 1, 0);
   glScalef(.8, .8, .8);
   crab(caustics[causticIndex], caustics[causticIndex]);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(-3.5, 1, 0);
   glRotatef(-45, 0, 1, 0);
   glScalef(.5, .5, .5);
   crab(caustics[causticIndex], caustics[causticIndex]);
   glPopMatrix();*/

   glEnable(GL_LIGHTING);
   glDisable(GL_TEXTURE_GEN_S);
   glDisable(GL_TEXTURE_GEN_T);

   glDepthMask(GL_TRUE);
   glDepthFunc(GL_LESS);
   glDisable(GL_BLEND);
}


/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   //ErrCheck("Start");
   posX += velocityX * t * 100000;
   posY += velocityY * t * 100000;
   posZ += velocityZ * t * 100000;
   //Set background color
   glClearColor(.8,.8,1,1);
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();


   glRotatef(ph,1,0,0);
   glRotatef(th,0,1,0);
   //Set camera position
   glTranslated(-posX,-posY - 3,-posZ - 8);

   //  Flat or smooth shading
   glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);

   //  Light switch
   if (light)
   {
        //  Translate intensity to color vectors
        float Ambient[]   = {0.01*ambient*20.0 ,0.01*ambient*20.0 ,0.01*ambient*20.0 ,1.0};
        //  Light position
        float Position[]  = {0,5,0,1.0};
        //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);
        //  Location of viewer for specular calculations
        glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
        //  glColor sets ambient and diffuse color materials
        glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
        //  Enable light 0
        glEnable(GL_LIGHT0);
        //  Set ambient, diffuse, specular components and position of light 0
        glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT0,GL_POSITION,Position);
   }
   else
     glDisable(GL_LIGHTING);

   glDisable(GL_LIGHTING); //Disable for print
   glWindowPos2i(5,5);
   //  White
   glColor3f(1,1,1);

   Print("Location=%f,%f,%f   ",posX, posY, posZ);

    if (light)
   {
      glWindowPos2i(5,45);
      glColor3f(1,1,1);
   }
        //Reference for fog: http://nehe.gamedev.net/tutorial/cool_looking_fog/19001/
   glFogi(GL_FOG_MODE, GL_EXP2); // Fog Mode
   GLfloat fogColor[4]= {0.1f, 0.1f, .8f, .3f};
   glHint(GL_FOG, GL_NICEST);
   glFogfv(GL_FOG_COLOR, fogColor); // Set Fog Color
   glFogf(GL_FOG_DENSITY, .05f); // How Dense Will The Fog Be
   glHint(GL_FOG_HINT, GL_NICEST); // Fog Hint Value
   glFogf(GL_FOG_START, 3.0f); // Fog Start Depth
   glFogf(GL_FOG_END, 10.0f); // Fog End Depth

//UNCOMMENT TO SEE FOG

   glEnable(GL_FOG); // Enables GL_FOG 

   glPushMatrix();
   glTranslated(0, 1, 0);
   //glScaled(.1, .1, .1);
   glCallList(objFiles[0]);
   glPopMatrix();
   
   glPushMatrix();
   glTranslated(3, 1, 0);
   glScaled(.01, .01, .02);
   glCallList(objFiles[1]);
   glPopMatrix();
   firstPass();
   /* Disable depth buffer update and exactly match depth
   buffer values for slightly faster rendering. */
   glDepthMask(GL_FALSE);
   glDepthFunc(GL_ADD);

   /* Multiply the source color (from the caustic luminance
   texture) with the previous color from the normal pass.  The
   caustics are modulated into the scene. */
   glBlendFunc(GL_ONE,GL_ZERO);
   glEnable(GL_BLEND);

   secondPass();

   /* Restore fragment operations to normal. */
   glDepthMask(GL_TRUE);
   glDepthFunc(GL_LESS);
   glDisable(GL_BLEND);


   seaweed();
   bubbles();

   //  Render the scene
   glFlush();
   //  Make the rendered scene visible
   glutSwapBuffers();
   //ErrCheck("End");
}

static void project()
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   gluPerspective (fov, asp, dim/4, 4*dim*4);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();

   glutPostRedisplay();
}



/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
   if (key == GLUT_KEY_RIGHT) {
      th += 5; //We have different mechanisms for angle in first person
   }
   //  Left arrow key - decrease angle by 5 degrees
   else if (key == GLUT_KEY_LEFT) {
      th -= 5;
   }
   //  Up arrow key - increase elevation by 5 degrees
   else if (key == GLUT_KEY_UP) {
      ph -= 5;
   }
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN) {
      ph += 5;
   }
   //  Smooth color model
   else if (key == GLUT_KEY_F1)
      smooth = 1-smooth;
      //  Local Viewer
   else if (key == GLUT_KEY_F2)
      local = 1-local;
   //  Ball distance
   else if (key == GLUT_KEY_F3)
      distance = (distance==1) ? 5 : 1;
   //  Pause Light
   else if (key == GLUT_KEY_F4)
      movingLight = 1-movingLight;
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset program
   else if (ch == '0') {
      th = ph = 0;
      fov = 55;
      project();
   }
   switch (ch){

      //First person controls

      case('w'): //Based on this source for first person http://www.swiftless.com/tutorials/opengl/camera2.html. 
         //I modified their math to some extent to resolve bugs. A bit of my math honestly came from discovering bugs and tinkering (a and d were tinkering results)
         //Move forward in first person mode
         velocityX = velocityX + (Sin(th)*Cos(ph)) / SPEED_DIVIDER;
         if (velocityX > TOP_SPEED) velocityX = TOP_SPEED;
         if (velocityX < -TOP_SPEED) velocityX = -TOP_SPEED;
         velocityZ = velocityZ - (Cos(th)*Cos(ph)) / SPEED_DIVIDER;
         if (velocityZ > TOP_SPEED) velocityZ = TOP_SPEED;
         if (velocityZ < -TOP_SPEED) velocityZ = -TOP_SPEED;
         break;
      case('s'):
         velocityX = velocityX - (Sin(th)*Cos(ph)) / SPEED_DIVIDER;
         if (velocityX > TOP_SPEED) velocityX = TOP_SPEED;
         if (velocityX < -TOP_SPEED) velocityX = -TOP_SPEED;
         velocityZ = velocityZ + (Cos(th)*Cos(ph)) / SPEED_DIVIDER;
         if (velocityZ >TOP_SPEED) velocityZ = TOP_SPEED;
         if (velocityZ < -TOP_SPEED) velocityZ = -TOP_SPEED;
         break;
      case('a'): //Move left in first person mode
         velocityX = velocityX - (Cos(th)*Cos(ph)) / SPEED_DIVIDER;
         if (velocityX > TOP_SPEED) velocityX = TOP_SPEED;
         if (velocityX < -TOP_SPEED) velocityX = -TOP_SPEED;
         velocityZ = velocityZ - (Sin(th)*Cos(ph)) / SPEED_DIVIDER;
         if (velocityZ > TOP_SPEED) velocityZ = TOP_SPEED;
         if (velocityZ < -TOP_SPEED) velocityZ = -TOP_SPEED;
         break;
      case ('d'): //Move right in first person mode
         velocityX = velocityX + (Cos(th)*Cos(ph)) / SPEED_DIVIDER;
         if (velocityX > TOP_SPEED) velocityX = TOP_SPEED;
         if (velocityX < -TOP_SPEED) velocityX = -TOP_SPEED;
         velocityZ = velocityZ + (Sin(th)*Cos(ph)) / SPEED_DIVIDER;
         if (velocityZ > TOP_SPEED) velocityZ = TOP_SPEED;
         if (velocityZ < -TOP_SPEED) velocityZ = -TOP_SPEED;
         break;
      case ('q'): //Move up in first person mode
         velocityY = velocityY + .2 / SPEED_DIVIDER;
         if (velocityY > TOP_SPEED) velocityY = TOP_SPEED;
         if (velocityY < -TOP_SPEED) velocityY = -TOP_SPEED;
         break;
      case ('z'): //Move down in first person mode
         velocityY = velocityY - .2 / SPEED_DIVIDER;
         if (velocityY > TOP_SPEED) velocityY = TOP_SPEED;
         if (velocityY < -TOP_SPEED) velocityY = -TOP_SPEED;
         break;
   }
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{ 
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, width,height);
   //  Set projection
   project();
}

void idle()
{
   //Decay the speed
   velocityX *= .9;
   velocityY *= .9;
   velocityZ *= .9;
   //Get delta time value
   t = (glutGet(GLUT_ELAPSED_TIME) - lastTime) / 1000.0;
   lastTime = glutGet(GLUT_ELAPSED_TIME);
   //Update bubble position
   for(int i = 0; i < NUMBER_OF_BUBBLES; i++) {
      bubbleArray[i].yPosition += t;
      if (bubbleArray[i].yPosition > worldDimensions)
         bubbleArray[i].yPosition = 0;
      bubbleArray[i].xPosition += sin(t) / 10;
   }
   causticCounter += t;
   if (causticCounter >= .05) {
   //Update caustic texture
         causticIndex = (causticIndex + 1)%16;
         causticCounter = 0;
   }
   glutPostRedisplay();
}


void initializeBubbles() {
   for (int i = 0; i < NUMBER_OF_BUBBLES; i++) {
      bubbleArray[i].xPosition = float_rand(-15.0, 15.0);
      bubbleArray[i].yPosition = float_rand(0.0, worldDimensions);
      bubbleArray[i].zPosition = float_rand(-15.0, 15.0);
   }
}

void initializeTexturesAndModels() {
   texture[0] = LoadTexBMP("Coral.bmp");
   texture[1] = LoadTexBMP("BlueEye.bmp");
   texture[2] = LoadTexBMP("Red.bmp");
   texture[3] = LoadTexBMP("CrabShell.bmp");
   texture[4] = LoadTexBMP("FishScales.bmp");
   texture[5] = LoadTexBMP("FishFin.bmp");
   texture[6] = LoadTexBMP("Lips.bmp");
   texture[7] = LoadTexBMP("MotherOfPearl.bmp");
   texture[8] = LoadTexBMP("MoPearl.bmp");
   texture[9] = LoadTexBMP("pearl.bmp");
   texture[10] = LoadTexBMP("bubble.bmp");
   texture[11] = LoadTexBMP("seaweed.bmp");
   texture[12] = LoadTexBMP("CoralTube.bmp");

   caustics[0] = LoadTexBMP("caustics_001.bmp");
   caustics[1] = LoadTexBMP("caustics_002.bmp");
   caustics[2] = LoadTexBMP("caustics_003.bmp");
   caustics[3] = LoadTexBMP("caustics_004.bmp");
   caustics[4] = LoadTexBMP("caustics_005.bmp");
   caustics[5] = LoadTexBMP("caustics_006.bmp");
   caustics[6] = LoadTexBMP("caustics_007.bmp");
   caustics[7] = LoadTexBMP("caustics_008.bmp");
   caustics[8] = LoadTexBMP("caustics_009.bmp");
   caustics[9] = LoadTexBMP("caustics_010.bmp");
   caustics[10] = LoadTexBMP("caustics_011.bmp");
   caustics[11] = LoadTexBMP("caustics_012.bmp");
   caustics[12] = LoadTexBMP("caustics_013.bmp");
   caustics[13] = LoadTexBMP("caustics_014.bmp");
   caustics[14] = LoadTexBMP("caustics_015.bmp");
   caustics[15] = LoadTexBMP("caustics_016.bmp");

   sky[0] = LoadTexBMP("underwater_rt.bmp");
   sky[1] = LoadTexBMP("underwater_ft.bmp");
   sky[2] = LoadTexBMP("underwater_lf.bmp");
   sky[3] = LoadTexBMP("underwater_bk.bmp");
   sky[4] = LoadTexBMP("underwater_up.bmp");
   sky[5] = LoadTexBMP("underwater_dn.bmp");

   objFiles[0] = LoadOBJ("Crab.obj");
   objFiles[1] = LoadOBJ("model.obj");
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{

   //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitWindowSize(600,600);
   //CALL FOR STENCIL
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   //  Create the window
   glutCreateWindow("Underwater Scene");
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
   //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(key);
   //  Tell GLUT to call "idle" when scene is idle
   glutIdleFunc(idle);
   //Initialize positions of bubbles in bubble system
   initializeBubbles();
   //Load textures
   initializeTexturesAndModels();
   //Initialize seaweed and coral positions
   seaweedInit();
   //  Pass control to GLUT so it can interact with the user 

   glutMainLoop();
   return 0;
}

//Yellow fish(https://sketchfab.com/models/3c065365c53f4e128242b0a003850e63) by Tan(https://sketchfab.com/Tan) is licensed under CC Attribution(http://creativecommons.org/licenses/by/4.0/)
