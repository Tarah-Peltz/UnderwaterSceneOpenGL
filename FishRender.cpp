#include "CSCIx229.h"
#include "FishRender.h"
#include "BasicShapes.h"

void fishBody(double x,double y,double z, double r, unsigned int fishScales)
{
   shinyMetal();
   glPushMatrix();
   glTranslated(x,y,z);
   glScaled(r,r,r);
   glColor3f(0, .5, .8);
   Sphere(4, 2, fishScales, 360);
   glColor3f(1, 1, 1);
   glPopMatrix();
}

void lips(double x, double y, double z, double r, unsigned int lipTexture) {
   notShiny();

   //Upper lip
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y + r,z);
   glRotatef(120, 1, 0, 0);
   glScaled(r,r * 2,r);
   glColor3f(1, 0, 0);
   Sphere(6, 1, lipTexture, 360);
   //  Undo transformations
   glPopMatrix();

   //Bottom Lip
   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x, y-r/2, z *1.2);
   glRotatef(90, 1, 0, 0);
   glScaled(r * 1.2 ,r * 2,r * 1.2);
   glColor3f(1, 0, 0);
   Sphere(6, 1, lipTexture, 360);
   //  Undo transformations
   glPopMatrix();
}

void fin(unsigned int finTexture) {
   notShiny();
   if (causticsPass == 0) {
      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
      glBindTexture(GL_TEXTURE_2D,finTexture);
   }
   glBegin(GL_TRIANGLES);

   normalV(0, 0, 3, -1, 1, -2, -1, -1, -1);
   glNormal3f(normalX, normalY, normalZ);
   glTexCoord2d(.5, .5); glColor3f(.8, .3, .8); glVertex3f(0, 0, 3);
   glTexCoord2d(0, 1); glColor3f(.8, .3, .6); glVertex3f(-1, 1, -2);
   glTexCoord2d(0, 0); glColor3f(.6, .3, .8); glVertex3f(-1, -1, -1);
   
   normalV(0, 0, 3, -1, -1, -1, 1, -1, -1);
   glNormal3f(normalX, normalY, normalZ);
   glTexCoord2d(.5, .5); glColor3f(.8, .3, .8); glVertex3f(0, 0, 3);
   glTexCoord2d(0, 0); glColor3f(.6, .3, .8); glVertex3f(-1, -1, -1);
   glTexCoord2d(1, 0); glColor3f(.6, .3, .8);glVertex3f(1, -1, -1);


   normalV(0, 0, 3, 1, -1, -1, 1, 1, -2);
   glNormal3f(normalX, normalY, normalZ);
   glTexCoord2d(.5, .5); glColor3f(.8, .3, .8); glVertex3f(0, 0, 3);
   glTexCoord2d(1, 0); glColor3f(.6, .3, .8);glVertex3f(1, -1, -1);
   glTexCoord2d(1, 1); glColor3f(.8, .3, .6);glVertex3f(1, 1, -2);

   normalV(0, 0, 3, 1, 1, -2, -1, 1, -2);
   glNormal3f(normalX, normalY, normalZ);
   glTexCoord2d(.5, .5); glColor3f(.8, .3, .8); glVertex3f(0, 0, 3);
   glTexCoord2d(1, 1); glColor3f(.8, .3, .6);glVertex3f(1, 1, -2);
   glTexCoord2d(0, 1); glColor3f(.8, .3, .6); glVertex3f(-1, 1, -2);


   glEnd();
   if (causticsPass == 0) {
      glDisable(GL_TEXTURE_2D);
   }
}

void dorsal(unsigned int finTexture){
   notShiny();
   if (causticsPass == 0) {
      glEnable(GL_TEXTURE_2D);
      glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
      glBindTexture(GL_TEXTURE_2D,finTexture);
   }
   glBegin(GL_QUADS); 
   normalV(1, 1, 0, -1, 0, 1, 1, 0, 1);
   glNormal3f(normalX, normalY, normalZ);
   glTexCoord2d(1, 0); glColor4d(.8, .4, .8, .8); glVertex3f(1, 1, 0);
   glTexCoord2d(0, 0); glColor4d(.6, .2, .6, .8); glVertex3f(-1, 1.5, 0);
   glTexCoord2d(0, 1); glColor4d(.5, .1, .5, .8); glVertex3f(-1, 0, 1);
   glTexCoord2d(1, 1); glColor4d(.7, .3, .7, .8); glVertex3f(1, 0, 1);

   normalV(1, 0, 1, -1, 0, 1, 1, 0, -1);
   glNormal3f(normalX, normalY, normalZ);
   glTexCoord2d(1, 1); glColor4d(.7, .3, .7, .2); glVertex3f(1, 0, 1);
   glTexCoord2d(1, 0); glColor4d(.7, .3, .7, .2); glVertex3f(1, 0, -1);
   glTexCoord2d(0, 0); glColor4d(.5, .1, .5, .2); glVertex3f(-1, 0, -1);
   glTexCoord2d(0, 1); glColor4d(.5, .1, .5, .2); glVertex3f(-1, 0, 1);

   normalV(1, 0, -1, -1, 0, -1, 1, 1, 0);
   glNormal3f(normalX, normalY, normalZ);
   glTexCoord2d(0, 1); glColor4d(.6, .2, .6, .2); glVertex3f(-1, 1.5, 0);
   glTexCoord2d(1, 1); glColor4d(.8, .4, .8, .2); glVertex3f(1, 1, 0);
   glTexCoord2d(1, 0); glColor4d(.7, .3, .7, .2); glVertex3f(1, 0, -1);
   glTexCoord2d(0, 0); glColor4d(.5, .1, .5, .2); glVertex3f(-1, 0, -1);
   glEnd();

   glBegin(GL_TRIANGLES);
   normalV(-1, 1.5, 0, -1, 0, -1, -1, 0, 1);
   glNormal3f(normalX, normalY, normalZ);
   glTexCoord2d(.5, .5); glColor4d(.6, .2, .6, .2); glVertex3f(-1, 1.5, 0);
   glTexCoord2d(0, 0); glColor4d(.5, .1, .5, .2); glVertex3f(-1, 0, -1);
   glTexCoord2d(1, 1); glColor4d(.5, .1, .5, .2); glVertex3f(-1, 0, 1);


   normalV(1, 1, 0, 1, 0, 1, 1, 0, -1);
   glNormal3f(normalX, normalY, normalZ);
   glTexCoord2d(.5, .5); glColor4d(.8, .4, .8, .2); glVertex3f(1, 1, 0);
   glTexCoord2d(1, 1); glColor4d(.7, .3, .7, .2); glVertex3f(1, 0, 1);
   glTexCoord2d(0, 0); glColor4d(.7, .3, .7, .2); glVertex3f(1, 0, -1);
   glEnd();
}

void fish(unsigned int scaleTexture, unsigned int eyeTexture, unsigned int lipTexture, unsigned int finTexture) {
   //Body
   glPushMatrix();
   glScalef(1, .8, .8);
   fishBody(0,0,0, 1, scaleTexture);
   glPopMatrix();

   //Eyes
   glPushMatrix();
   glTranslatef(.7, .5, .35);
   glRotatef(90, 0, 1, 0);
   eyeball(0, 0, 0, .2, eyeTexture);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(.7, .5, -.35);
   glRotatef(90, 0, 1, 0);
   eyeball(0, 0, 0, .2, eyeTexture);
   glPopMatrix();

   //Lips
   glPushMatrix();
   glTranslatef(.95, 0, 0);
   glRotatef(-90, 0, 1, 0);
   lips(0,0,0, .1, lipTexture);
   glPopMatrix();

   //Fins
   glPushMatrix();
   glTranslatef(0, 0, .9);
   glRotatef(30, 1, 0, 0);
   glScalef(.2, .1, .2);
   fin(finTexture);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(0, 0, -.9);
   glRotatef(180, 0, 1, 0);
   glRotatef(30, 1, 0, 0);
   glScalef(.2, .1, .2);
   fin(finTexture);
   glPopMatrix();

   //Tail
   glPushMatrix();
   glTranslatef(-1, .1, 0);
   glRotatef(90, 0, 1, 0);
   glRotatef(210, 1, 0, 0);
   glScalef(.3, .2, .2);
   fin(finTexture);
   glPopMatrix();

   glPushMatrix();
   glTranslatef(-1, -.1, 0);
   glRotatef(90, 0, 1, 0);
   glRotatef(150, 1, 0, 0);
   glRotatef(180,0,0,1);
   glScalef(.3, .2, .2);
   fin(finTexture);
   glPopMatrix();

   //Dorsal
   glPushMatrix();
   glTranslatef(-.2, .6, 0);
   glScalef(.4, .3, .1);
   dorsal(finTexture);
   glPopMatrix();
}