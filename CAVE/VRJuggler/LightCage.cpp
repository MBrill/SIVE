// --------------------------------------------------------------------
//  LightCage.C
//  
//  Implementation file:
//  A STL container (list) for LightLines
// --------------------------------------------------------------------
//  $RCSfile$
//  $Revision$
//  $Date$
// --------------------------------------------------------------------
#include <cmath>

#include <GL/glu.h>
#include "LightLine.h"
#include "LightCage.h"

using namespace std;

// The next two functions can be used to set the attenuation of the
// lightlines. No inhomogenous lightcages with concern of the
// attenuation are supported. Maybe in the future.
void   LightCage::setCageAttenuation(LightLine::Attenuation att)
{
   list<LightLine>::iterator iter= cage.begin();
   list<LightLine>::iterator end = cage.end();

   attenuation = att;

// Iterate through the lines and set the requested attenuation
   while (iter != end) {
      iter->setAttenuation(att);
      ++iter;
   }
}

void   LightCage::setAttenuation(LightLine::Attenuation att)
{
   attenuation = att;
}

LightLine::Attenuation LightCage::getAttenuation(void)
{
   return attenuation;
}

// List functions
bool  LightCage::empty(void)
{
   return cage.empty();
}

// push of LightLine on LightCage
// line is pushed as is, no check on attributes, so 
// this way we could possibly get inhomomgenous attributes!
void  LightCage::pushback(LightLine line) 
{
   line.setAttenuation(attenuation);
   line.setRadius(radius);
   cage.push_back(line);
}

void  LightCage::pushback(float *p, float *d, float l)
{
   // construct the LightLine and insert
   float r = 0.0f;
   LightLine *line = new LightLine(p,d,r,l);
   line->setAttenuation(attenuation);
   line->setRadius(radius);

   cage.push_back(*line);
}

// Iterate through the lines, compute scalar values for every line
// overloaded computeScalar
// perpendicular distance
void LightCage::computeScalar(float values[], float point[3], float normal[3])
{
   int i=0;
   list<LightLine>::iterator iter;
   list<LightLine>::iterator end = cage.end();
   // Iterate through the lines, compute scalar values for every line
   // and return the minimum value!

   iter = cage.begin();
   while (iter != end) {
      values[i] = iter->highlightValue(point,normal);
      ++iter; ++i;
   }
}

// overloaded computeScalar
// isophotes; only one light line!
void LightCage::computeScalar(float &value, float normal[3])
{
   list<LightLine>::iterator iter;

   iter = cage.begin();
      value = iter->isophoteValue(normal);
}

// overloaded computeScalar
// reflection lines
void LightCage::computeScalar(float values[], float point[3], 
                                 float normal[3], float eye[3])
{
   int i=0;
   list<LightLine>::iterator iter;
   list<LightLine>::iterator end = cage.end();
// Iterate through the lines, compute scalar values for every line
// and return the minimum value!

   iter = cage.begin();
   while (iter != end) {
      values[i] = iter->reflectionValue(point,normal, eye);
      ++iter; ++i;
   }
}

// render with OpenGL
void  LightCage::draw(void)
{
   list<LightLine>::iterator iter;
   list<LightLine>::iterator end = cage.end();

   iter = cage.begin();

   while (iter != end) {
      iter->draw();
      ++iter;
   }
}

// get the cage as a OpenGL Display List
GLuint LightCage::createList(void)
{
   GLuint group = glGenLists(1);

   list<LightLine>::iterator iter = cage.begin();
   list<LightLine>::iterator end = cage.end();

   glNewList(group, GL_COMPILE);
   while (iter != end) {
      iter->draw();
      ++iter;
   }
   glEndList();

   return group;
}

// add the cage geometry to a empty, existing display list
void LightCage::createList(GLuint group)
{
   list<LightLine>::iterator iter=cage.begin();
   list<LightLine>::iterator end = cage.end();

   glNewList(group, GL_COMPILE);
   while (iter != end) {
      iter->draw();
      ++iter;
   }
   glEndList();
}

LightLine& LightCage::front(void)
{
   return cage.front();
}
LightLine& LightCage::back(void)
{
   return cage.back();
}

int LightCage::size(void)
{
   return cage.size();
}

void LightCage::setCageRadius(float r)
{
   list<LightLine>::iterator iter=cage.begin();
   list<LightLine>::iterator end = cage.end();

   radius = r;

   while (iter != end) {
      iter->setRadius(r);
      ++iter;
   }
}

void LightCage::setRadius(float r)
{
   radius =r;
}

float LightCage::getRadius(void)
{
   return radius;
}

void LightCage::setColor(float c[3])
{
   list<LightLine>::iterator iter=cage.begin();
   list<LightLine>::iterator end = cage.end();

   while (iter != end) {
      iter->setColor(c);
      ++iter;
   }
}

void LightCage::setColor(float r, float g, float b)
{
   list<LightLine>::iterator iter=cage.begin();
   list<LightLine>::iterator end = cage.end();

   while (iter != end) {
      iter->setColor(r,g,b);
      ++iter;
   }
}

list<LightLine>::iterator LightCage::begin(void)
{
   return cage.begin();
}

list<LightLine>::iterator LightCage::end(void)
{
   return cage.end();
}
