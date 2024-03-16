#ifndef DESENHA_H
#define DESENHA_H

#include <math.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "../include/GL/glut.h"

#include "Vetor3D.h"
//#include "Quaternion.h"

class Desenha
{
   public:

      Desenha(){};
      ~Desenha(){};
      static void drawBox(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
      static void drawWireBox(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
      static void drawSelectedBox(GLfloat, GLfloat, GLfloat, GLfloat, GLfloat, GLfloat);
      static void drawQuad(GLfloat, GLfloat, GLfloat, GLfloat);
      static void drawGrid(int, GLfloat, int, int);
      static void gluClosedCylinder(GLUquadric*, GLdouble, GLdouble, GLdouble, GLint, GLint);
      static void gluCapsule(GLUquadric*, GLdouble, GLdouble, GLdouble, GLint, GLint);
      static void drawArrow(Vetor3D from, Vetor3D to, GLfloat larg = 0.0);
      static void drawDoubleArrow(Vetor3D from, Vetor3D to, GLfloat larg = 0.0);
      static void drawEixos(GLfloat);

      static void setTransformODE(const dReal* pos, const dReal* R);
};

#endif

