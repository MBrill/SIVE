// --------------------------------------------------------------------
//  TopParallelLightCage.cpp
//  
//  Implementation file:
//  A STL container (list) for parallel LightLines in the topplane
//  of the interrogated object.
// --------------------------------------------------------------------
//  $RCSfile$
//  $Revision$
//  $Date$
// --------------------------------------------------------------------
#include <cmath>

#include <GL/glu.h>

#include "TopParallelLightCage.h"

using namespace std;

TopParallelLightCage::TopParallelLightCage(void) 
{
   BBox[0] = BBox[2] = BBox[4] = 1.0f;
   BBox[1] = BBox[3] = BBox[4] = -1.0f;
   xs = 1.0f; ys = 1.0f; zs = 1.0f;
   centerx = 0.0f; centery = 0.0f; centerz = 0.0f;
   normal[0] = normal[1] = 0.0f;
   normal[2] = 1.0f;
   lui = 0;

   attenuation = LightLine::Constant;
   radius = 0.0f;
   preFilterMap = false;
}

TopParallelLightCage::TopParallelLightCage(const TopParallelLightCage& copy)
{
   cage = copy.cage;
   for (int i=0; i<6; i++) 
      BBox[i] = copy.BBox[i];

   lui = copy.lui;
   xs = copy.xs; ys = copy.ys; zs = copy.zs;
   centerx = copy.centerx; centery = copy.centery; centerz = copy.centerz;
   normal[0] = normal[1] = 0.0f;
   normal[2] = 1.0f;
   attenuation = copy.attenuation;
   radius = copy.radius;
   preFilterMap = copy.preFilterMap;
}

TopParallelLightCage::TopParallelLightCage(TopParallelLightCage *copy)
{
   cage = copy->cage;
   for (int i=0; i<6; i++) 
      BBox[i] = copy->BBox[i];

   lui = copy->lui;
   xs = copy->xs; ys = copy->ys; zs = copy->zs;
   centerx = copy->centerx; centery = copy->centery; centerz = copy->centerz;
   normal[0] = normal[1] = 0.0f;
   normal[2] = 1.0f;
   attenuation = copy->attenuation;
   radius = copy->radius;
   preFilterMap = copy->preFilterMap;
}

// Just one lightline, centered, parallel to z. We elevate the
// constructed lightlines (50% of the height of the box. 
// This can be changed by LightCage::translate.

TopParallelLightCage::TopParallelLightCage(float box[6])
{
   this->setBBox(box);

   float delta = 0.5f*(BBox[3] - BBox[2]), length = BBox[5] - BBox[4];
   float p[3] = {0.5f*(BBox[1]+BBox[0]), BBox[3]+delta, BBox[4]},
         d[3] = {0.0f, 0.0f, 1.0f};

   this->pushback(p,d,length);

   // set the lookup index for luminance computation
   lui = 0;
   normal[0] = normal[1] = 0.0f;
   normal[2] = 1.0f;
   preFilterMap=false;
}


// Construct lightlines, parallel to z. We elevate the
// constructed lightlines. This can be changed by LightCage::translate.
TopParallelLightCage::TopParallelLightCage(float box[6], int number)
{
   int i;

   this->setBBox(box);

   float delta, length = BBox[5] - BBox[4];

   if (number>1)
      delta = (BBox[1]-BBox[0])/(number-1);
   else
      delta = 0.0f;

   float p[3] = {BBox[0], BBox[3]+delta, BBox[4]}, d[3]={0.0, 0.0, 1.0};

   for (i=0; i<number; i++) {
      this->pushback(p,d, length);
      p[0] += delta;
   }

   // set the lookup index for luminance computation
   lui = 0;
   normal[0] = normal[1] = 0.0f;
   normal[2] = 1.0f;
   preFilterMap=false;
}


// Construct lightline, parallel to z, if vertical is true; parallel
// to x, if vertical is false. We elevate the
// constructed lightlines. This can be changed by LightCage::translate.
TopParallelLightCage::TopParallelLightCage(float box[6], bool vertical)
{
   float delta, length;
   float p[3], d[3];

   this->setBBox(box);
   // amount of raising in y
   delta = 0.5*(BBox[3] - BBox[2]);

   if (vertical) {
       // parallel to z, one line.
       length = BBox[5] - BBox[4];
       p[0] = 0.5*(BBox[1]+BBox[0]); 
       p[1] = BBox[3]+delta;
       p[2] = BBox[4];
       d[0] = 0.0f;
       d[1] = 0.0f;
       d[2] = 1.0f;

       // set the lookup index for luminance computation
       lui = 0;
   }
   else {
       length = BBox[1] - BBox[0];
       p[0] = BBox[0]; 
       p[1] = BBox[3] + delta;
       p[2] = 0.5f*(BBox[4] + BBox[5]);
       d[0] = 1.0f;
       d[1] = 0.0f;
       d[2] = 0.0f;

       // set the lookup index for luminance computation
       lui = 1;
   }

   this->pushback(p,d,length);
   normal[0] = normal[1] = 0.0f;
   normal[2] = 1.0f;
   preFilterMap=false;
}

// Construct a set of lightlines, parallel to y, if vertical is true; parallel
// to x, if vertical is false. We elevate the
// constructed lightlines. This can be changed by LightCage::translate.
TopParallelLightCage::TopParallelLightCage(float box[6], bool vertical,
                                           int number)
{
   int i;
   float delta, length;
   float p[3], d[3];

   this->setBBox(box);

   if (vertical) {
       delta=0.5*(BBox[5] - BBox[4]); 
       length = BBox[3] - BBox[2];
      
       p[1] = BBox[3]+delta;
       p[2] = BBox[4];
       d[0] = 0.0f;
       d[1] = 0.0f;
       d[2] = 1.0f;

       if (number>1) {
           p[0] = BBox[0]; 
           delta = (BBox[1]-BBox[0])/(number-1);
       }
       else {
           p[0] = 0.5*(BBox[1]+BBox[0]); 
           delta = 0.0;
       }

       for (i=0; i<number; i++) {
           this->pushback(p,d, length);
           p[0] += delta;
       }

       // set the lookup index for luminance computation
       lui = 0;
   }
   else {
       delta=0.5*(BBox[5] - BBox[4]); 
       length = BBox[1] - BBox[0];
       p[0] = BBox[0]; 
       p[1] = BBox[3]+delta;
       d[0] = 1.0f;
       d[1] = 0.0f;
       d[2] = 0.0f;

       if (number>1) {
          p[2] = BBox[4];
          delta = (BBox[5]-BBox[4])/(number-1);
       }
       else {
          p[2] = 0.5f*(BBox[5]+BBox[4]);
          delta = 0.0;
       }

       for (i=0; i<number; i++) {
          this->pushback(p,d, length);
          p[1] += delta;
       }

       // set the lookup index for luminance computation
       lui = 1;
   }
   normal[0] = normal[1] = 0.0f;
   normal[2] = 1.0f;
   preFilterMap=false;
}

float* TopParallelLightCage::getBBox(void)
{
   float *box = new float[6];

   box[0] = BBox[0];
   box[1] = BBox[1];
   box[2] = BBox[2];
   box[3] = BBox[3];
   box[4] = BBox[4];
   box[5] = BBox[5];

   return box;
}


// Transforming a lookup-vector (normals or reflected vector)
// in the unit-cube (texture environment!) to the actual used
// Bounding Box of the light cage.
Vector3 TopParallelLightCage::transformVectorToBBox(Vector3 vector)
{
   Vector3 transformed(vector.getX()*xs, vector.getY()*ys, vector.getZ()*zs);
   return transformed;
}

void   TopParallelLightCage::transformVectorToBBox(Vector3 vector, Vector3 &transformed)
{
   transformed.set(vector.getX()*xs, vector.getY()*ys, vector.getZ()*zs);
}


// get back the centerpoint of the BBox (defining the translational
// part of [-1,1]^3 -> BBox.
Vector3 TopParallelLightCage::getCenterOfBBox()
{
   Vector3 center(centerx, centery, centerz);
   return center;
}

void TopParallelLightCage::getCenterOfBBox(Vector3 &center)
{
   center.set(centerx, centery, centerz);
}


// compute the luminance, given the lookup-vector ray
// ray is assumed to be in texture coordinates. 
// Pure virtual function in base class!
float TopParallelLightCage::luminance(Vector3 ray)
{
   bool bang=true;
   float value = 0.0f;

   list<LightLine>::iterator iter = cage.begin();
   list<LightLine>::iterator end = cage.end();

   // We have to transform the ray to world coordinates:
   // we translate
   // with getCenterOfBBox() and scale with
   // transformVectorToBBox().
   Vector3 center(centerx, centery, centerz),
           worldray(ray.getX()*xs, ray.getY()*ys, ray.getZ()*zs);
   
   while ((iter != end) && bang) {
     value = iter->luminance(center, worldray);
     if (value != 0.0f) bang = !bang;
     ++iter;
   }
   return value;
}

// compute the luminance, given the coordinates of a 
// intersection point with the light plane, and an information
// on the lookup-index (horizontal/vertical).
// Pure virtual function in base class!
// 
// We use parallel light lines and a 1D texture. So we ignore
// the second parameter.
float TopParallelLightCage::luminance(float c, float d)
{
   bool bang=true;
   float value = 0.0f;

   list<LightLine>::iterator iter = cage.begin();
   list<LightLine>::iterator end = cage.end();

   while ((iter != end) && bang) {
     value = iter->luminance(lui, c);
     if (value != 0.0f) bang = !bang;
     ++iter;
   }
   return value;
}

// compute a texture representing the light cage. Only
// parallel light cages are supported, we use a one-dimensional
// texture vlgTextureMap1D. It would be possible to handle the
// general case with a two-dimension texture.
// Remember, that the size in int has to be a power of 2!
vlgTextureMap1D* TopParallelLightCage::computeTexture(int size)
{
   int i, localSize, vh=1;
   vlgTextureMap1D *tex = new vlgTextureMap1D;
   float clr[4];

   float c, h, d=0.0f;

   if (preFilterMap) 
      localSize = 4*size;
   else
      localSize = size;

   unsigned short* image = new unsigned short[localSize];
   if (lui == 0) {
      h = (BBox[1]-BBox[0])/(size-1);
      c = BBox[0] + h/2.0f;
   }
   else if (lui == 1) {
      h = (BBox[3]-BBox[2])/(size-1);
      c = BBox[2] + h/2.0f;
   }


   // 1D!
   for (i=0; i<localSize; i++) {
        // We get back floats, so we have to do a cast.
        image[i] = (unsigned short)(this->luminance(c,d)*65535.0f);

        // Update the point coordinate c
        c += h;
   }

   // Do the preFilter
   if (preFilterMap) {
        unsigned short* filteredImage = new unsigned short[size];
  
        preFilter(vh, size, image, filteredImage);

        // Store the texture 
	for (i=0; i<size; i++)
		tex->setPixel(i, image[i]);

   }
   else {
	for (i=0; i<size; i++)
		tex->setPixel(i, image[i]);
   }

   return tex;
}

// compute a 1D texture coordinate for a given point and normal.
float* TopParallelLightCage::computeTextureCoordinates(float *vertex, float *normal)
{
   float *co = new float[2];
   float epsilon = 1.0E-8;
   float p, lambda, zplane = BBox[5] + zs;

   if (fabsf(normal[2]) > epsilon)
       lambda = (zplane - vertex[2])/normal[2]; 
   else {
       co[0] = -100.0f; co[1] = 0.0f;
       return co;
   }

   p = vertex[lui] + lambda * normal[lui];

   if (lui == 0) p = (p-BBox[0])/(BBox[1] - BBox[0]);
   else if (lui == 1) p = (p-BBox[2])/(BBox[3] - BBox[2]);

   co[0] = p;
   co[1] = 0.5f;
   return co;
}

float* TopParallelLightCage::getCageNormal(float x)
{
   float *n = new float[3];
   n[0] = normal[0];
   n[1] = normal[1];
   n[2] = normal[2];

   return n;
}

void TopParallelLightCage::getCageNormal(float x, float n[3])
{
   n[0] = normal[0];
   n[1] = normal[1];
   n[2] = normal[2];
}

// Transform the lightcage
void TopParallelLightCage::translate(float x, float y, float z)
{
   list<LightLine>::iterator iter=cage.begin();
   list<LightLine>::iterator end = cage.end();

   while (iter != end) {
      iter->translate(x,y,z);
      ++iter;
   }

   // translating the BBox
   BBox[0] += x;
   BBox[1] += x;
   BBox[2] += y;
   BBox[3] += y;
   BBox[4] += z;
   BBox[5] += z;

   // the scalings is not affected, but we have to update the center
   centerx += x;
   centery += y;
   centerz += z;
}

void TopParallelLightCage::rotateX(float degrees)
{
   // we rotate the BBox. To get the new values for xmin, xmax, 
   // ymin, ymax, zmin, zmax, we transform the points (xmin, ymin, zmin)
   // and (xmax, ymax, zmax). We don't deform the cube, so we can 
   // read the new values from that. 
   Vector3 vector;

   list<LightLine>::iterator iter=cage.begin();
   list<LightLine>::iterator end = cage.end();

   while (iter != end) {
      iter->rotateX(degrees);
      ++iter;
   }

   vector.set(BBox[0], BBox[2], BBox[4]);
   vector.rotateX(degrees);
   //
   // set back to BBox
   BBox[0] = vector.getX();
   BBox[2] = vector.getY();
   BBox[4] = vector.getZ();

   vector.set(BBox[1], BBox[3], BBox[5]);
   vector.rotateX(degrees);
   // set back to BBox
   BBox[1] = vector.getX();
   BBox[3] = vector.getY();
   BBox[5] = vector.getZ();

   // update the center. Scaling is not affected.
   centerx = 0.5f * (BBox[0] + BBox[1]);
   centery = 0.5f * (BBox[2] + BBox[3]);
   centerz = 0.5f * (BBox[4] + BBox[5]);
}

void TopParallelLightCage::rotateY(float degrees)
{
   // we rotate the BBox. To get the new values for xmin, xmax, 
   // ymin, ymax, zmin, zmax, we transform the points (xmin, ymin, zmin)
   // and (xmax, ymax, zmax). We don't deform the cube, so we can 
   // read the new values from that. 
   Vector3 vector;

   list<LightLine>::iterator iter=cage.begin();
   list<LightLine>::iterator end = cage.end();

   while (iter != end) {
      iter->rotateY(degrees);
      ++iter;
   }

   vector.set(BBox[0], BBox[2], BBox[4]);
   vector.rotateY(degrees);
   //
   // set back to BBox
   BBox[0] = vector.getX();
   BBox[2] = vector.getY();
   BBox[4] = vector.getZ();

   vector.set(BBox[1], BBox[3], BBox[5]);
   vector.rotateY(degrees);
   // set back to BBox
   BBox[1] = vector.getX();
   BBox[3] = vector.getY();
   BBox[5] = vector.getZ();

   // update the center. Scaling is not affected.
   centerx = 0.5f * (BBox[0] + BBox[1]);
   centery = 0.5f * (BBox[2] + BBox[3]);
   centerz = 0.5f * (BBox[4] + BBox[5]);
}

void TopParallelLightCage::rotateZ(float degrees)
{
   // we rotate the BBox. To get the new values for xmin, xmax, 
   // ymin, ymax, zmin, zmax, we transform the points (xmin, ymin, zmin)
   // and (xmax, ymax, zmax). We don't deform the cube, so we can 
   // read the new values from that. 
   Vector3 vector;

   list<LightLine>::iterator iter=cage.begin();
   list<LightLine>::iterator end = cage.end();

   while (iter != end) {
      iter->rotateZ(degrees);
      ++iter;
   }

   vector.set(BBox[0], BBox[2], BBox[4]);
   vector.rotateZ(degrees);
   //
   // set back to BBox
   BBox[0] = vector.getX();
   BBox[2] = vector.getY();
   BBox[4] = vector.getZ();

   vector.set(BBox[1], BBox[3], BBox[5]);
   vector.rotateZ(degrees);
   // set back to BBox
   BBox[1] = vector.getX();
   BBox[3] = vector.getY();
   BBox[5] = vector.getZ();

   // update the center. Scaling is not affected.
   centerx = 0.5f * (BBox[0] + BBox[1]);
   centery = 0.5f * (BBox[2] + BBox[3]);
   centerz = 0.5f * (BBox[4] + BBox[5]);
}

// private function setBBox
void TopParallelLightCage::setBBox(float box[6])
{
   BBox[0] = box[0];
   BBox[1] = box[1];
   BBox[2] = box[2];
   BBox[3] = box[3];
   BBox[4] = box[4];
   BBox[5] = box[5];

   xs = 0.5f * (BBox[1] - BBox[0]);
   ys = 0.5f * (BBox[3] - BBox[2]);
   zs = 0.5f * (BBox[5] - BBox[4]);

   centerx = 0.5f * (BBox[0] + BBox[1]);
   centery = 0.5f * (BBox[2] + BBox[3]);
   centerz = 0.5f * (BBox[4] + BBox[5]);
}

void TopParallelLightCage::preFilter(int vh, int size,
                                        unsigned short *bigImage,
                                        unsigned short *smallImage)
{
   int i,j, k, l, sub=4, superSize = sub*size;
   unsigned short sample, weightSum=20;

   unsigned short filterKernel[4][4] = {1,1,1,1,1,2,2,1,1,2,2,1,1,1,1,1};

   for (j=0; j< vh; j++)
   for (i=0; i< size; i++) {
       sample = 0;
       // Supersample
       for (l=j*sub; l<(j+1)*sub; l++)
       for (k=i*sub; k<(i+1)*sub; k++) {
            sample += filterKernel[l-j*sub][k-i*sub]*bigImage[l*superSize+k];
       }

       smallImage[j*size+i] = sample/weightSum;
   }
}
