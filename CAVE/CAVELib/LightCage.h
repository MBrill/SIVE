// --------------------------------------------------------------------
//  LightCage.h
//  A pure virtual base class
// --------------------------------------------------------------------
//  $RCSfile: LightCage.h,v $
//  $Revision: 3.9 $
//  $Date: 2002/05/07 16:07:47 $
// --------------------------------------------------------------------
#ifndef LightCage_H
#define LightCage_H
#include <list.h>

#include <vtkScalars.h>

#include <Performer/pf/pfGroup.h>
#include <Performer/pr/pfLinMath.h>
#include <Performer/pr/pfTexture.h>

#include <vtkRenderer.h>

#include "LightLine.h"

//! Base class representing a set of light lines or cylinders, called a cage.
/*!
  A class representing a set of light lines or cylinders, called a light cage.
  The main purpose of this class is to present an interface to the light lines
  and to manage homogenous attributes. 
*/
class LightCage 
{
// ----------------------------------------------
// public
// ----------------------------------------------

public: 

   //! Compute the luminance, given the lookup-vector ray
   /*!
     Compute the luminance value for a lookup-vector ray. 
     This lookup=-vector is assumed to bin in texture coordinates.
     By that we mean it is contained in the unit cube [01]^3.
     Thats how OpenGL stores texture coordinates.
   */ 
   virtual float luminance(pfVec3 ray)=0;
   //! Compute the luminance, given the coordinates of a intersection point with the light plane.
   /*!
     Compute the luminance, given the coordinate of a intersection
     point with the geometry containing the light cage. This is a plane,
     of a cylinder (not supported). 
   */
   virtual float luminance(float, float)=0;

   //! Compute a texture representing the light cage.
   /*!
     Compute a texture representing the light cage. Only
     parallel light cages are supported, we use a one-dimensional
     Performer texture. It would be possible to handle the
     general case with a two-dimension texture.
     Remember, that the size in int has to be a power of 2!
   */
   virtual pfTexture* computeTexture(int)=0;

   //! Give the texture back as a vtkImage (Structured Points) for prints
   virtual vtkScalars* saveTexture(int)=0;

   //! Compute texture coordinates for a given point and normal.
   virtual float* computeTextureCoordinates(float*, float*)=0;

   // These functions can be used to set the attenuation of the
   // lightlines. No inhomogenous lightcages with concern of the
   // attenuation are supported. Maybe in the future.

   //! Iterating over the lines and setting homogenous attenuation
   /*!  
     Function iterating over the lightlines and setting all values,
     overwriting the old ones.
   */
   void   setCageAttenuation(LightLine::Attenuation);
   //! Setting the cage value, new built lines get this value.
   /*!
     Function sets the cage value, so all newly built lines
     get this value.
   */
   void   setAttenuation(LightLine::Attenuation);
   //! Query the cage attenuation
   LightLine::Attenuation getAttenuation(void);

   //! Push a LightLine in the cage.
   void  pushback(LightLine);        // push of LightLine on LightCage
   //! Push a LightLine in the cage, given by float arrays
   void  pushback(float*, float *);  // push of LightLine on LightCage
   //! Push a LightLine in the cage, given by float arrays and a radius
   void  pushback(float*, float *, float);
                                     // push of LightLine on LightCage
                                     // length is third argument
   //! Perpendicular distance for highlight lines
   void computeScalar(float values[], float point[3], float normal[3]);
   //! Distance function for reflection lines
   void computeScalar(float values[], float point[3], float normal[3], float eye[3]);
   //! Function for isophote lines. Only one lightline for isophotes!
   void computeScalar(float &value, float normal[3]);

   //! Get the size of the set
   int size(void);
   //! Is the cage empty?
   bool empty(void);
   //! Get the first lightline in the cage
   LightLine& front(void);
   //! Get the last lightline in the cage
   LightLine& back(void);

   //! Get a iterator, pointing to the first lightline in the cage
   list<LightLine>::iterator begin(void);
   //! Get a iterator, pointing to the last lightline in the cage
   list<LightLine>::iterator end(void);
 
   //! Translate all the cage lines
   virtual void translate(float, float, float)=0;
   //! Rotate all the cage lines. First argument is angle, the others the axis
   virtual void rotate(float, float, float, float)=0;

   //! Render the cage with  a vtkRenderer in a VTK window
   void  render(vtkRenderer*);
   //! Render the cage in a pfGroup with OpenGL Performer
   void  render(pfGroup*);
   //! Get the cage as a pfGroup with OpenGL Performer
   pfGroup* getCage(void);
   //! Get the cage as a pfGroup with OpenGL Performer
   void     getCage(pfGroup*);
   //! Replace the existing geodes with new ones
   /*!
     Render the light cage in a pfGroup. First all lines in the pfGroup
     are deleted, and then new geometry is inserted. 
     This function is needed to interactive transformation of the cage.
   */
   void replaceCage(pfGroup*);

   //! Set the render color of the cage as RGB float array
   void setColor(float color[3]);
   //! Set the render color of the cage as RGB float array
   void setColor(float, float, float);

   //! Set the radius for all lightlines and for the cage to new value
   void  setCageRadius(float);

   //! Set the radius of the cage, so all new lightlines after that get the same radius.
   void  setRadius(float);
   //! Get the cage radius
   float getRadius(void);

   //! Get the bounding box of the cage
   /*!
     Get the bounding box of the cage. The result is a float[6] array
     with (xmin, xmax, ymin, ymax, zmin, zmax).
   */
   virtual float* getBBox(void)=0;

   //! Query the cage normal.
   virtual float* getCageNormal(float)=0;
   //! Query the cage normal.
   virtual void   getCageNormal(float, float v[3])=0;

   //! Turn prefiltering on
   inline void setPreFilterOn(void) {preFilterMap=true;}
   //! Turn prefiltering off
   inline void setPreFilterOff(void) {preFilterMap=false;}
// ----------------------------------------------
// protected
// ----------------------------------------------
protected:
   //! Set of LightLines, using the STL list.
   list<LightLine> cage;

   // homogenous attributes. They are only stored here, so we don't have
   // to query these attributes every time looping over the cage ..
   //! Homogenous radius for all contained lightlines.
   /*!
     Homogenous radius for all contained lightlines. The cage
     is able to contain lightlines with individual radiuses!
   */
   float radius;

   //! Flag for the computation of a pre-filtered texture map
   /*!
     The default is false. Use ::setPreFilterOn() and ::setPreFilterOff()
     to toggle.
   */
   bool preFilterMap;

   //! Homogenous light attenuation for all contained lightlines
   /*!
     Homogenous light attenuation for all contained lightlines. The cage
     is able to contain lightlines with inidiviual light forms!
   */
   LightLine::Attenuation attenuation;
};
#endif
