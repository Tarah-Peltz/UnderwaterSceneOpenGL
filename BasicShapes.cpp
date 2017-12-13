#include "CSCIx229.h"
#include "BasicShapes.h"


void normalV(double x1, double y1, double z1, double x2, double y2, double z2, double x3, double y3,  double z3){
   normalX = (y2-y1)*(z3-z1)-(z2-z1)*(y3-y1);
   normalY = (z2-z1)*(x3-x1)-(x2-x1)*(z3-z1); 
   normalZ = (x2-x1)*(y3-y1)-(y2-y1)*(x3-x1);
}

void shinyMetal(){
   float white[] = {1,1,1,1};
   glMaterialf(GL_FRONT,GL_SHININESS,100);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   diffuse = 10; specular = 90;
   float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
   float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};

   glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
   glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
}

void notShiny(){
   float white[] = {1,1,1,1};
   glMaterialf(GL_FRONT,GL_SHININESS,10);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   diffuse = 90; specular = 10;
   float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
   float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};

   glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
   glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
}

static void TexVertex(int th,int ph, float degrees, int scaleFactor)
{
   double x = -Sin(th)*Cos(ph);
   double y =  Cos(th)*Cos(ph);
   double z =          Sin(ph);
   glNormal3d(x,y,z);

   if (causticsPass == 0) glTexCoord2d(th/degrees * scaleFactor,ph/(degrees/2) * scaleFactor + 0.5);
   glVertex3d(x,y,z);
}

void Sphere(int textureIndex, int scaleFactor, unsigned int tex, float degrees)
{
   int th,ph;

   //  Set texture
   if (causticsPass == 0) {
      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
      glBindTexture(GL_TEXTURE_2D,tex);
   }
   //  Latitude bands
   for (ph=-90;ph<90;ph+=5)
   {
      glBegin(GL_QUAD_STRIP);
      for (th=0;th<=degrees;th+=5)
      {
         TexVertex(th,ph, degrees, scaleFactor);
         TexVertex(th,ph+5, degrees, scaleFactor);
      }
      glEnd();
   }
   if (causticsPass == 0) {
      glDisable(GL_TEXTURE_2D);
   }
}


// modified from https://stackoverflow.com/questions/26536570/how-do-i-texture-a-cylinder-in-opengl-created-with-triangle-strip
void drawCylinder(float height, float radius, unsigned int texture)
{
   if (causticsPass == 0) {
      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
      glBindTexture(GL_TEXTURE_2D,texture);
   }
   const double PI = 3.14159;
   double i, resolution  = 0.05;
   glPushMatrix();
    glColor3f(.3, .3, .3);
    float oldX = radius - .15;
    float oldZ = 0;
    glBegin(GL_QUAD_STRIP);
        for (i = 0; i <= 1.1; i += resolution)
        {
         glTexCoord2f( i, 0.0 );
         glVertex3f(radius * cos(i * 2 * PI), 0, radius * sin(i* 2 * PI));
         glTexCoord2f( i, 1.0 );
         glVertex3f((radius - .15) * cos(i* 2 * PI), height, (radius - .15) * sin(i* 2 * PI));
         normalV(radius * cos(i* 2 * PI), 0, radius * sin(i* 2 * PI), (radius - .15) * cos(i* 2 * PI), height, (radius - .15) * sin(i* 2 * PI), oldX, height, oldZ);
         glNormal3f(normalX, normalY, normalZ);
         oldX = (radius - .15) * cos(i* 2 * PI); 
         oldZ = (radius - .15) * sin(i* 2 * PI);


        }
        // close the loop back to zero degrees 
      glTexCoord2f( 0.0, 0.0 );
      glVertex3f(radius, 0, 0);
      glTexCoord2f( 0.0, 1.0 );
      glVertex3f(radius - .15, height, 0);
      normalV(radius * cos(i* 2 * PI), 0, radius * sin(i), (radius - .15) * cos(i* 2 * PI), height, (radius - .15) * sin(i* 2 * PI), oldX, height, oldZ);
      glNormal3f(normalX, normalY, normalZ);
      glEnd();
      glColor3f(.7, .7, .6);
      oldX = radius - .25;
      oldZ = 0;
      glBegin(GL_QUAD_STRIP);
      for (i = 0; i <= 1.1; i += resolution)
      {
         glTexCoord2f( i, 0.0 );
         glVertex3f((radius - .15) * cos(i* 2 * PI), height, (radius - .15) * sin(i* 2 * PI));
         glTexCoord2f( i, 1.0 );
         glVertex3f((radius - .25) * cos(i* 2 * PI), height + .15, (radius - .25) * sin(i* 2 * PI));
         normalV(radius * cos(i* 2 * PI), height, radius * sin(i* 2 * PI), (radius - .15) * cos(i* 2 * PI), height + .15, (radius - .15) * sin(i* 2 * PI), oldX, height + .15, oldZ);
         glNormal3f(normalX, normalY, normalZ);
         oldX = (radius - .25) * cos(i* 2 * PI); 
         oldZ = (radius - .25) * sin(i* 2 * PI);
      }
      normalV(radius * cos(i* 2 * PI), height, radius * sin(i* 2 * PI), (radius - .15) * cos(i* 2 * PI), height + .15, (radius - .15) * sin(i* 2 * PI), oldX, height + .15, oldZ);
      glNormal3f(normalX, normalY, normalZ);
      glEnable(GL_TEXTURE_2D);
      glEnd();
      glColor3f(1, 1, 1);
      glPopMatrix();
}

void disc(float radius, unsigned int texture)
{
   if (causticsPass == 0) {
      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
      glBindTexture(GL_TEXTURE_2D,texture);
   }
   const double PI = 3.14159;
   double i, resolution  = 0.1;
   glPushMatrix();
   glColor3f(.7, .7, .6);
   glBegin(GL_TRIANGLE_FAN);
   glTexCoord2f( 0.5, 0.5 );
   glVertex3f(0, 0, 0);  // center 
   for (i = 0; i <= 1; i += resolution)
   {
      glTexCoord2f( 0.5f * cos(i* 2 * PI) + 0.5f, 0.5f * sin(i* 2 * PI) + 0.5f );
      glVertex3f(radius * cos(i* 2 * PI), .01, radius * sin(i* 2 * PI));
   }
   normalV(0, .01, 0, radius, .01, 0, radius * cos(i* 2 * PI), .01, radius * sin(i* 2 * PI));
   glNormal3f(normalX, normalY, normalZ);
   glEnd();
   glPopMatrix();
}


void eyeball(double x,double y,double z,double r, unsigned int eyeTexture) {
   shinyMetal();
   glPushMatrix();
   glTranslated(x,y,z);
   glRotated(-90, 1, 0, 0);
   glScaled(r,r,r);
   Sphere(1, 1, eyeTexture, 360);
   glPopMatrix();
}