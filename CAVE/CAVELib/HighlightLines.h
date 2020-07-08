// --------------------------------------------------------------------
//  HighlightLines.h
//
//  A class to represent highlightlines on a polygonal net
//  Derived from InterrogationLines
// --------------------------------------------------------------------
//  $RCSfile: HighlightLines.h,v $
//  $Revision: 1.14 $
//  $Date: 2002/05/06 15:20:37 $
// --------------------------------------------------------------------
#ifndef HIGHLIGHTlINE_H
#define HIGHLIGHTlINE_H
#include <list.h>
#include <vtkScalars.h>

#include "InterrogationLines.h"

#include "InterrogationObject.h"
#include "LightLine.h"
#include "LightCage.h"

//! A class representing highlight lines on a polygonal net
class HighlightLines : public InterrogationLines
{
public:
   //! Default constructor, the pointers are created, no data
   /*!
     The render color is set to white, the eye point to (0,0,0) (we
     do not need it for highlight lines). PreFiltering is switched on.
   */
   HighlightLines(void); // Default, lines created, no data
   //! Copy constructor
   HighlightLines(const HighlightLines& copy);
   //! Constructor setting the interrogated object and the light cage 
   /*!
     The radius is set to 0.0 without querying the radius in the light cage.

     The render color is set to white, the eye point to (0,0,0) (we
     do not need it for highlight lines). PreFiltering is switched on.
   */
   HighlightLines(InterrogationObject *net, LightCage *lcage);
   //! Constructor setting the interrogated object and the light cage 
   /*!
     The radius is set with to r.

     The render color is set to white, the eye point to (0,0,0) (we
     do not need it for highlight lines). PreFiltering is switched on.
   */
   HighlightLines(InterrogationObject *net, LightCage *lcage, float r);
   //! Constructor setting the interrogated object and the light cage 
   /*!
     The radius is set with to r, the number of lines used to render
     the interrogation bands to num.

     The render color is set to white, the eye point to (0,0,0) (we
     do not need it for highlight lines). PreFiltering is switched on.
   */
   HighlightLines(InterrogationObject *net, LightCage *lcage, float r, int n);
   //! Destructor, deleting the vtkPolygonalData
   ~HighlightLines(void);
                      
   //! Compute the texture map
   /*!
     For highlight lines the texture map is computed as a representation
     of the light cage geometry. So LightCage::computeTexture() is called.
   */
   virtual pfTexture* computeTexture(int);
   //! Compute the texture coordinates
   /*!
     We compute the texture coordinates, store them in vtkTCoords. These
     vtkTCoords are added to the vtkPolygonalData representing the interrogated
     object.

     The computation is done in LightCage::computeTextureCoordinates().
   */
   virtual void       computeTextureCoordinates(void);
   //! Build the texture environment for OpenGL Performer
   /*!
     We blend the luminance values contained in the texture map for the
     highlight lines with the object color.
   */
   virtual pfTexEnv*  buildTexEnv(void);
   //! Build the texture coordinates generator
   /*!
     For highlight lines we compute our own texture coordinates, so actually
     here this is only a dummy.
   */
   virtual pfTexGen*  buildTexGen(void);

   //! Save the computed texture map as vtkStructuredPoints
   virtual vtkScalars* saveTexture(int);

private:
   //! Compute the scalars to contour 
   /*!
     We compute the scalars as vtkScalars, for an individual line in the
     light cage. This function is called in compute().

     We use LightLine::highlightValue() for the computation.
   */
   virtual void computeScalars(vtkScalars*, list<LightLine>::iterator); 
                               
};
#endif
