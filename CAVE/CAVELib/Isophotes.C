// --------------------------------------------------------------------
//  Isophotes.C
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

#include <Performer/pfdu.h>

#include <vtkTCoords.h>
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

   direction->setDirection((copy.direction)->getDirection());

   preFilterMap = copy.preFilterMap;
}

Isophotes::Isophotes(InterrogationObject *pnet, LightVector *dir)
{
   // pnet == surface to be interrogated
   // dir == light vector for parallel light
   surfaceNet = pnet;
   direction->setDirection(dir->getDirection());

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

   radius = 0.0; numLines = num;
   color[0] = 1.0, color[1] = 1.0; color[2] = 1.0;
   eyePoint[0] = 0.0f;
   eyePoint[1] = 0.0f;
   eyePoint[2] = 0.0f;

   preFilterMap = false;
}

// Destructor
Isophotes::~Isophotes(void)
{
   surfaceNet->getObject()->Delete();
}


void Isophotes::computeScalars(vtkScalars *highlightNumbers,
                               list<LightLine>::iterator line)
{
   int i, noP;
   float *surfaceNormal = new float[3];

   noP = surfaceNet->getNumberOfPoints();

   // Compute the scalars
   for (i=0; i < noP; i++) {
       // Get the current normal
       surfaceNormal = surfaceNet->getNormal(i);
       // Get the data in the scalar function
       highlightNumbers->SetScalar(i,direction->isophoteValue(surfaceNormal));
   }
}

//
// Isophotes need an own compute, we have no light cage, which is used
// by the InterrogationLines::compute() function. Also, we handle the numlines
// in another way.
//
void Isophotes::compute(void)
{
   float range[2];
   list<LightLine>::iterator iter = NULL; // only dummy, but we need it.

   vtkScalars *highlightNumbers = vtkScalars::New();
   vtkContourFilter *iso = vtkContourFilter::New();
   vtkPolyData *result = vtkPolyData::New();
   vtkPolyData *local = vtkPolyData::New();

   local->CopyStructure(surfaceNet->getObject());
   highlightNumbers->SetNumberOfScalars(surfaceNet->getObject()->GetNumberOfPoints());

   // Set up the ContourFilter
   iso->UseScalarTreeOn();
   // if numLines>1, use range of iso
   if (numLines>1)
         {
         range[0] = -1.0f; range[1] = 1.0f;
         iso->GenerateValues(numLines, range);
         }
   else
         iso->SetValue(0, 0.0f);

   this->computeScalars(highlightNumbers, iter);
   // Tell vtk the scalars are new!
   highlightNumbers->Modified();
   local->GetPointData()->SetScalars(highlightNumbers);
   iso->SetInput(local);
   // Update, to make sure the vtk pipeline is executed
   iso->Update();
   result->CopyStructure(iso->GetOutput());
   lines.push_back(result);

   // clean up
   // scalar values are NOT stored!
   iso->Delete();
   local->Delete();
   highlightNumbers->Delete();
}

// We use a one-dimensional Performer texture. 
// Remember, that the size in int has to be a power of 2!
//
// No prefiltering is done for that pixel.
pfTexture* Isophotes::computeTexture(int size)
{
   int i;
   pfTexture *tex = new pfTexture;
   pfVec4 clr;
   unsigned short *image, one=(unsigned short)65535.0f, zero=(unsigned short)0;
   int h;

   // Allocating the memory for the bitmap, using pfMalloc
   image = (unsigned short*) pfMalloc(
            sizeof(unsigned short)*size, pfGetSharedArena());

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

   // Store that image in tex, and give it back.
   tex->setImage((uint*) image, 2, size, 1, 0);

   tex->setFormat(PFTEX_INTERNAL_FORMAT, PFTEX_IA_8);
   tex->setRepeat(PFTEX_WRAP, PFTEX_CLAMP);

   clr.set(0.0f, 0.0f, 0.0f, 1.0f);
   tex->setBorderColor(clr);
   tex->setBorderType(PFTEX_BORDER_COLOR);

   return tex;
}

// 
// We store the texture coordinate in VTK
void Isophotes::computeTextureCoordinates(void)
{
   int i, noP;
   float isov, *surfaceNormal = new float[3];
   vtkTCoords *tcoords = vtkTCoords::New();

   noP = surfaceNet->getNumberOfPoints();
   tcoords->SetNumberOfTCoords(noP);
   tcoords->SetNumberOfComponents(2);

   // Compute the texture coordinates and store them in vtkTCoords  
   for (i=0; i<noP; i++) {
       // Get the current normal
       surfaceNormal = surfaceNet->getNormal(i);
       // Compute the coordinate (in [0,1]) and store it
       isov = direction->isophoteValue(surfaceNormal);
       // Take the absolute value, and hope the light direction is
       // a unit vector.
       tcoords->SetTCoord(i,isov,0.5f,0.0f);
   }
   // Add the tcoords to the VTK data surfaceNet
   surfaceNet->getObject()->GetPointData()->SetTCoords(tcoords);
}

//
// Build the texture environment for rendering the highlight lines
// as textures. We have to compute the texture coordinates!
//
pfTexEnv* Isophotes::buildTexEnv(void)
{
   pfTexEnv *local = new pfTexEnv;
   // See the remark in ReflectionLines, maybe we use PFTE_MODULATE.

   local->setMode(PFTE_BLEND);

   // the light color is stored in private array color[3]!
   local->setBlendColor(color[0], color[1], color[2], 1.0f);

   return local;
}

pfTexGen* Isophotes::buildTexGen(void)
{
   pfTexGen *local = new pfTexGen;

   // Do the work, or do it in the GSet, because we don't use automatic
   // texture coordinate generation for highlightlines.
   return local;
}

void Isophotes::writeLinesAsPerformerFile(const char *outFile)
{
   pfGroup *model, *root;

   root = new pfGroup; model = new pfGroup;
   root->setName("Interrogation");

   model = getLines();
   model->setName("Lines");

   root->addChild(model);

   pfFilePath("../data");
   pfdStoreFile(root, outFile);
}

vtkScalars* Isophotes::saveTexture(int size)
{
   int i, j, vh=50;
   unsigned short *image;
   vtkScalars *vtkimage = vtkScalars::New();

   int h;
   unsigned short one=1, zero=0;

   // Allocate the float image
   image = (unsigned short*) pfMalloc(
            sizeof(unsigned short)*vh*size, pfGetSharedArena());

   // Allocating the memory for the bitmap
   vtkimage->SetNumberOfScalars(size*vh);

   for (i=0; i<vh*size; i++)
                 image[i]=zero;

   if (numLines==1) {
        for (j=0; j<vh; j++)
            image[(j+1)*size-1]=one;
   }
   else {
        h = size/(numLines-1);
        for (j=0; j<vh; j++) {
            image[(j+1)*size-1]=one;
            image[j*size]=one;
        }
        for (i=1; i<numLines-1; i++)
            for (j=0; j<vh; j++)
               image[j*size+i*h-1]=one;
   }

   for (i=0; i<vh*size; i++)
                 vtkimage->SetScalar(i, image[i]);

   return vtkimage;
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
