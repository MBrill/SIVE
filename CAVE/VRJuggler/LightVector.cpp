// --------------------------------------------------------------------
//  Implementation of LightVector
//  
// --------------------------------------------------------------------
//  $RCSfile: LightVector.cpp $
//  $Revision: 1.3 $
//  $Date: 2002/04/26 21:32:44 $
// --------------------------------------------------------------------
#include <iostream>
#include <cmath>
#include <stdlib.h>
#include "LightVector.h"

using namespace std;

// Default, direction = e_3
LightVector::LightVector(void) 
{
	direction.e3();
	// light is white, isn't it?
	color[0] = 1.0f;
	color[1] = 1.0f;
	color[2] = 1.0f;
	renderOrigin.zero();
	length = 1.0f;
}

LightVector::LightVector(const LightVector &copy)
{
   direction.set(copy.direction.getX(), copy.direction.getY(), copy.direction.getZ());
   renderOrigin.set(copy.renderOrigin.getX(), copy.renderOrigin.getY(), copy.renderOrigin.getZ());

   color[0] = copy.color[0];
   color[1] = copy.color[1];
   color[2] = copy.color[2];

   length = copy.length;
}

LightVector::LightVector(float d[])
{
   direction.set(d[0], d[1], d[2]);
   direction.normalize();  

   // light is white, isn't it?
   color[0] = 1.0f;
   color[1] = 1.0f;
   color[2] = 1.0f;
   renderOrigin.zero();
   length = 1.0f;
}

LightVector::LightVector(Vector3 d)
{
   direction = d;
   direction.normalize();  

   // light is white, isn't it?
   color[0] = 1.0f;
   color[1] = 1.0f;
   color[2] = 1.0f;

   renderOrigin.zero();

   length = 1.0f;
}

LightVector::LightVector(float d1, float d2, float d3)
                     
{
   direction.set(d1, d2, d3);
   direction.normalize();  
   
   // light is white, isn't it?
   color[0] = 1.0f;
   color[1] = 1.0f;
   color[2] = 1.0f;

   renderOrigin.zero();
   length = 1.0f;
}

LightVector::LightVector(float d1, float d2, float d3, float c1, 
                               float c2, float c3, float l)
{
   direction.set(d1, d2, d3);
   direction.normalize();  

   color[0] = c1;
   color[1] = c2;
   color[2] = c3;

   renderOrigin.zero();

   length = l;
}

LightVector::LightVector(float d1, float d2, float d3, 
                               float o1, float o2, float o3,
                               float c1, float c2, float c3, float l)
{
   direction.set(d1, d2, d3);
   direction.normalize();  

   color[0] = c1;
   color[1] = c2;
   color[2] = c3;

   renderOrigin.set(o1, o2, o3);
   length = l;
}

LightVector::LightVector(float d[], float c[], float l)
{
   direction.set(d[0], d[1], d[2]);
   direction.normalize();  
   
   color[0] = c[0];
   color[1] = c[1];
   color[2] = c[2];

   renderOrigin.zero();

   length=l;
}

LightVector::LightVector(float d[], float o[], float c[], float l)
{
   direction.set(d[0], d[1], d[2]);
   direction.normalize();  
   
   color[0] = c[0];
   color[1] = c[1];
   color[2] = c[2];

   renderOrigin.set(o[0], o[1], o[2]);

   length=l;
}

LightVector::LightVector(Vector3 d, Vector3 o, float c[], float l)
{
   direction = d;
   direction.normalize();  
   
   color[0] = c[0];
   color[1] = c[1];
   color[2] = c[2];

   renderOrigin = o;

   length=l;
}

float LightVector::isophoteValue(float surfaceNormal[3])
{
   Vector3 perfNormal(surfaceNormal);
   return direction.dot(perfNormal);
}

float LightVector::isophoteValue(Vector3 surfaceNormal)
{
   return direction.dot(surfaceNormal);
}

//
// render using OpenGL
//
void LightVector::draw(void)
{
  Vector3 p(renderOrigin);
  p.lerp(1.0f, length, direction);

  glColor3fv(color);
  glBegin(GL_LINES);
           glVertex3f(renderOrigin.getX(), renderOrigin.getY(), renderOrigin.getZ());
	   glVertex3f(p.getX(), p.getY(), p.getZ()); 
  glEnd();       
}

// Create a display list and render the geometry
GLuint LightVector::getLine()
{
  GLuint list = glGenLists(1);
  Vector3 p(renderOrigin);
  p.lerp(1.0, length, direction);

  glNewList(list, GL_COMPILE);
       glColor3fv(color);
       glBegin(GL_LINES);
           glVertex3f(renderOrigin.getX(), renderOrigin.getY(), renderOrigin.getZ());
	   glVertex3f(p.getX(), p.getY(), p.getZ()); 
       glEnd();       
  glEndList();

  return list;
}

// Rotate the lightline
void LightVector::rotateX(float deg)
{
        direction.rotateX(deg);
}

// Rotate the lightline
void LightVector::rotateY(float deg)
{
        direction.rotateY(deg);
}

// Rotate the lightline
void LightVector::rotateZ(float deg)
{
        direction.rotateZ(deg);
}

void LightVector::setRenderOrigin(float o[])
{
  renderOrigin.set(o[0], o[1], o[2]);
}

void LightVector::setRenderOrigin(float o1, float o2, float o3)
{
  renderOrigin.set(o1, o2, o3);
}

void LightVector::setRenderOrigin(Vector3 dir)
{
  renderOrigin = dir;
}

void LightVector::setDirection(float dir[])
{
  direction.set(dir[0], dir[1], dir[2]);;
}

void LightVector::setDirection(Vector3 dir)
{
  direction = dir;
}

float* LightVector::getDirection(void)
{
  float* local = new float[3];

  local[0] = direction.getX();
  local[1] = direction.getY();
  local[2] = direction.getZ();
  return local;
}

void LightVector::setColor(float r, float g, float b)
{
  color[0] = r;
  color[1] = g;
  color[2] = b;
}

void LightVector::setColor(float c[3])
{
  color[0] = c[0];
  color[1] = c[1];
  color[2] = c[2];
}

void LightVector::setLength(float s)
{
  length = s;
}

float LightVector::getLength(void)
{
  return length;
}
