// --------------------------------------------------------------------
//  InterrogationLines.cpp
// 
//  A class to represent lines and curves to interrogate a polygonal net
//  Implementation
// --------------------------------------------------------------------
//  $RCSfile: InterrogationLines.cpp $
//  $Revision: 1.17 $
//  $Date: 2002/04/26 21:32:22 $
// --------------------------------------------------------------------
//  History:  09/19/2001 base version
//            09/19/2001 radius added to simulate light cylinders
// --------------------------------------------------------------------
#include "InterrogationLines.h"

#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkContourFilter.h>

// Bei Isophoten gibt es eine eigene ::compute. Sobald die funktioniert diese
// hier nochmal überarbeiten!
void InterrogationLines::compute(void)
{
   double range[2];
   list<LightLine>::iterator iter = cage->begin(), end = cage->end();

   vtkFloatArray *highlightNumbers = vtkFloatArray::New();
   vtkContourFilter *iso = vtkContourFilter::New();
   vtkPolyData *local = vtkPolyData::New();
 
   local->CopyStructure(surfaceNet->getVTKData());
   highlightNumbers->SetNumberOfValues(surfaceNet->getVTKData()->GetNumberOfPoints());

   // Set up the ContourFilter
   iso->UseScalarTreeOn();
   // if radius >= 0.0 and numLines>1, use range of iso
   if ( (radius> 0.0)&&(numLines>1))
         {
         range[0] = static_cast<double>(-radius); 
	 range[1] = static_cast<double>(radius);
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
          //result = new vlgGetVTKPolyData;
          //result->setData(iso->GetOutput());
	  //result->processData();
          //lines.push_back(result);
          ++iter;
   }

   // clean up
   // scalar values are NOT stored!
   iso->Delete();
   local->Delete();
   highlightNumbers->Delete();
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
