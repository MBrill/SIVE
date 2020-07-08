// --------------------------------------------------------------------
//  TopCrissCrossLightCage.h
//  A STL container (list) for LightLines. We handle the special
//  case of 2 bundles of parallel lightlines, 
//  in a plane parallel to the top plane
//  of the bounding box.
// --------------------------------------------------------------------
//  $RCSfile: TopCrissCrossLightCage.h,v $
//  $Revision: 1.5 $
//  $Date: 2002/05/07 15:13:41 $
// --------------------------------------------------------------------
#ifndef TOPCRISSCROSSLIGHTCAGE_H
#define TOPCRISSCROSSLIGHTCAGE_H
#include <list.h>

#include <Performer/pf/pfGroup.h>
#include <Performer/pr/pfLinMath.h>
#include <Performer/pr/pfTexture.h>

#include <vtkRenderer.h>
#include <vtkScalars.h>

#include "LightCage.h"
#include "LightLine.h"

//! A class representing a light cage with two bundles of parallel lines in a plane
/*!
  A class representing a light cage with two bundles of parallel
  lines in the top plane of the bounding box of an object. The bundles
  are parallel to the x- and y-axis.
*/

class TopCrissCrossLightCage : public LightCage
{
// ----------------------------------------------
// public
// ----------------------------------------------
public: 
   //! Default Constructur
   /*!
     Default Constructor. The bounding box is set to [-1,1]^3; the plane
     normal is set parallel to z.
   */
   TopCrissCrossLightCage(void);
   //! Copy-Constructor
   TopCrissCrossLightCage(const TopCrissCrossLightCage&);
   //! Copy-Constructor
   TopCrissCrossLightCage(TopCrissCrossLightCage*);
   //! Constructor based on the bounding box of an object
   /*!
     Constructor based on the bounding box of an object. Two lines
     are build, centered in the top of the box, one parallel to the 
     x-, one parallel to the y-axis.

     We elevate the light cage; 50% of the height of the bounding
     box. This can be changed using TopCrissCrossLightCage::translate().
   */
   TopCrissCrossLightCage(float box[6]); // 2 lines, one parallel to y and 
                                         // one parallel to x, centered
   //! Constructor based on the bounding box of an object
   /*!
     Constructor based on the bounding box of an object. 
     Two bundles of lines are build, with the indicated
     number of lines respectively.
     In both bundles, the lines are 
     spread equidistant in the top of the box, parallel to the x-axis
     and y-axis.

     We elevate the light cage; 50% of the height of the bounding
     box. This can be changed using TopCrissCrossLightCage::translate().
   */
   TopCrissCrossLightCage(float box[6], int); // int lines, in x and y.
   //! Constructor based on the bounding box of an object
   /*!
     Constructor based on the bounding box of an object. 
     Two bundles of lines are build, with the indicated
     number of lines respectively.
     In both bundles, the lines are 
     spread equidistant in the top of the box, parallel to the x-axis
     and y-axis.

     We elevate the light cage; 50% of the height of the bounding
     box. This can be changed using TopCrissCrossLightCage::translate().
   */
   TopCrissCrossLightCage(float box[6], int, int); // first parallel in y, 
                                                   // second int parallel in x.

   // compute the luminance, given the lookup-vector ray
   // ray is assumed to be in texture coordinates. 
   //! Compute the luminance, given the lookup-vector ray
   /*!
     Compute the luminance, given the lookup-vector ray.
     ray is assumed to be in texture space [-1,1]^3.

     It is assumed that the light cylinders do not overlap. That
     could be handeled. But the first hit (luminance >0 for a line)
     stops the computation.
   */
   virtual float luminance(pfVec3 ray);
   // compute the luminance, given the coordinates of a 
   // intersection point with the light plane.
   //! Compute the luminance, based on an intersection point in the light plane
   /*!
     Compute the luminance, based on the coordinates of a intersection
     point of a ray and the light plane. This function is only called
     for highlight lines. There we use one-dimensional textures, so the
     second coordinate is momentarily ignored. Thats a hack. but it works.
   */
   virtual float luminance(float, float);

   //! Compute a texture representing the light cage
   /*!
      Compute a two dimensional 
      OpenGL Performer texture representing the light cage. 
  
      The size in int has to be a power of 2!
   */
   virtual pfTexture* computeTexture(int);

   //! Compute and save the texture map representing the light cage
   /*!
     Compute the two dimensional 
     texture map representing the light cage and give it
     back as vtkStructuredPoints.

      The size in int has to be a power of 2!
   */
   virtual vtkScalars* saveTexture(int);

   //! Compute texture coordinates for a given point and normal.
   virtual float* computeTextureCoordinates(float*, float*);

   // transforming a lookup-vector out of the unit cube [-1,1]^3
   // to the BBox
   //! Transform a vector from texture space to the bounding box
   pfVec3 transformVectorToBBox(pfVec3 vector);
   //! Transform a vector from texture space to the bounding box
   void   transformVectorToBBox(pfVec3 vector, pfVec3 &transformed);

   // get back the centerpoint of the BBox (defining the translational
   // part of [-1,1]^3 -> BBox.
   //! Query the center of the bounding box
   /*!
     Get back the centerpoint of the BBox defining the translational
     part of [-1,1]^3 -> BBox.
   */
   pfVec3 getCenterOfBBox();
   //! Query the center of the bounding box
   /*!
     Get back the centerpoint of the BBox defining the translational
     part of [-1,1]^3 -> BBox.
   */
   void   getCenterOfBBox(pfVec3 &center);

   //! Query the bounding box as a float array (xmin, xmax, ymin, ymax, zmin, zmax)
   float* getBBox(void);

   //! Query the normal.
   virtual float* getCageNormal(float);
   //! Query the normal.
   virtual void getCageNormal(float, float v[3]);

   //! Translate the cage lines
   virtual void translate(float, float, float);
   //! Rotate the cage lines
   /*!
     Rotate the cage lines. First argument is angle, followed
     by the rotation axis.
   */
   virtual void rotate(float, float, float, float);

// ----------------------------------------------
// private
// ----------------------------------------------
private:

   // BBox   = Bounding Box (xmin, xmax, ymin, ymax, zmin, zmax),
   // We store the centerpoint of the BBox and the scaling
   // information xs, ys, zs, so tranformations from texture space
   // to the BBox are fast.
   //! Axis parallel bounding box
   /*!
     Axis parallel bounding box (xmin, xmax, ymin, ymax, zmin, zmax).
   */
   float BBox[6];
   //! x, y and z scale of the bounding box.
   /*!
     x, y and z scale of the bounding box in world
     coordinates. These numbers are used to
     scale the bounding box to texture space.
   */
   float xs, ys, zs;
   //! Centerpoint of bounding box.
   /*!
     Centerpoint of bounding box in world coordinates. This point
     is needed for translation of the bounding box in texture space.
   */
   float centerx, centery, centerz;
   //! Normal vector of top plane
   /*!
       Normal vector of top plane for the cage. Default is (0,0,1).
   */
   float normal[3];

   // compound function to set all bbox related variables
   void setBBox(float box[6]);

   // prefiltering the textures map
   void preFilter(int, float*, float*);
};
#endif
