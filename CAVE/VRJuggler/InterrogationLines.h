// --------------------------------------------------------------------
//  InterrogationLines
//
//  A class to represent lines and curves, 
//  designed to interrogate the surface quality on a polygonal net
// --------------------------------------------------------------------
//  $RCSfile$
//  $Revision$
//  $Date$
// --------------------------------------------------------------------
#ifndef INTERROGATIONLINES
#define INTERROGATIONLINES
#include <list>

#include <GL/glu.h>
#include <vtkFloatArray.h>

#include <vlgGetVTKPolyData.h>
#include <vlgTextureMap2D.h>
#include "LightLine.h"
#include "LightCage.h"
#include "LightVector.h"
#include "InterrogationObject.h"

using namespace std;

//! A base class for interrogation lines
/*!
  The class is designed to interrogate a surface quality on a
  polygonal net.
*/
class InterrogationLines : public vlgGetVTKPolyData
{
public:
   // scalars for the isolines
   //! Compute the lines
   /*!
     This is the central function of all classes derived from
     InterrogationLines.
   */
   virtual void compute(void); // Compute the line
   //! Texture object
   virtual vlgTextureMap2D* computeTexture(int)=0;
   //! Compute the textures coordinates
   virtual void computeTextureCoordinates(void)=0;

   //! Set the light used to interrogate 
   void setLightCage(LightCage*);

   //! Set the light vector used to interrogate in case of isophotes
   void setLightVector(LightVector*);

   //! Set the surface to be interrogated
   void setInterrogationObject(InterrogationObject*);

   //! Set the radius of the light cylinders
   void  setRadius(float);
   //! Query the radius of the light cylinders
   float getRadius(void);

   //! Get the number of lines used to render the result for light cylinders
   int  getNumberOfLines(void);
   //! Set the number of lines used to render the result for light cylinders
   /*!
     For a light cage with light cylinders and rendering the results as
     geometry we render the interrogation bands as a set of lines for every
     band. A minimum reasonable value is 3, for the center and the boundaries.
   */
   void setNumberOfLines(int);

   //! Set the eye point for view dependent interrogation lines
   void   setEyePoint(float eye[3]);
   //! Query the eye point for view dependent interrogation lines
   float* getEyePoint(void);
   //! Query the eye point for view dependent interrogation lines
   void   getEyePoint(float eye[3]);

   //! Turn the prefilter on
   void preFilterOn(void);
   //! Turn the prefilter off
   void preFilterOff(void);
   //! Toggle the prefiltering
   void togglePreFilter(void);

// ----------------------------------------------
//  protected
// ----------------------------------------------
protected:
   //! The object interrogated
   InterrogationObject *surfaceNet; 
   //! The light cage used to interrogate
   LightCage   *cage;       // the light
   //! Für Isophoten wird nur eine Lichtrichtung benötigt!
   LightVector *direction;
   //! The radius used for the light cylinders
   /*!
     Note that the individual light lines in the cage can have
     individual radiuses. Thats not really supported, but would be a
     nice feature.
   */
   float       radius;      // radius used for the lightlines
                            // Note that the individual lightlines
                            // in cage could have individual radiuses.
                            // Thats not supported, but would be a nice
                            // feature.
                            // We need no cylinders in cage, lines are enough.
   //! Number of lines rendered for light cylinders
   /*!
     If the light cage contains light cylinders we render the
     interrogation bands using numLines lines. A minimum value for that is 3,
     representing the center line and the boundaries.
   */
   int         numLines;    // if the lightcage contains lightcylinders
                            // decide how many lines we want to generate
                            // on the surface

   //! The eye point, needed for all view dependend interrogation lines
   float       eyePoint[3]; // Eyepoint. Its defined here, so we have 
                            // functions set/getEyePoint for all derived classes.

   //! Color in RGB float[3] used for the rendering of the interrogatin lines
   float       color[3];    // color to render the lines

   //! Toggle to determine, if texture maps are prefiltered.
   /*!
     Default is no. No really satisfying solution implemented at this moment.
   */
   bool       preFilterMap; // Toggle for preFilter texture maps. 
                            // Default is No.

   // private function
   //! Here is the difference!
   /*!
     A class derived from InterrrogationLines has to implement 
     a function computing scalar values for every vertex. ::compute()
     does contouring, no matter what interrogation lines are wanted. The
     scalars make the difference!
   */
   virtual void computeScalars(vtkFloatArray*, list<LightLine>::iterator)=0;  
};
#endif
