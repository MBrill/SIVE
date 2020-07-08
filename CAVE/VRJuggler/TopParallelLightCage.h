// --------------------------------------------------------------------
//  TopParallelLightCage
//  A STL container (list) for LightLines. We handle the special
//  case of parallel lightlines, in a plane parallel to the top plane
//  of the bounding box.
// --------------------------------------------------------------------
//  $RCSfile$
//  $Revision$
//  $Date$
// --------------------------------------------------------------------
#ifndef TOPPARALLELIGHTCAGE
#define TOPPARALLELIGHTCAGE
#include <list>

#include "vlgTextureMap1D.h"
#include "LightCage.h"

using namespace std;

//! A class representing a light cage with parallel lines in a plane
/*!
  A class representing a light cage with parallel lines in the top plane of 
  the bounding box of an object.
*/
class TopParallelLightCage : public LightCage
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
   TopParallelLightCage(void);
   //! Copy-Constructor
   TopParallelLightCage(const TopParallelLightCage&);
   //! Copy-Constructor
   TopParallelLightCage(TopParallelLightCage*);
   //! Constructor based on the bounding box of an object
   /*!
     Constructor based on the bounding box of an object. One line
     is build, centered in the top of the box, parallel to the y-axis.

     We elevate the light cage; 50% of the height of the bounding
     box. This can be changed using TopParallelLightCage::translate().
   */
   TopParallelLightCage(float box[6]); // 1 line, parallel to y, centered
   //! Constructor based on the bounding box of an object
   /*!
     Constructor based on the bounding box of an object. The indicated
     number of lines
     is build, spread equidistant in the top of the box, parallel to the y-axis.

     We elevate the light cage; 50% of the height of the bounding
     box. This can be changed using TopParallelLightCage::translate().
   */
   TopParallelLightCage(float box[6], int);

   //! Constructor based on the bounding box of an object
   /*!
     Constructor based on the bounding box of an object. One line
     is build, centerd in the top of the box. If bool is true parallel
     to the y-axis, if false parallel to the x-axis.

     We elevate the light cage; 50% of the height of the bounding
     box. This can be changed using TopParallelLightCage::translate().
   */
   TopParallelLightCage(float box[6], bool); // boolean true is parallel to y,
                                                // false is parallel to x.
   //! Constructor based on the bounding box of an object
   /*!
     Constructor based on the bounding box of an object. The indicated
     number of lines
     is build, spread equidistant in the top of the box.
     If bool is true parallel to the y-axis, if false parallel to
     the x-axis.

     We elevate the light cage; 50% of the height of the bounding
     box. This can be changed using TopParallelLightCage::translate().
   */
   TopParallelLightCage(float box[6], bool, int);

   //! Compute the luminance, given the lookup-vector ray
   /*!
     Compute the luminance, given the lookup-vector ray.
     ray is assumed to be in texture space [-1,1]^3. 

     It is assumed that the light cylinders do not overlap. That
     could be handeled. But the first hit (luminance >0 for a line)
     stops the computation.
   */
   virtual float luminance(Vector3 ray);
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

   //! Compute the texture map representing the light cage
   /*!
      Compute a texture map representing the light cage. 
      This class represents parallel light lines, so
      we use a one-dimensional texture. 

      The size in int has to be a power of 2!
   */
   virtual vlgTextureMap1D* computeTexture(int);

   // compute a 1D texture coordinate for a given point and normal.
   virtual float* computeTextureCoordinates(float*, float*);

   //! Transform a vector from texture space to the bounding box
   Vector3 transformVectorToBBox(Vector3 vector);
   //! Transform a vector from texture space to the bounding box
   void   transformVectorToBBox(Vector3 vector, Vector3 &transformed);

   //! Query the center of the bounding box
   /*!
     Get back the centerpoint of the BBox defining the translational
     part of [-1,1]^3 -> BBox.
   */
   Vector3 getCenterOfBBox();
   //! Query the center of the bounding box
   /*!
     Get back the centerpoint of the BBox defining the translational
     part of [-1,1]^3 -> BBox.
   */
   void getCenterOfBBox(Vector3 &center);

   //! Query the bounding box as a float array (xmin, xmax, ymin, ymax, zmin, zmax)
   float* getBBox(void);

   //! Query the normal.
   virtual float* getCageNormal(float);
   //! Query the normal.
   virtual void getCageNormal(float, float v[3]);

   //! Translate the cage lines
   virtual void translate(float, float, float);
   //! Rotate the cage lines
   virtual void rotateX(float);
   //! Rotate the cage lines
   virtual void rotateY(float);
   //! Rotate the cage lines
   virtual void rotateZ(float);

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

   // lookup index for horizontal or vertical light lines, created
   // by the corresponding functions. 
   //! Lookup index for horizontal or vertical light lines
   /*!
     Lookup index for horizontal or vertical light lines.
     lui = 0 or vertical means light lines parallel to the y-axis.
     lui = 1 or horizontal means light lines parallel to the x-axis.
     The lookup index is used for the luminance computation. 
   */
   int lui;

   // compound function to set all bbox related variables
   void setBBox(float box[6]);

   // preFilter function for texture maps
   void preFilter(int vh, int size, unsigned short *bigImage,
                                    unsigned short *smallImage);
};
#endif
