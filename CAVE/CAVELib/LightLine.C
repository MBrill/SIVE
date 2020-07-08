// --------------------------------------------------------------------
//  Implementation of LightLine
//  
// --------------------------------------------------------------------
//  $RCSfile: LightLine.C,v $
//  $Revision: 2.10 $
//  $Date: 2002/05/06 15:35:09 $
// --------------------------------------------------------------------
#include <math.h>
#include <fstream.h>
#include <stdlib.h>

#include <Performer/pf/pfGroup.h>
#include <Performer/pf/pfGeode.h>
#include <Performer/pr/pfMaterial.h>
#include <Performer/pr/pfGeoSet.h>
#include <Performer/pr/pfGeoState.h>

#include "vtkLineSource.h"
#include "vtkProperty.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderer.h"
#include "vtkActor.h"

#include "LightLine.h"

LightLine::LightLine(void) 
// Default, point = 0, direction = e_1
{
   point.set(0.0, 0.0, 0.0);
   direction.set(1.0, 0.0, 0.0);

   // line, no cylinder as default
   radius = 0.0;

   // light is white, isn't it?
   color[0] = 1.0;
   color[1] = 1.0;
   color[2] = 1.0;

   cylinder = false;
   lightform = LightLine::Constant;
   length = 1.0;
}

LightLine::LightLine(float p[], float d[])
{
   point.set(p[0], p[1], p[2]);
   direction.set(d[0], d[1], d[2]);
   float dumm = direction.normalize();  

   radius = 0.0;

   // light is white, isn't it?
   color[0] = 1.0;
   color[1] = 1.0;
   color[2] = 1.0;

   cylinder = false;
   lightform = LightLine::Constant;
   length = 1.0;
}

LightLine::LightLine(float p[], float d[], float r, float l)
{
   point.set(p[0], p[1], p[2]);
   direction.set(d[0], d[1], d[2]);
   float dumm = direction.normalize();  
   
   radius = r;

   // light is white, isn't it?
   color[0] = 1.0;
   color[1] = 1.0;
   color[2] = 1.0;

   cylinder = false;
   lightform = LightLine::Constant;
   length = l;
}

LightLine::LightLine(float p[], float d[], float r, Attenuation form, float l)
{
   point.set(p[0], p[1], p[2]);
   direction.set(d[0], d[1], d[2]);
   float dumm = direction.normalize();  
   
   radius = r;

   // light is white, isn't it?
   color[0] = 1.0;
   color[1] = 1.0;
   color[2] = 1.0;

   cylinder = false;
   lightform = form;
   length = l;
}

LightLine::LightLine(float p[], float d[], float r)
{
   point.set(p[0], p[1], p[2]);
   direction.set(d[0], d[1], d[2]);
   float dumm = direction.normalize();  
   
   radius = r;

   // light is white, isn't it?
   color[0] = 1.0;
   color[1] = 1.0;
   color[2] = 1.0;

   cylinder = false;
   lightform = LightLine::Constant;
   length = 1.0;
}

LightLine::LightLine(float p[], float d[], float r, Attenuation form)
{
   point.set(p[0], p[1], p[2]);
   direction.set(d[0], d[1], d[2]);
   float dumm = direction.normalize();  
   
   radius = r;

   // light is white, isn't it?
   color[0] = 1.0;
   color[1] = 1.0;
   color[2] = 1.0;

   cylinder = false;
   lightform = form;
   length = 1.0;
}

LightLine::LightLine(float p1, float p2, float p3, float d1, 
                     float d2, float d3)
{
   point.set(p1, p2, p3);
   direction.set(d1, d2, d3);
   float dumm = direction.normalize();  
   
   radius = 0;

   // light is white, isn't it?
   color[0] = 1.0;
   color[1] = 1.0;
   color[2] = 1.0;

   cylinder = false;
   lightform = LightLine::Constant;
   length = 1.0;
}

// Set Radius, Attenuation is Constant
LightLine::LightLine(float p1, float p2, float p3, float d1, 
                     float d2, float d3, float r)
{
   point.set(p1, p2, p3);
   direction.set(d1, d2, d3);
   float dumm = direction.normalize();  

   radius = r;

   // light is white, isn't it?
   color[0] = 1.0;
   color[1] = 1.0;
   color[2] = 1.0;

   cylinder = false;
   lightform = LightLine::Constant;
   length = 1.0;
}

// Set Radius and Attenuation
LightLine::LightLine(float p1, float p2, float p3, float d1, 
                     float d2, float d3, float r, Attenuation form)
{
   point.set(p1, p2, p3);
   direction.set(d1, d2, d3);
   float dumm = direction.normalize();  

   radius = r;

   // light is white, isn't it?
   color[0] = 1.0;
   color[1] = 1.0;
   color[2] = 1.0;

   cylinder = false;
   lightform = form;
   length = 1.0;
}

// Read the definition from file
// Read the definition from file
LightLine::LightLine(const char* inFile) 
{
  ifstream data;
  float p[3], d[3];

  data.open(inFile, ios::in | ios::nocreate);
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
  color[0] = 1.0;
  color[1] = 1.0;
  color[2] = 1.0;

  cylinder = false;
  lightform = LightLine::Constant;
}

void LightLine::getPointOnLine(float lambda, float p[3])
// give parameter, compute point on line
{
  p[0] = point[0] + lambda*direction[0];
  p[1] = point[1] + lambda*direction[1];
  p[2] = point[2] + lambda*direction[2];
}

//
// For a given point and normal, compute the unsigned
// perpendicular distance
//
float LightLine::perpendicularDistance(float origin[3],
                                       float lineDirection[3])
{
   pfVec3 sp(origin[0], origin[1], origin[2]),
          sn(lineDirection[0], lineDirection[1], lineDirection[2]);
   float v = sn.normalize();
   return this->perpendicularDistance(sp,sn);
}

float LightLine::perpendicularDistance(pfVec3 sp,
                                       pfVec3 sn)
{
   // This function assumes, that the Vector sn is
   // normalized before the call !!!
   pfVec3 help;

   // Cross-product
   help.cross(direction, sn);

   return help.dot(point-sp);
}

float LightLine::highlightValue(float surfacePoint[3],
                                float surfaceNormal[3])
{
   pfVec3 sp(surfacePoint[0], surfacePoint[1], surfacePoint[2]),
          sn(surfaceNormal[0], surfaceNormal[1], surfaceNormal[2]);

   // Cross-product
   float v = sn.normalize();
   return this->perpendicularDistance(sp, sn);
}

float LightLine::reflectionValue(float surfacePoint[3],
                                 float surfaceNormal[3],
                                 float eyePoint[3])

{  pfVec3 sp(surfacePoint[0], surfacePoint[1], surfacePoint[2]),
          sn(surfaceNormal[0], surfaceNormal[1], surfaceNormal[2]),
          eye(eyePoint[0], eyePoint[1], eyePoint[2]),
          help;

   help = 2.0f*sn.dot(eye-sp)*sn-eye-sp;
   float v = help.normalize();

   // compute the perpendicular distance with direction help
   return this->perpendicularDistance(sp, help);
}

float LightLine::isophoteValue(float surfaceNormal[3])
{
   pfVec3 perfNormal(surfaceNormal[0], surfaceNormal[1], 
                     surfaceNormal[2]);
   return direction.dot(perfNormal);
}

//
// compute the luminance, using the perpendicular distance, and the
// different lightforms.
//
float LightLine::luminance(pfVec3 center, pfVec3 luv)
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
// compute the luminance in the lightplane. c is a coordinate of a point 
// in the the light plane. i gives us the information, if we have to
// compute the luminance for horizontal or vertical lightlines. Thats a
// hack, but it works.
//
float LightLine::luminance(int i, float luv)
{
   // i has to be 0 or 1!
   double pD = fabs(point[i]-luv)/(double)radius, value=0.0f;
   
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
// compute the luminance in the lightplane. c and d are coordinates
// of a point in the the light plane. 
//
// We use orthogonal projection, so we assume, the light plane is one of the
// coordinate axis!
float LightLine::luminance(float u, float v)
{
   double lambda, normDir, pD, value=0.0;
  
   // compute the norm sqared of direction
   normDir = direction[0]*direction[0] + direction[1]*direction[1];
   lambda = ((u-point[0])*direction[0] + (v-point[1])*direction[1])/normDir;
   
   pD = sqrt((point[0]+lambda*direction[0]-u)*(point[0]+lambda*direction[0]-u)+
              (point[1]+lambda*direction[1]-v)*(point[1]+lambda*direction[1]-v));
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

void LightLine::getGeometry(vtkLineSource *line)
{
  float p1[3], p2[3];

  p1[0] = point[0]; p1[1] = point[1]; p1[2] = point[2];
  getPointOnLine(length, p2);

  line->SetPoint1(p1);
  line->SetPoint2(p2);
}

vtkLineSource* LightLine::getGeometry(void)
{
  vtkLineSource *line = vtkLineSource::New();
  float p1[3], p2[3];

  p1[0] = point[0]; p1[1] = point[1]; p1[2] = point[2];
  getPointOnLine(length, p2);

  line->SetPoint1(p1);
  line->SetPoint2(p2);

  return line;
}

// render to vtkRenderer
// only lines are rendered
void LightLine::render(vtkRenderer *ren)
{
  // radius is ignored, lightline is rendered as a vtkLine
  vtkLineSource     *line   = vtkLineSource::New();
  vtkProperty     *property = vtkProperty::New();
  vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
  vtkActor          *actor  = vtkActor::New();

  line = getGeometry();

  property->SetColor(color);
  mapper->SetInput(line->GetOutput());
  actor->SetMapper(mapper);
  actor->SetProperty(property);
  ren->AddActor(actor);
}

//
// render with Performer, using an existing pfGroup
//
// only lines (cylinder = false)!!
//
void LightLine::render(pfGroup *child)
{
  // get the shared memory for pfMalloc
  void              *arena  = pfGetSharedArena();;

  pfGeoSet          *line   = new pfGeoSet;
  pfGeode           *tree   = new pfGeode;
  pfVec3            *coords = new pfVec3[2];
  pfGeoState        *gstate = new pfGeoState;
  pfMaterial      *material = new pfMaterial;

  float p2[3];

  material->setColor(PFMTL_AMBIENT, color[0], color[1], color[2]);
  material->setColor(PFMTL_DIFFUSE, color[0], color[1], color[2]);
  material->setColor(PFMTL_SPECULAR, color[0], color[1], color[2]);
  //material->setColor(PFMTL_EMISSION, color[0], color[1], color[2]);

  gstate->setAttr(PFSTATE_FRONTMTL, material);
  gstate->setAttr(PFSTATE_BACKMTL, material);

  // compute second point
  getPointOnLine(length, p2);

  // use pfMalloc to create the coords in shared memory
  coords = (pfVec3*) pfMalloc(sizeof(pfVec3)*2, arena);

  coords[0].set(point[0], point[1], point[2]);
  coords[1].set(p2[0], p2[1], p2[2]);

  line->setPrimType(PFGS_LINES);
  line->setNumPrims(1);
  line->setAttr(PFGS_COORD3, PFGS_PER_VERTEX, coords, NULL);

  line->setGState(gstate);
  tree->addGSet(line);

  child->addChild(tree);
}

pfGeode* LightLine::getLine(void)
{
  // get the shared memory for pfMalloc
  void              *arena  = pfGetSharedArena();;

  pfGeoSet          *line   = new pfGeoSet;
  pfVec3            *coords;
  pfGeode           *tree   = new pfGeode;
  pfGeoState        *gstate = new pfGeoState;
  pfMaterial      *material = new pfMaterial;

  float p2[3];

  // use pfMalloc to create the coords in shared memory
  coords = (pfVec3*) pfMalloc(sizeof(pfVec3)*2, arena);

  material->setColor(PFMTL_DIFFUSE, color[0], color[1], color[2]);
  material->setColor(PFMTL_AMBIENT, color[0], color[1], color[2]);
  material->setColor(PFMTL_SPECULAR, color[0], color[1], color[2]);
  //material->setColor(PFMTL_EMISSION, color[0], color[1], color[2]);

  gstate->setAttr(PFSTATE_FRONTMTL, material);

  // compute second point
  getPointOnLine(length, p2);

  coords[0]=point;
  coords[1].set(p2[0], p2[1], p2[2]);

  line->setPrimType(PFGS_LINES);
  line->setNumPrims(1);
  line->setAttr(PFGS_COORD3, PFGS_PER_VERTEX, coords, NULL);

  line->setGState(gstate);

  tree->addGSet(line);

  return tree;
}

// Translate the lightline
void LightLine::translate(float x, float y, float z)
{
  // translating the light line means changing the point only!
  pfVec3 t;
  t.set(x,y,z);
  point += t;
}

// Rotate the lightline
void LightLine::rotate(float degrees, float x, float y, float z)
{
  // rotatin the light line means changing the direction vector only!
  pfMatrix rot;
  rot.makeRot(degrees, x,y,z);
  direction.xformVec(direction,rot);
}


void LightLine::setPoint(float p[])
{
  point.set(p[0],p[1], p[2]);;
}

float* LightLine::getPoint(void)
{
  float local[3] = {point[0], point[1], point[2]};
  return local;
}


void LightLine::setDirection(float dir[])
{
  direction.set(dir[0], dir[1], dir[2]);;
}

float* LightLine::getDirection(void)
{
  float local[3] = {direction[0], direction[1], direction[2]};
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
  data << point[0] << point[1] << point[2] << endl;
  data << direction[0] << direction[1] << direction[2] << endl;

  data << radius << endl;
  data << length;

  data.close();
}

void LightLine::read(const char *inFile)
{
  ifstream data;

  data.open(inFile, ios::in | ios::nocreate);
  if (data.fail()) {
      cerr << "Inputfile does not exist!" << endl;
      exit(1);
  }
  data >> point[0] >> point[1] >> point[2];
  data >> direction[0] >> direction[1] >> direction[2];

  data >> radius;
  data >> length;

  data.close();
}
