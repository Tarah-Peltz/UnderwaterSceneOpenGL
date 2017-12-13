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

void Cylinder(int numMajor, int numMinor, float height, float radius, unsigned int texture) {
   //Based off this source but modified greatly for lighting: http://www.unix.com/programming/124875-draw-3d-cylinder-using-opengl.html
   notShiny();
   double majorStep = height / numMajor;
   double minorStep = 2.0 * M_PI / numMinor;
   int i, j;
   if (causticsPass == 0) {
      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
      glBindTexture(GL_TEXTURE_2D,texture);
   }
   //https://gist.github.com/nikAizuddin/5ea402e9073f1ef76ba6

   double a, b;
   float x1, x2, y1, y2, z0, z1;
   //y1 = 0;
   //y2 = height;
  /* glBegin(GL_TRIANGLES);
   for (i = 0; i < numMajor; i++) {
      z0 = 0.5 * height - i * majorStep;
      z1 = z0 - majorStep;*/

     // x1 = 0;
     // x2 = sin(minorStep) * radius;
      //y1 += majorStep;
      //y2 += majorStep;
  /* glBegin(GL_QUAD_STRIP);

    float angle = 0.0;


     while( angle < 2*M_PI ) {

       x1 = radius * cos(angle);

      y1 = radius * sin(angle);

      glTexCoord2f(x1 / 2*M_PI, 1); glVertex3f(x1, y1 , height);

      glTexCoord2f(x1 / 2*M_PI, 0); glVertex3f(x1, y1 , 0.0);

      angle = angle + minorStep;

     }

        glTexCoord2f(1, 1); glVertex3f(radius, 0.0, height);

        glTexCoord2f(0, 0); glVertex3f(radius, 0.0, 0.0);

    glEnd();*/
      /*for (j = 0; j <= numMinor ; j++) {
         //printf("%f\n", j);
         a = j * minorStep;
         b = (j+1) * minorStep;

         x1 = radius * cos(a);
         y1 = radius * sin(a);
         x2 = radius * cos(b);
         y2 = radius * sin(b);
         //(radius * 2 * M_PI)
         //height
         //glColor3f((i + j) / (numMinor + numMajor), (i + j) / (numMinor + numMajor), (i + j) / (numMinor + numMajor));
         //glColor3f(x1, x1, x1);
         glTexCoord2f(sqrt((x1*x1) + (z0*z0)) , sqrt((y1*y1) + (z0*z0)) / height); glVertex3f(x1, y1, z0);
         glTexCoord2f(sqrt((x1*x1) + (z1*z1)) , sqrt((y1*y1) + (z1*z1)) / height); glVertex3f(x1, y1, z1);
         glTexCoord2f(sqrt((x2*x2) + (z0*z0)) , sqrt((y2*y2) + (z0*z0)) / height); glVertex3f(x2, y2, z0);
         normalV(x1, y1, z0, x1, y1, z1, x2, y2, z0);
         glNormal3f(normalX, normalY, normalZ);
         //ErrCheck(x1);

         glTexCoord2f(sqrt((x2*x2) + (z0*z0)) , sqrt((y2*y2) + (z0*z0)) / height); glVertex3f(x2, y2, z0);
         glTexCoord2f(sqrt((x2*x2) + (z1*z1)) , sqrt((y2*y2) + (z1*z1)) / height); glVertex3f(x2, y2, z1);
         glTexCoord2f(sqrt((x1*x1) + (z1*z1)) , sqrt((y1*y1) + (z1*z1)) / height); glVertex3f(x1, y1, z1);
         normalV(x2, y2, z0, x1, y1, z1, x2, y2, z1);
         glNormal3f(normalX, normalY, normalZ);
      }
      }
      glEnd();*/
         //ErrCheck(x2);
         /*printf ("Here");
         if (i == numMajor - 1)
            if (j == numMinor - 1)
            printf("%f, %f, %f, %f\n", i , i, j, j);*/
         /*printf("%d %d\n", j, numMinor );
         glColor3f(j/numMinor, j/numMinor, j/numMinor);
         glTexCoord2f(i / numMajor, j / numMinor); glVertex3f(x1, z0, 0);
         glTexCoord2f((i + 1) / numMajor, j / numMinor); glVertex3f(x1, z1, 0);
         //glTexCoord2f(i / numMajor, j + 1/ numMinor); glVertex3f(x2, y2, z0);*/
         //normalV(x1, y1, z0, x1, y1, z1, x2, y2, z0);
         //glNormal3f(normalX, normalY, normalZ);

        // glTexCoord2f((i + 1) / numMajor, (j + 1) / numMinor); glVertex3f(x2, z1, 1);
         //glTexCoord2f(i / numMajor, (j + 1) / numMinor); glVertex3f(x2, z0, 1);
         
         //glTexCoord2f(i + 1/ numMajor, j / numMinor); glVertex3f(x1, y1, z1);
         /*normalV(x2, y2, z0, x1, y1, z1, x2, y2, z1);
         glNormal3f(normalX, normalY, normalZ); 
         x1 = sin(minorStep * j) * radius;
         x2 = sin(minorStep * j) * radius;
         z0 = cos(minorStep * j) * radius;
         z1 = cos(minorStep * j) * radius;
      
   }
   glEnd();*/
   glBegin(GL_QUAD_STRIP);
   for (i = 0; i <= 2 * M_PI; i += numMinor)
   {
       const float tc = ( i / (float)( 2 * M_PI ) );
       glTexCoord2f( tc, 0.0 );
       glVertex3f(radius * cos(i), 0, radius * sin(i));
       glTexCoord2f( tc, 1.0 );
       glVertex3f(radius * cos(i), height, radius * sin(i));
   }

   /* close the loop back to zero degrees */
   glTexCoord2f( 0.0, 0.0 );
   glVertex3f(radius, 0, 0);
   glTexCoord2f( 0.0, 1.0 );
   glVertex3f(radius, height, 0);
   glEnd();

   if (causticsPass == 0) {
      glDisable(GL_TEXTURE_2D);
   }      
}

// modified from https://stackoverflow.com/questions/26536570/how-do-i-texture-a-cylinder-in-opengl-created-with-triangle-strip
void drawCylinder(float height, float radius, unsigned int texture)
{
   //ErrCheck("Cylinder Start");
   if (causticsPass == 0) {
      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
      glBindTexture(GL_TEXTURE_2D,texture);
   }
   const double PI = 3.14159;
   double i, resolution  = 0.05;
   glPushMatrix();


    /* middle tube */
   // Draw bottom cap.

  /* const unsigned DIV_COUNT = 32;

// Calculate angle increment from point to point, and its cos/sin.
float angInc = 2.0f * M_PI / static_cast<float>(DIV_COUNT);
float cosInc = cos(angInc);
float sinInc = sin(angInc);
glBegin(GL_TRIANGLE_FAN);

glTexCoord2f(0.5f, 0.5f);
glVertex3f(0.0f, 0.0f, 0.0f);

glTexCoord2f(1.0f, 0.5f);
glVertex3f(radius, 0.0f, 0.0f);

float xc = 1.0f;
float yc = 0.0f;
for (unsigned iDiv = 1; iDiv < DIV_COUNT; ++iDiv) {
    float xcNew = cosInc * xc - sinInc * yc;
    yc = sinInc * xc + cosInc * yc;
    xc = xcNew;
    glTexCoord2f(0.5f + 0.5f * xc, 0.5f + 0.5f * yc);
    glVertex3f(radius * xc, 0.0f, radius * yc);
}

glTexCoord2f(1.0f, 0.5f);
glVertex3f(radius, 0.0f, 0.0f);

glEnd();*/
    glColor3f(.3, .3, .3);
    float oldX = radius - .15;
    float oldZ = 0;
    glBegin(GL_QUAD_STRIP);
        for (i = 0; i <= 1.1; i += resolution)
        {
         float tc = ( i / (float)( 2 * PI ) );
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
         const float tc = ( i / (float)( 2 * PI ) );
         glTexCoord2f( i, 0.0 );
         glVertex3f((radius - .15) * cos(i* 2 * PI), height, (radius - .15) * sin(i* 2 * PI));
         glTexCoord2f( i, 1.0 );
         glVertex3f((radius - .25) * cos(i* 2 * PI), height + .15, (radius - .25) * sin(i* 2 * PI));
         normalV(radius * cos(i* 2 * PI), height, radius * sin(i* 2 * PI), (radius - .15) * cos(i* 2 * PI), height + .15, (radius - .15) * sin(i* 2 * PI), oldX, height + .15, oldZ);
         glNormal3f(normalX, normalY, normalZ);
         oldX = (radius - .25) * cos(i* 2 * PI); 
         oldZ = (radius - .25) * sin(i* 2 * PI);
      }
        /* close the loop back to zero degrees */
      /*glTexCoord2f( 0.0, 0.0 );
      glVertex3f(radius - .15, height, 0);
      glTexCoord2f( 0.0, 1.0 );
      glVertex3f(radius - .25, height + .15, 0);*/
      normalV(radius * cos(i* 2 * PI), height, radius * sin(i* 2 * PI), (radius - .15) * cos(i* 2 * PI), height + .15, (radius - .15) * sin(i* 2 * PI), oldX, height + .15, oldZ);
      glNormal3f(normalX, normalY, normalZ);
      glEnable(GL_TEXTURE_2D);
      glEnd();
      glColor3f(1, 1, 1);
      glPopMatrix();
       //ErrCheck("Cylinder End");
}

void disc(float radius, unsigned int texture)
{
   //ErrCheck("Cylinder Start");
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
   //glTexCoord2f( 0.5, 0.5 );
   //glVertex3f(radius , 0, 0);
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