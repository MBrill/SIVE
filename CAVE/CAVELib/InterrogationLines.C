// --------------------------------------------------------------------
//  InterrogationLines.C
// 
//  A class to represent lines and curves to interrogate a polygonal net
//  Implementation
// --------------------------------------------------------------------
//  $RCSfile: InterrogationLines.C,v $
//  $Revision: 1.17 $
//  $Date: 2002/04/26 21:32:22 $
// --------------------------------------------------------------------
//  History:  09/19/2001 base version
//            09/19/2001 radius added to simulate light cylinders
// --------------------------------------------------------------------
#include "InterrogationLines.h"

#include <Performer/pfdu.h>
#include <Performer/pf/pfDCS.h>
#include <Performer/pf/pfSCS.h>
#include <Performer/pf/pfGeode.h>
#include <Performer/pr/pfGeoSet.h>
#include <Performer/pr/pfGeoState.h>
#include <Performer/pr/pfMaterial.h>

#include <vtkPolyData.h>
#include <vtkScalars.h>
#include <vtkAppendPolyData.h>
#include <vtkContourFilter.h>
#include <vtkDataSetReader.h>
#include <vtkPolyDataWriter.h>
#include <vtkProperty.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkPolyDataMapper.h>

void InterrogationLines::clearLines(void)
{
// clear the list of computed polylines. This function has to be called,
// if compute is called more than once, and You only want to see the new
// results!
   lines.clear();

}

void InterrogationLines::compute(void)
{
   float range[2];
   list<LightLine>::iterator iter = cage->begin(), end = cage->end();

   vtkScalars *highlightNumbers = vtkScalars::New();
   vtkContourFilter *iso = vtkContourFilter::New();
   vtkPolyData *local = vtkPolyData::New();
 
   local->CopyStructure(surfaceNet->getObject());
   highlightNumbers->SetNumberOfScalars(surfaceNet->getObject()->GetNumberOfPoints());

   // Set up the ContourFilter
   iso->UseScalarTreeOn();
   // if radius >= 0.0 and numLines>1, use range of iso
   if ( (radius> 0.0)&&(numLines>1))
         {
         range[0] = -radius; range[1] = radius;
         iso->GenerateValues(numLines, range);
         }
   else
         iso->SetValue(0, 0.0);

   while (iter != end) {
          this->computeScalars(highlightNumbers, iter);
          // Tell vtk the scalars are new!
          highlightNumbers->Modified();
          local->GetPointData()->SetScalars(highlightNumbers);
          iso->SetInput(local);
          // Update, to make sure the vtk pipeline is executed
          iso->Update();
          vtkPolyData *result = vtkPolyData::New();
          result->CopyStructure(iso->GetOutput());
          lines.push_back(result);
          ++iter;
   }

   // clean up
   // scalar values are NOT stored!
   iso->Delete();
   local->Delete();
   highlightNumbers->Delete();
}

// r/w the line-geometry, using the Performer pfb Format and pfdLoadFile,
// pfdStoreFile
pfNode* InterrogationLines::readLines(const char *inFile)
{
   pfNode *model;
   // Setup the path to look for files
   pfFilePath(".:../data");
   model = pfdLoadFile (inFile);

   return model;
}

void InterrogationLines::writeLinesAsPerformerFile(const char *outFile)
{
   pfGroup *model, *cagelines, *root;
   
   root = new pfGroup; cagelines = new pfGroup; model = new pfGroup;
   root->setName("Interrogation");

   model = getLines();
   model->setName("Lines");

   cagelines = cage->getCage();
   cagelines->setName("Lightcage");

   root->addChild(model);
   root->addChild(cagelines);

   pfFilePath("../data");
   pfdStoreFile(root, outFile);
}

// render with vtk
void InterrogationLines::render(vtkRenderer *renderer) 
{
   vtkProperty *property     = vtkProperty::New();
   list<vtkPolyData*>::iterator iter = lines.begin(), end = lines.end();
 
   property->SetColor(color);

   while (iter != end) {
          vtkPolyDataMapper *mapper = vtkPolyDataMapper::New();
          vtkActor *actor = vtkActor::New();

          mapper->SetInput(*iter);
          actor->SetMapper(mapper);
          actor->SetProperty(property);
          renderer->AddActor(actor);
          ++iter;
   }
}

// render with performer, vtk and vtkActorToPFTranslator are used
void InterrogationLines::render(pfGroup *group)
{
   list<vtkPolyData*>::iterator iter = lines.begin(), end = lines.end();
   pfGeode                      *geo = new pfGeode;

   while (iter != end) {
          // Make sure the VTK objects are up-to-date
          (*iter)->Update();
          
          vtkCellArray *cells = vtkCellArray::New();
          cells = (*iter)->GetLines();
 
          pfGeoSet *gset = new pfGeoSet;
          gset = processPrim(*iter, cells);

          if (gset != NULL) geo->addGSet(gset);

          ++iter;
   }
   group -> addChild(geo);
}

void InterrogationLines::getLines(pfGroup *group)
{
   list<vtkPolyData*>::iterator iter = lines.begin(), end = lines.end();
   pfGeode                      *geo = new pfGeode;

   while (iter != end) {
          // Make sure the VTK objects are up-to-date
          (*iter)->Update();
          
          vtkCellArray *cells = vtkCellArray::New();
          cells = (*iter)->GetLines();
 
          pfGeoSet *gset = new pfGeoSet;
          gset = processPrim(*iter, cells);

          if (gset != NULL) geo->addGSet(gset);

          ++iter;
   }
   group -> addChild(geo);
}

pfGroup* InterrogationLines::getLines(void)
{
   list<vtkPolyData*>::iterator iter = lines.begin(), end = lines.end();
   pfGroup                    *group = new pfGroup;
   pfGeode                      *geo = new pfGeode;

   while (iter != end) {
          // Make sure the VTK objects are up-to-date
          (*iter)->Update();
          
          vtkCellArray *cells = vtkCellArray::New();
          cells = (*iter)->GetLines();
 
          pfGeoSet *gset = new pfGeoSet;
          gset = processPrim(*iter, cells);

          if (gset != NULL) geo->addGSet(gset);

          ++iter;
   }
   group -> addChild(geo);
   return group;
}

// Set the Light
void InterrogationLines::setLightCage(LightCage *lcage)
{
   cage = lcage;
}

// Set the Light
void InterrogationLines::setLightVector(LightVector *ldir)
{
   direction = ldir;
}

// Set the Surface to be interrogated
void InterrogationLines::setInterrogationObject(InterrogationObject *net)
{
   surfaceNet = net;
}

void InterrogationLines::setRadius(float r)
{
   radius = r;
}

float InterrogationLines::getRadius(void)
{
   return radius;
}

void InterrogationLines::setNumberOfLines(int num)
{
   numLines = num;
}

int InterrogationLines::getNumberOfLines(void)
{  
   return numLines;
}


void  InterrogationLines::setEyePoint(float eye[3])
{
   for (int i=0; i<2; i++) eyePoint[i] = eye[i];
}

float* InterrogationLines::getEyePoint(void)
{
   return eyePoint;
}

void InterrogationLines::getEyePoint(float eye[3])
{
   for (int i=0; i<2; i++) eye[i] = eyePoint[i];
}

void InterrogationLines::setColor(float c[3])
{
   color[0] = c[0];
   color[1] = c[1];
   color[2] = c[2];
}

void InterrogationLines::setColor(float r, float g, float b)
{
   color[0] = r;
   color[1] = g;
   color[2] = b;
}

void InterrogationLines::preFilterOn(void)
{
   preFilterMap = true;
}

void InterrogationLines::preFilterOff(void)
{
   preFilterMap = false;
}

void InterrogationLines::togglePreFilter(void)
{
   preFilterMap = !preFilterMap;
}

//
// Private Functions
//
pfGeoSet* InterrogationLines::processPrim(vtkPolyData *data, 
                                          vtkCellArray *lines)
{
   int numPrimitives = lines->GetNumberOfCells();
   if (numPrimitives == 0) return NULL;

   pfGeoSet *gset = new pfGeoSet;
   gset->setPrimType(PFGS_LINESTRIPS);

   int primArraySize = lines->GetNumberOfConnectivityEntries();
   int numIndices    = primArraySize - numPrimitives;

   void *pfArena = pfGetSharedArena();

   // set number of primitives and allocate lenghts
   gset->setNumPrims(numPrimitives); 
   int *lengths = (int *) pfMalloc(numPrimitives*sizeof(int), pfArena);
   gset->setPrimLengths(lengths);

   // copy data from vtkCellArray to GeoSet
   int prim = 0, vert=0;
   int i, npts, *pts;

   pfVec3 *verts = (pfVec3 *)pfMalloc(numIndices*sizeof(pfVec3), pfArena);
;
   for (lines->InitTraversal(); lines->GetNextCell(npts, pts); prim++) {
     lengths[prim] = npts;
     for (i=0; i<npts; i++) {
          float *aVertex = data->GetPoint(pts[i]);
          verts[vert].set(aVertex[0], aVertex[1], aVertex[2]);
          vert++;
     }
   }

   // add the attributes to Performer
   gset->setAttr(PFGS_COORD3, PFGS_PER_VERTEX, verts, NULL);
   // Overall color
   pfMaterial *material = new pfMaterial;
   
   material->setColor(PFMTL_AMBIENT, color[0], color[1], color[2]);
   material->setColor(PFMTL_DIFFUSE, color[0], color[1], color[2]);

   //
   // create a GeoState
   //
   pfGeoState *gstate = new pfGeoState;
   
   gstate->setAttr(PFSTATE_FRONTMTL, material);
   gstate->setAttr(PFSTATE_BACKMTL, material);

   gstate->setMode(PFSTATE_ENLIGHTING, PF_OFF);

   gset->setGState(gstate);

   return gset;
}
