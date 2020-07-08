// --------------------------------------------------------------------
//  Isophotes.h
//
//  A class to represent isophotes on a polygonal net
//  Derived from InterrogationLines
// --------------------------------------------------------------------
//  $RCSfile: Isophotes.h,v $
//  $Revision: 2.4 $
//  $Date: 2002/05/07 15:13:41 $
// --------------------------------------------------------------------
#ifndef ISOPHOTES_H
#define ISOPHOTES_H
#include <list.h>
#include <vtkScalars.h>
#include <vtkStructuredPoints.h>

#include "InterrogationLines.h"
#include "InterrogationObject.h"
#include "LightVector.h"
#include "LightCage.h"

//! A class representing isophotes on a polygonal net
class Isophotes : public InterrogationLines
{
// -------------------------------------
//  public
// -------------------------------------
public:
   //! Default constructor, the pointers are created, no data
   /*!
     The render color is set to white, the eye point to (0,0,0) (we
     do not need it for isophotes). PreFiltering is switched on.
   */
   Isophotes(void); // Default, lines created, no data
   //! Copy construtor
   Isophotes(const Isophotes& copy);
   //! Constructor setting the interrogated object and the light vector
   /*!
     The render color is set to white, the eye point to (0,0,0) (we
     do not need it for isophotes lines). PreFiltering is switched on.
     Number of lines to render to 1.
   */
   Isophotes(InterrogationObject *net, LightVector *dir);
   //! Constructor setting the interrogated object and the light vector
   /*!
     The render color is set to white, the eye point to (0,0,0) (we
     do not need it for isophotes). PreFiltering is switched on.
     Number of lines to render to num.
   */
   Isophotes(InterrogationObject *net, LightVector *dir, int num);
   //! Destructor, deleting the vtkPolygonalData
   ~Isophotes(void);
                      
   //! Compute the isolines
   /*!
     Isophotes need their own ::compute(), because the do not use
     a light cage. 
   */
   virtual void compute(void);
   //! Compute the texture map
   /*!
     For isophotes the texture map is computed as a 1D texture, representing
     one line.
     
     This texture is stored as a pfTexture. It is set to wrap and lamp, the
     border color is set to (0, 0, 0, 1).

     The texture represents luminance values in a ramp, from white (1,1,1)
     to black (0,0,0), using int pixel for the ramp.
   */
   virtual pfTexture* computeTexture(int);
   //! Compute texture coordinates
   /*!
     Isophotes are isolines of light intensity, determined by the scalar
     product scal=<normal, light_direction>. The biggest intensity is given for
     scal = 0, thats the color white for that vertex. White as a texture
     coordinate is texcoord = 0.

     No backfaces are culled, so isophotes are rendered for backfaces. Should
     be worked on.
   */
   virtual void       computeTextureCoordinates(void);
   //! Build the texture environment for OpenGL Performer
   /*!
     The computed luminance is blended with the object color.
   */
   virtual pfTexEnv*  buildTexEnv(void);
   //! Build the texture coordinates generator for OpenGL Performer
   /*!
     We compute our texture coordinates for isophotes, so thats a dummy here.
   */
   virtual pfTexGen*  buildTexGen(void);

   //! Save the computed texture map as vtkStructuredPoints
   virtual vtkScalars* saveTexture(int);

   //! Save the computed lines and the light direction as a OpenGL Performer pfb file
   void writeLinesAsPerformerFile(const char*);
// ------------------------------------------
//  private
// ------------------------------------------
private:

   //! Compute the scalars to contour
   /*!
     We compute the scalars as vtkScalars, for an individual line in the
     light cage. This function is called in compute().

     We use LightLine::highlightValue() for the computation.
   */
   virtual void computeScalars(vtkScalars*, list<LightLine>::iterator); 

   // preFilter for 2D (saveTextures computes 2D!
   void preFilter(int vh, int size, unsigned short *bigImage, 
                                    unsigned short *smallImage);
   // preFilter for 1D
   void preFilter(int size, unsigned short *bigImage,
                            unsigned short *smallImage);
};
#endif
