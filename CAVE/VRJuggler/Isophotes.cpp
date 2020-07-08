// --------------------------------------------------------------------
//  Isophotes.cpp
//
//  A class to represent isophotes on a polygonal net
//  Based on class InterrogationLines
//
//  Implementation
// --------------------------------------------------------------------
//  $RCSfile: Isophotes.C,v $
//  $Revision: 2.4 $
//  $Date: 2002/05/07 15:13:41 $
// --------------------------------------------------------------------
#include "Isophotes.h"

#include <vtkPointData.h>
#include <vtkContourFilter.h>

Isophotes::Isophotes(void)
{
   radius = 0.0; numLines = 1;
   color[0] = 1.0, color[1] = 1.0; color[2] = 1.0;
   eyePoint[0] = 0.0f;
   eyePoint[1] = 0.0f;
   eyePoint[2] = 0.0f;
 
   direction = new LightVector;

   preFilterMap = false;
}

Isophotes::Isophotes(const Isophotes& copy)
{
   lines = copy.lines;
   surfaceNet=copy.surfaceNet;
   surfaceNet->getVTKData()->GetPointData()->SetScalars(
     copy.surfaceNet->getVTKData()->GetPointData()->GetScalars());

   cage = copy.cage;
   radius = copy.radius;
   numLines = copy.numLines;
   color[0] = copy.color[0];
   color[1] = copy.color[1];
   color[2] = copy.color[2];
   eyePoint[0] = copy.eyePoint[0];
   eyePoint[1] = copy.eyePoint[1];
   eyePoint[2] = copy.eyePoint[2];

   direction->setDirection((copy.direction)->getDirection());

   preFilterMap = copy.preFilterMap;
}

Isophotes::Isophotes(InterrogationObject *pnet, LightVector *dir)
{
   // pnet == surface to be interrogated
   // dir == light vector for parallel light
   surfaceNet = pnet;
   direction = dir;

   radius = 0.0; 

   numLines = 1;
   color[0] = 1.0, color[1] = 1.0; color[2] = 1.0;
   eyePoint[0] = 0.0f;
   eyePoint[1] = 0.0f;
   eyePoint[2] = 0.0f;

   direction = dir;
   preFilterMap = false;
}

Isophotes::Isophotes(InterrogationObject *pnet, LightVector *dir, int num)
                                       
{
   // pnet == surface to be interrogated
   // dir == light vector for parallel light
   surfaceNet = pnet;
   direction->setDirection(dir->getDirection());

   radius = 0.0; 
   numLines = num;
   color[0] = 1.0, color[1] = 1.0; color[2] = 1.0;
   eyePoint[0] = 0.0f;
   eyePoint[1] = 0.0f;
   eyePoint[2] = 0.0f;

   preFilterMap = false;
}

// Destructor
Isophotes::~Isophotes(void)
{
}


void Isophotes::computeScalars(vtkFloatArray *highlightNumbers,
                               list<LightLine>::iterator line)
{
   int i, noP;
   double *surfaceNormal = new double[3];

   noP = surfaceNet->getNumberOfPoints();
   // Compute the scalars
   for (i=0; i < noP; i++) {
       // Get the current normal
       surfaceNormal = surfaceNet->getVTKData()->GetPointData()->GetNormals()->GetTuple(i);
       // Get the data in the scalar function
       highlightNumbers->SetValue(i,direction->isophoteValue(surfaceNormal));
   }
}

//
// Isophotes need an own compute, we have no light cage, which is used
// by the InterrogationLines::compute() function. Also, we handle the numlines
// in another way.
//
void Isophotes::compute(void)
{
   double range[2];
   list<LightLine>::iterator iter = NULL; // only dummy, but we need it.

   vtkFloatArray *highlightNumbers = vtkFloatArray::New();
   vtkContourFilter *iso = vtkContourFilter::New();
   vlgGetVTKPolyData *result;
   vtkPolyData *local = vtkPolyData::New();

   local->CopyStructure(surfaceNet->getVTKData());
   highlightNumbers->SetNumberOfValues(surfaceNet->getNumberOfPoints());

   // Set up the ContourFilter
   iso->UseScalarTreeOn();
   // Falls numLines>1 wird eine Menge von Konturlinien berechnet
   if (numLines>1)
         {
         range[0] = -1.0; range[1] = 1.0;
         iso->GenerateValues(numLines, range);
         }
   else
         iso->SetValue(0, 0.0);

   this->computeScalars(highlightNumbers, iter);
   // Sicherstellen, dass die Pipeline getriggert wird
   highlightNumbers->Modified();
   local->GetPointData()->SetScalars(highlightNumbers);

   iso->SetInput(local);
   // Update, to make sure the vtk pipeline is executed
   iso->Update();
   setData(iso->GetOutput());

   //verboseOn();
   doLines();
   hasNormals = false;
   noAttributes();
   processData();	
   setPointer();

   // clean up
   // scalar values are NOT stored!
   iso->Delete();
   local->Delete();
   highlightNumbers->Delete();
}

// Remember, that the size in int has to be a power of 2!
//
// No prefiltering is done for that pixel.
vlgTextureMap2D* Isophotes::computeTexture(int size)
{
   int i;
   vlgTextureMap2D *tex = new vlgTextureMap2D;
   short *image, one=(unsigned short)65535.0f, zero=(unsigned short)0;
   int h;

   image = new short[size];
   for (i=0; i<size; i++) image[i] = zero;

   if (numLines==1) 
        image[size-1] = one;
   else {
        h = size/(numLines-1);
        image[0] = one;
        image[size-1] = one;
        for (i=1; i<numLines-1; i++)
            image[i*h-1] = one;
   }

   //for (i=0; i<size; i++) tex->setPixel(i, i, image[i]);
   return tex;
}

// 
// We store the texture coordinate in VTK
void Isophotes::computeTextureCoordinates(void)
{
   int i, noP;
   float isov, *surfaceNormal = new float[3];

}

void Isophotes::preFilter(int vh, int size, 
                          unsigned short *bigImage,
                          unsigned short *smallImage)
{
   int i, j, l, sub=4;
   unsigned short sample, weightSum=4;

   unsigned short filterKernel[4] = {1,2,2,1};

   // Integrate for the first row, then copy
   for (j=0; j< size; j++) {
       sample = 0;
       // Supersample
       for (l=j*sub; l<(j+1)*sub; l++) {
            sample += filterKernel[l-j*sub]*bigImage[l*vh];
       }
       smallImage[j*vh] = sample/weightSum;
   }

   // copy the first row to the rest
   for (i=1; i<vh; i++)
   for (j=0; j<size; j++)
       smallImage[j*vh+i] = smallImage[j*vh];
}

void Isophotes::preFilter(int size,
                          unsigned short *bigImage,
                          unsigned short *smallImage)
{
   int i, k, sub=4;
   unsigned short sample, weightSum=4;

   unsigned short filterKernel[4] = {1,2,2,1};

   for (i=0; i< size; i++) {
       sample = 0;
       // Supersample
       for (k=i*sub; k<(i+1)*sub; k++) {
            sample += filterKernel[k-i*sub]*bigImage[k];
       }
       smallImage[i] = sample/weightSum;
   }
}
