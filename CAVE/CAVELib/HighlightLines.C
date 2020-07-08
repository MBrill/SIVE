// --------------------------------------------------------------------
//  HighlightLines.C
//
//  A class to represent highlightlines on a polygonal net
//  Based on class InterrogationLines
//
//  Implementation
// --------------------------------------------------------------------
//  $RCSfile: HighlightLines.C,v $
//  $Revision: 1.15 $
//  $Date: 2002/05/30 13:56:05 $
// --------------------------------------------------------------------
#include "HighlightLines.h"

#include <vtkTCoords.h>

HighlightLines::HighlightLines(void)
{
   radius = 0.0; numLines = 1;
   color[0] = 1.0, color[1] = 1.0; color[2] = 1.0;
   eyePoint[0] = 0.0f;
   eyePoint[1] = 0.0f;
   eyePoint[2] = 0.0f;

   preFilterMap = false;
}

HighlightLines::HighlightLines(const HighlightLines& copy)
{
   lines = copy.lines;
   surfaceNet=copy.surfaceNet;
   surfaceNet->getObject()->GetPointData()->SetScalars(
     copy.surfaceNet->getObject()->GetPointData()->GetScalars());
   surfaceNet->getObject()->GetPointData()->SetTCoords(
     copy.surfaceNet->getObject()->GetPointData()->GetTCoords());

   cage = copy.cage;
   radius = copy.radius;
   numLines = copy.numLines;
   color[0] = copy.color[0];
   color[1] = copy.color[1];
   color[2] = copy.color[2];
   eyePoint[0] = copy.eyePoint[0];
   eyePoint[1] = copy.eyePoint[1];
   eyePoint[2] = copy.eyePoint[2];

   preFilterMap = copy.preFilterMap;
}

HighlightLines::HighlightLines(InterrogationObject *pnet, LightCage *lcage)
{
   // pnet == surface to be interrogated
   // lcage == lights
   surfaceNet = pnet;
   cage = lcage;

   radius = 0.0; numLines = 1;
   color[0] = 1.0, color[1] = 1.0; color[2] = 1.0;
   eyePoint[0] = 0.0f;
   eyePoint[1] = 0.0f;
   eyePoint[2] = 0.0f;

   preFilterMap = false;
}

HighlightLines::HighlightLines(InterrogationObject *pnet, LightCage *lcage, float r)
{
   // pnet == surface to be interrogated
   // lcage == lights
   surfaceNet = pnet;
   cage = lcage;

   radius = r; numLines = 1;
   color[0] = 1.0, color[1] = 1.0; color[2] = 1.0;
   eyePoint[0] = 0.0f;
   eyePoint[1] = 0.0f;
   eyePoint[2] = 0.0f;

   preFilterMap = false;
}

HighlightLines::HighlightLines(InterrogationObject *pnet, LightCage *lcage,
                                       float r, int num)
{
   // pnet == surface to be interrogated
   // lcage == lights
   surfaceNet = pnet;
   cage = lcage;

   radius = r; numLines = num;
   color[0] = 1.0, color[1] = 1.0; color[2] = 1.0;
   eyePoint[0] = 0.0f;
   eyePoint[1] = 0.0f;
   eyePoint[2] = 0.0f;

   preFilterMap = false;
}

// Destructor
HighlightLines::~HighlightLines(void)
{
   surfaceNet->getObject()->Delete();
}


void HighlightLines::computeScalars(vtkScalars *highlightNumbers,
                                    list<LightLine>::iterator line)
{
   int i, noP;
   float *surfacePoint, *surfaceNormal;

   noP = surfaceNet->getNumberOfPoints();

   // Compute the scalars
   for (i=0; i < noP; i++) {
       // Get the current surface point == vertex
       surfacePoint = surfaceNet->getPoint(i);
       // Get the current normal
       surfaceNormal = surfaceNet->getNormal(i);
       // Get the data in the scalar function
       highlightNumbers->SetScalar(i,line->highlightValue(surfacePoint, surfaceNormal));
   }
}

pfTexture* HighlightLines::computeTexture(int size)
{
   if (preFilterMap) cage->setPreFilterOn();

   return cage->computeTexture(size);
}

vtkScalars* HighlightLines::saveTexture(int size)
{
   if (preFilterMap) cage->setPreFilterOn();

   return cage->saveTexture(size);
}

// 
// We store the texture coordinate in VTK
void HighlightLines::computeTextureCoordinates(void)
{
   int i, noP;
   float *surfacePoint = new float[3], *surfaceNormal = new float[3],
         *coo = new float[2];
   vtkTCoords *tcoords = vtkTCoords::New();

   noP = surfaceNet->getNumberOfPoints();
   tcoords->SetNumberOfTCoords(noP);
   tcoords->SetNumberOfComponents(2);

   // Compute the 1D textured coordinates and store them in vtkTCoords  
   for (i=0; i<noP; i++) {
       // Get the current surface point == vertex
       surfacePoint = surfaceNet->getPoint(i);
       // Get the current normal
       surfaceNormal = surfaceNet->getNormal(i);
       // Compute the coordinate (in [0,1]) and store it
       coo = cage->computeTextureCoordinates(surfacePoint, surfaceNormal);
       tcoords->SetTCoord(i,
                coo[0],coo[1],0.0f);
   }
   // Add the tcoords to the VTK data surfaceNet
   surfaceNet->getObject()->GetPointData()->SetTCoords(tcoords);
}

//
// Build the texture environment for rendering the highlight lines
// as textures. We have to compute the texture coordinates!
//
pfTexEnv* HighlightLines::buildTexEnv(void)
{
   pfTexEnv *local = new pfTexEnv;
   // See the remark in ReflectionLines, maybe we use PFTE_MODULATE.

   local->setMode(PFTE_BLEND);

   // the light color is stored in private array color[3]!
   local->setBlendColor(color[0], color[1], color[2], 1.0f);

   return local;
}

pfTexGen* HighlightLines::buildTexGen(void)
{
   pfTexGen *local = new pfTexGen;

   // Do the work, or do it in the GSet, because we don't use automatic
   // texture coordinate generation for highlightlines.
   return local;
}

