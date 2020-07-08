// --------------------------------------------------------------------
//  Implementation of LightVector
//  
// --------------------------------------------------------------------
//  $RCSfile: LightVector.C,v $
//  $Revision: 1.3 $
//  $Date: 2002/04/26 21:32:44 $
// --------------------------------------------------------------------
#include <math.h>
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

#include "LightVector.h"

LightVector::LightVector(void) 
// Default, direction = e_3
{
   direction.set(0.0, 0.0, 1.0);

   // light is white, isn't it?
   color[0] = 1.0;
   color[1] = 1.0;
   color[2] = 1.0;

   renderOrigin[0] = 0.0;
   renderOrigin[1] = 0.0;
   renderOrigin[2] = 0.0;

   length = 1.0;
}

LightVector::LightVector(const LightVector &copy)
{
   direction.set(copy.direction[0], copy.direction[1], copy.direction[2]);

   color[0] = copy.color[0];
   color[1] = copy.color[1];
   color[2] = copy.color[2];

   renderOrigin[0] = copy.renderOrigin[0];
   renderOrigin[1] = copy.renderOrigin[1];
   renderOrigin[2] = copy.renderOrigin[2];

   length = copy.length;
}

LightVector::LightVector(float d[])
{
   direction.set(d[0], d[1], d[2]);
   float dumm = direction.normalize();  

   // light is white, isn't it?
   color[0] = 1.0;
   color[1] = 1.0;
   color[2] = 1.0;

   renderOrigin[0] = 0.0;
   renderOrigin[1] = 0.0;
   renderOrigin[2] = 0.0;

   length = 1.0;
}

LightVector::LightVector(float d1, float d2, float d3)
                     
{
   direction.set(d1, d2, d3);
   float dumm = direction.normalize();  
   
   // light is white, isn't it?
   color[0] = 1.0;
   color[1] = 1.0;
   color[2] = 1.0;

   renderOrigin[0] = 0.0;
   renderOrigin[1] = 0.0;
   renderOrigin[2] = 0.0;

   length = 1.0;
}

LightVector::LightVector(float d1, float d2, float d3, float c1, 
                     float c2, float c3, float l)
{
   direction.set(d1, d2, d3);
   float dumm = direction.normalize();  

   // light is white, isn't it?
   color[0] = c1;
   color[1] = c2;
   color[2] = c3;

   renderOrigin[0] = 0.0;
   renderOrigin[1] = 0.0;
   renderOrigin[2] = 0.0;

   length = l;
}

LightVector::LightVector(float d1, float d2, float d3, 
                     float o1, float o2, float o3,
                     float c1, float c2, float c3, float l)
{
   direction.set(d1, d2, d3);
   float dumm = direction.normalize();  

   // light is white, isn't it?
   color[0] = c1;
   color[1] = c2;
   color[2] = c3;

   renderOrigin[0] = o1;
   renderOrigin[1] = o2;
   renderOrigin[2] = o3;

   length = l;
}

LightVector::LightVector(float d[], float c[], float l)
{
   direction.set(d[0], d[1], d[2]);
   float dumm = direction.normalize();  
   
   color[0] = c[0];
   color[1] = c[1];
   color[2] = c[2];

   renderOrigin[0] = 0.0;
   renderOrigin[1] = 0.0;
   renderOrigin[2] = 0.0;

   length=l;
}

LightVector::LightVector(float d[], float o[], float c[], float l)
{
   direction.set(d[0], d[1], d[2]);
   float dumm = direction.normalize();  
   
   color[0] = c[0];
   color[1] = c[1];
   color[2] = c[2];

   renderOrigin[0] = o[0];
   renderOrigin[1] = o[1];
   renderOrigin[2] = o[2];

   length=l;
}

float LightVector::isophoteValue(float surfaceNormal[3])
{
   pfVec3 perfNormal(surfaceNormal[0], surfaceNormal[1], 
                     surfaceNormal[2]);
   return direction.dot(perfNormal);
}

void LightVector::getGeometry(vtkLineSource *line)
{
  float p[3], o[3];

  for (int i=0; i<3; i++) {
    p[i] = renderOrigin[i] + length*direction[i];
    o[i] = renderOrigin[i];
  }

  line->SetPoint1(o);
  line->SetPoint2(p);
}

vtkLineSource* LightVector::getGeometry(void)
{
  vtkLineSource *line = vtkLineSource::New();
  float p[3], o[3];

  for (int i=0; i<3; i++) {
    p[i] = renderOrigin[i] + length*direction[i];
    o[i] = renderOrigin[i];
  }

  line->SetPoint1(o);
  line->SetPoint2(p);
  return line;
}

// render to vtkRenderer
// only lines are rendered
void LightVector::render(vtkRenderer *ren)
{
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
void LightVector::render(pfGroup *child)
{
  // get the shared memory for pfMalloc
  void              *arena  = pfGetSharedArena();;

  pfGeoSet          *line   = new pfGeoSet;
  pfGeode           *tree   = new pfGeode;
  pfVec3            *coords = new pfVec3[2];
  pfGeoState        *gstate = new pfGeoState;
  pfMaterial      *material = new pfMaterial;

  float p[3];
  for (int i=0; i<3; i++)
    p[i] = renderOrigin[i] + length*direction[i];

  // use pfMalloc to create the coords in shared memory
  coords = (pfVec3*) pfMalloc(sizeof(pfVec3)*2, arena);

  material->setColor(PFMTL_AMBIENT, color[0], color[1], color[2]);
  material->setColor(PFMTL_DIFFUSE, color[0], color[1], color[2]);
  material->setColor(PFMTL_SPECULAR, color[0], color[1], color[2]);

  gstate->setAttr(PFSTATE_FRONTMTL, material);
  gstate->setAttr(PFSTATE_BACKMTL, material);

  coords[0].set(renderOrigin[0], renderOrigin[1], renderOrigin[2]);
  coords[1].set(p[0], p[1], p[2]);

  line->setPrimType(PFGS_LINES);
  line->setNumPrims(1);
  line->setAttr(PFGS_COORD3, PFGS_PER_VERTEX, coords, NULL);

  line->setGState(gstate);
  tree->addGSet(line);

  child->addChild(tree);
}

pfGeode* LightVector::getLine()
{
  // get the shared memory for pfMalloc
  void              *arena  = pfGetSharedArena();;

  pfGeoSet          *line   = new pfGeoSet;
  pfVec3            *coords;
  pfGeode           *tree   = new pfGeode;
  pfGeoState        *gstate = new pfGeoState;
  pfMaterial      *material = new pfMaterial;

  float p[3];
  for (int i=0; i<3; i++)
    p[i] = renderOrigin[i] + length*direction[i];

  // use pfMalloc to create the coords in shared memory
  coords = (pfVec3*) pfMalloc(sizeof(pfVec3)*2, arena);

  material->setColor(PFMTL_DIFFUSE, color[0], color[1], color[2]);
  material->setColor(PFMTL_AMBIENT, color[0], color[1], color[2]);
  material->setColor(PFMTL_SPECULAR, color[0], color[1], color[2]);

  gstate->setAttr(PFSTATE_FRONTMTL, material);

  coords[0].set(renderOrigin[0], renderOrigin[1], renderOrigin[2]);
  coords[1].set(p[0], p[1], p[2]);

  line->setPrimType(PFGS_LINES);
  line->setNumPrims(1);
  line->setAttr(PFGS_COORD3, PFGS_PER_VERTEX, coords, NULL);

  line->setGState(gstate);

  tree->addGSet(line);

  return tree;
}

void LightVector::replaceDirection(pfGroup* group)
{
   int childNumber = 0;

   group->replaceChild(group->getChild(childNumber),this->getLine());
}

// Rotate the lightline
void LightVector::rotate(float degrees, float x, float y, float z)
{
  // rotatiang the light line means changing the direction vector only!
  pfMatrix rot;
  rot.makeRot(degrees, x,y,z);
  direction.xformVec(direction,rot);
}

void LightVector::setRenderOrigin(float o[])
{
  renderOrigin.set(o[0], o[1], o[2]);
}

void LightVector::setRenderOrigin(float o1, float o2, float o3)
{
  renderOrigin.set(o1, o2, o3);
}

void LightVector::setDirection(float dir[])
{
  direction.set(dir[0], dir[1], dir[2]);;
}

float* LightVector::getDirection(void)
{
  float local[3] = {direction[0], direction[1], direction[2]};
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
