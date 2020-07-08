// --------------------------------------------------------------------
//  Implementation of LightLine
// --------------------------------------------------------------------
//  $RCSfile: LightLine.cpp $
//  $Revision: 4.0 $
//  $Date: 2005/11/12 16:56:26 $
// --------------------------------------------------------------------
#include <cmath>
#include <fstream.h>
#include <stdlib.h>

#include <GL/glu.h>

#include "vtkLineSource.h"
#include "vtkPolyDataMapper.h"

#include "LightLine.h"

#include <GL/gl.h>
using namespace std;

// Default, point = 0, direction = e_1
LightLine::LightLine(void) 
{
   point.zero();
   direction.e1();

   // line, no cylinder as default
   radius = 0.0f;

   // light is white, isn't it?
   color[0] = 1.0f;
   color[1] = 1.0f;
   color[2] = 1.0f;

   cylinder = false;
   lightform = LightLine::Constant;
   length = 1.0f;
}

LightLine::LightLine(float p[], float d[])
{
   point.set(p[0], p[1], p[2]);
   direction.set(d[0], d[1], d[2]);
   direction.normalize();  

   radius = 0.0f;

   // light is white, isn't it?
   color[0] = 1.0f;
   color[1] = 1.0f;
   color[2] = 1.0f;

   cylinder = false;
   lightform = LightLine::Constant;
   length = 1.0f;
}

LightLine::LightLine(float p[], float d[], float r, float l)
{
   point.set(p[0], p[1], p[2]);
   direction.set(d[0], d[1], d[2]);
   direction.normalize();  
   
   radius = r;

   // light is white, isn't it?
   color[0] = 1.0f;
   color[1] = 1.0f;
   color[2] = 1.0f;

   cylinder = false;
   lightform = LightLine::Constant;
   length = l;
}

LightLine::LightLine(float p[], float d[], float r, Attenuation form, float l)
{
   point.set(p[0], p[1], p[2]);
   direction.set(d[0], d[1], d[2]);
   direction.normalize();  
   
   radius = r;

   // light is white, isn't it?
   color[0] = 1.0f;
   color[1] = 1.0f;
   color[2] = 1.0f;

   cylinder = false;
   lightform = form;
   length = l;
}

LightLine::LightLine(float p[], float d[], float r)
{
   point.set(p[0], p[1], p[2]);
   direction.set(d[0], d[1], d[2]);
   direction.normalize();  
   
   radius = r;

   // light is white, isn't it?
   color[0] = 1.0f;
   color[1] = 1.0f;
   color[2] = 1.0f;

   cylinder = false;
   lightform = LightLine::Constant;
   length = 1.0f;
}

LightLine::LightLine(float p[], float d[], float r, Attenuation form)
{
   point.set(p[0], p[1], p[2]);
   direction.set(d[0], d[1], d[2]);
   direction.normalize();  
   
   radius = r;

   // light is white, isn't it?
   color[0] = 1.0f;
   color[1] = 1.0f;
   color[2] = 1.0f;

   cylinder = false;
   lightform = form;
   length = 1.0f;
}

LightLine::LightLine(float p1, float p2, float p3, float d1, 
                     float d2, float d3)
{
   point.set(p1, p2, p3);
   direction.set(d1, d2, d3);
   direction.normalize();  
   
   radius = 0.0f;

   // light is white, isn't it?
   color[0] = 1.0f;
   color[1] = 1.0f;
   color[2] = 1.0f;

   cylinder = false;
   lightform = LightLine::Constant;
   length = 1.0f;
}

// Set Radius, Attenuation is Constant
LightLine::LightLine(float p1, float p2, float p3, float d1, 
                     float d2, float d3, float r)
{
   point.set(p1, p2, p3);
   direction.set(d1, d2, d3);
   direction.normalize();  

   radius = r;

   // light is white, isn't it?
   color[0] = 1.0f;
   color[1] = 1.0f;
   color[2] = 1.0f;

   cylinder = false;
   lightform = LightLine::Constant;
   length = 1.0f;
}

// Set Radius and Attenuation
LightLine::LightLine(float p1, float p2, float p3, float d1, 
                     float d2, float d3, float r, Attenuation form)
{
   point.set(p1, p2, p3);
   direction.set(d1, d2, d3);
   direction.normalize();  

   radius = r;

   // light is white, isn't it?
   color[0] = 1.0f;
   color[1] = 1.0f;
   color[2] = 1.0f;

   cylinder = false;
   lightform = form;
   length = 1.0f;
}

// Read the definition from file
LightLine::LightLine(const char* inFile) 
{
  ifstream data;
  float p[3], d[3];

  data.open(inFile, ios::in);
  if (data.fail()) {
      cerr << "Inputfile does not exist!" << endl;
      exit(1);
  }
  data >> p[0] >> p[1] >> p[2];
  data >> d[0] >> d[1] >> d[2];

  data >> radius;
  data >> length;
  data.close();

  point.set(p[0], p[1], p[2]);
  direction.set(d[0], d[1], d[2]);
   
  // light is white, isn't it?
  color[0] = 1.0f;
  color[1] = 1.0f;
  color[2] = 1.0f;

  cylinder = false;
  lightform = LightLine::Constant;
}

// give parameter, compute point on line
void LightLine::getPointOnLine(float lambda, float p[3])
{
  p[0] = point.getX() + lambda*direction.getX();
  p[1] = point.getY() + lambda*direction.getY();
  p[2] = point.getZ() + lambda*direction.getZ();
}

//
// For a given point and normal, compute the unsigned
// perpendicular distance
//
float LightLine::perpendicularDistance(float origin[3],
                                       float lineDirection[3])
{
   Vector3 sp(origin[0], origin[1], origin[2]),
           sn(lineDirection[0], lineDirection[1], lineDirection[2]);
   sn.normalize();
   return this->perpendicularDistance(sp,sn);
}

float LightLine::perpendicularDistance(Vector3 sp,
                                       Vector3 sn)
{
   // This function assumes, that the Vector sn is
   // normalized before the call !!!
   Vector3 help, diff(point);

   // Cross-product
   help=direction.crossProduct(sn);
   diff.lerp(1.0f, -1.0f, sp);

   return help.dot(diff);
}

float LightLine::highlightValue(float surfacePoint[3],
                                float surfaceNormal[3])
{
   Vector3 sp(surfacePoint[0], surfacePoint[1], surfacePoint[2]),
           sn(surfaceNormal[0], surfaceNormal[1], surfaceNormal[2]);

   // Cross-product
   sn.normalize();
   return this->perpendicularDistance(sp, sn);
}

float LightLine::reflectionValue(float surfacePoint[3],
                                 float surfaceNormal[3],
                                 float eyePoint[3])

{  
   float lambda;
   Vector3 sp(surfacePoint[0], surfacePoint[1], surfacePoint[2]),
           sn(surfaceNormal[0], surfaceNormal[1], surfaceNormal[2]),
           eye(eyePoint[0], eyePoint[1], eyePoint[2]),
           help, diff(eye);

   diff = eye - sp;

   lambda = 2.0f*sn.dot(diff);
   help.lerp(lambda, -1.0, diff);
   help.normalize();

   // compute the perpendicular distance with direction help
   return this->perpendicularDistance(sp, help);
}

float LightLine::isophoteValue(float surfaceNormal[3])
{
   Vector3 perfNormal(surfaceNormal[0], surfaceNormal[1], 
                         surfaceNormal[2]);
   return direction.dot(perfNormal);
}

//
// compute the luminance, using the perpendicular distance, and the
// different lightforms.
//
float LightLine::luminance(Vector3 center, Vector3 luv)
{
   double pD = this->perpendicularDistance(center, luv)/(double)radius, 
          value = 0.0f;
   
   if (fabs(pD)>1.0) return 0.0f;
   else {
      switch (lightform) {
              // only constant and linear lightbands are supported ...
              case LightLine::Linear:
                          if (pD > 0.0f) value=1.0f - pD;
                          else value=1.0f + pD;
                          break;
              case LightLine::Quadratic:
                          value = 1.0f - pD*pD;
                          break;
              case LightLine::Polynomial:
                          value = 1.0f - pD*pD*(3.0 - 2.0 * fabs(pD));
                          break;
              case LightLine::Constant:
                          value = 1.0f;
                          break;
              default:
                          break;
              }
              return value;
   }
}

//
// Compute the luminance in the lightplane. c is a coordinate of a point 
// in the the light plane. i gives us the information, if we have to
// compute the luminance for horizontal or vertical lightlines. Thats a
// hack, but it works.
//
float LightLine::luminance(int i, float luv)
{
   // i has to be 0 or 1!
   double pD, value = 0.0f;
	  
   if (i==0)
	   pD = fabs(point.getX()-luv)/(double)radius;
   else 
	   pD = fabs(point.getY()-luv)/(double)radius;
   if (fabs(pD)>1.0) return 0.0f;
   else {
      switch (lightform) {
              // only constant and linear lightbands are supported ...
              case LightLine::Linear:
                          if (pD > 0.0f) value=1.0f - pD;
                          else value=1.0f + pD;
                          break;
              case LightLine::Quadratic:
                          value = 1.0f - pD*pD;
                          break;
              case LightLine::Polynomial:
                          value = 1.0f - pD*pD*(3.0f - 2.0f * pD);
                          break;
              case LightLine::Constant:
                          value = 1.0f;
                          break;
              default:
                          break;
              }
              return value;
   }
}

//
// Compute the luminance in the lightplane. c and d are coordinates
// of a point in the the light plane. 
//
// We use orthogonal projection, so we assume, the light plane is one of the
// coordinate axis!
float LightLine::luminance(float u, float v)
{
   double lambda, normDir, pD, value=0.0;
  
   // compute the norm sqared of direction
   normDir = direction.getX()*direction.getX() + direction.getY()*direction.getY();
   lambda = ((u-point.getX())*direction.getX() + (v-point.getY())*direction.getY())/normDir;
   
   pD = sqrt((point.getX()+lambda*direction.getX()-u)*(point.getX()+lambda*direction.getX()-u)+
             (point.getY()+lambda*direction.getY()-v)*(point.getY()+lambda*direction.getY()-v));
   pD /= (double)radius;

   if (fabs(pD)>1.0) return 0.0f;
   else {
      switch (lightform) {
              // only constant and linear lightbands are supported ...
              case LightLine::Linear:
                          if (pD > 0.0f) value=1.0 - pD;
                          else value=1.0 + pD;
                          break;
              case LightLine::Quadratic:
                          value = 1.0 - pD*pD;
                          break;
              case LightLine::Polynomial:
                          value = 1.0 - pD*pD*(3.0 - 2.0 * pD);
                          break;
              case LightLine::Constant:
                          value = 1.0;
                          break;
              default:
                          break;
              }
              return value;
   }
}

//
// render with OpenGL
//
void LightLine::draw(void)
{
  float p2[3];
  // compute second point
  getPointOnLine(length, p2);

  if (cylinder) {
	  GLUquadricObj *cyl = gluNewQuadric();
	  gluQuadricNormals(cyl, GLU_SMOOTH);
	  gluQuadricDrawStyle(cyl, GLU_FILL);
	  // TODO: Rotation und Translation so, dass Zylinder passt!
          gluCylinder(cyl, radius, radius, length, 20, 20);
  }
  else {
	  glColor3fv(color);
	  glBegin(GL_LINES);
	  glVertex3f(point.getX(), point.getY(), point.getZ());
	  glVertex3fv(p2);
	  glEnd();       
  }
}

// 
// Creating an OpenGL Display List
//
GLuint LightLine::createList(void)
{
  float p2[3];
  GLuint list = glGenLists(1);
  
  if (!cylinder) {
  // compute second point
  getPointOnLine(length, p2);

  glNewList(list, GL_COMPILE);
       glColor3fv(color);
       glBegin(GL_LINES);
           glVertex3f(point.getX(), point.getY(), point.getZ());
	   glVertex3fv(p2);
       glEnd();       
  glEndList();
  }
  return list;
}

// Translate the lightline
void LightLine::translate(float x, float y, float z)
{
  // translating the light line means changing the point only!
  Vector3 t(x,y,z);
  point += t;
}

// Rotate the lightline
void LightLine::rotateX(float deg)
{
	// Rotate only direction!
        direction.rotateX(deg);
}

// Rotate the lightline
void LightLine::rotateY(float deg)
{
	// Rotate only direction!
        direction.rotateY(deg);
}

// Rotate the lightline
void LightLine::rotateZ(float deg)
{
	// Rotate only direction!
        direction.rotateZ(deg);
}


void LightLine::setPoint(float p[])
{
  point.set(p[0],p[1], p[2]);;
}

void LightLine::setPoint(float x, float y, float z)
{
  point.set(x, y, z);
}

float* LightLine::getPoint(void)
{
  float* local = new float[3];
  
  local[0] = point.getX();
  local[1] = point.getY();
  local[2] = point.getZ();

  return local;
}

void LightLine::setDirection(float dir[])
{
  direction.set(dir[0], dir[1], dir[2]);
}

void LightLine::setDirection(float x, float y, float z)
{
  direction.set(x,y,z);
}

float* LightLine::getDirection(void)
{
  float* local = new float[3];
  
  local[0] = direction.getX();
  local[1] = direction.getY();
  local[2] = direction.getZ();
  return local;
}

void LightLine::setRadius(float r)
{
  radius = r;
}

float LightLine::getRadius(void)
{
  return radius;
}

void LightLine::setColor(float r, float g, float b)
{
  color[0] = r;
  color[1] = g;
  color[2] = b;
}

void LightLine::setColor(float c[3])
{
  color[0] = c[0];
  color[1] = c[1];
  color[2] = c[2];
}

void LightLine::setCylinder(bool c)
{
  cylinder = c;
}

bool LightLine::getCylinder(void)
{
  return cylinder;
}

LightLine::Attenuation LightLine::getAttenuation(void)
{
  return lightform;
}


void LightLine::setAttenuation(LightLine::Attenuation form)
{
   lightform = form;
}


void LightLine::setLength(float s)
{
  length = s;
}

float LightLine::getLength(void)
{
  return length;
}

//
// Private functions
//
void LightLine::write(const char *outFile)
{
  ofstream data;

  data.open(outFile, ios::out | ios::trunc);

  if (data.fail()) {
      cerr << "Outputfile error!" << endl;
      exit(1);
  }
  data << point.getX() << point.getY() << point.getZ() << endl;
  data << direction.getX() << direction.getY() << direction.getZ() << endl;

  data << radius << endl;
  data << length;

  data.close();
}

void LightLine::read(const char *inFile)
{
  float p[3], d[3];
  ifstream data;

  data.open(inFile, ios::in);
  if (data.fail()) {
      cerr << "Inputfile does not exist!" << endl;
      exit(1);
  }
  data >> p[0] >> p[1] >> p[2];
  data >> d[0] >> d[1] >> d[2];

  point.set(p);
  direction.set(d);

  data >> radius;
  data >> length;

  data.close();
}
