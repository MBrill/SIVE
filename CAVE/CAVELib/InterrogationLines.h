// --------------------------------------------------------------------
//  InterrogationLines.h
//
//  A class to represent lines and curves, 
//  designed to interrogate the surface quality on a polygonal net
// --------------------------------------------------------------------
//  $RCSfile: InterrogationLines.h,v $
//  $Revision: 1.21 $
//  $Date: 2002/05/07 15:13:41 $
// --------------------------------------------------------------------
#ifndef INTERROGATIONLINES_H
#define INTERROGATIONLINES_H
#include <list.h>

#include <Performer/pf/pfGroup.h>
#include <Performer/pf/pfGeode.h>
#include <Performer/pr/pfTexture.h>

#include <vtkStructuredPoints.h>
#include <vtkScalars.h>
#include <vtkCellArray.h>
#include <vtkRenderer.h>

#include "LightLine.h"
#include "LightCage.h"
#include "LightVector.h"
#include "InterrogationObject.h"

//! A base class for interrogation lines
/*!
  The class is designed to interrogate a surface quality on a
  polygonal net.
*/
class InterrogationLines
{
public:
   // scalars for the isolines
   //! Compute the lines
   /*!
     This is the central function of all classes derived from
     InterrogationLines.
   */
   virtual void compute(void); // Compute the line
   //! Texture objects for OpenGL Performer
   virtual pfTexture* computeTexture(int)=0;
   //! Compute the textures coordinates
   virtual void computeTextureCoordinates(void)=0;
   //! Build the texture environment for OpenGL Performer
   virtual pfTexEnv* buildTexEnv(void)=0;
   //! Build the texture coordinate generator for OpenGL Performer
   /*!
     This function is used in case of reflection lines and sphere
     mapping.
   */
   virtual pfTexGen* buildTexGen(void)=0;

   //! Save the computed texture as vtkStructuredPoints
   virtual vtkScalars* saveTexture(int)=0;

   // call clearLines, if main is recomputing, to
   // delete all polylines
   //! Delete all polylines, so a recompute can be done
   void clearLines(void);

   //! Read the line-geometry, using the Performer native pfb-format
   /*!
     A file in pfb-format can be read and rendered in Perfly.
   */
   pfNode* readLines(const char*);
   //! Write the line-geometry, using the Performer native pfb-Format
   /*!
     A file in pfb-format can be read and rendered in Perfly.
   */
   void writeLinesAsPerformerFile(const char*);

   //! Render with vtk
   /*!
     Build a vtkPolyDataMapper, vtkProperty and vtkActor and attach
     it to the vtkRenderer.
   */
   void render(vtkRenderer*);
   //! Render as Performer geometry 
   /*!
     A pfGeode is build containing lines and a pfGeoState. The pfGeode
     is attached to the group node.
   */
   void render(pfGroup*);

   //! Get the lines as Performer Geode attached to a group node
   void getLines(pfGroup*);
   //! Get the lines as Performer Geode attached to a group node
   pfGroup* getLines(void);

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

   //! Set the number of lines used to render the result for light cylinders
   /*!
     For a light cage with light cylinders and rendering the results as
     geometry we render the interrogation bands as a set of lines for every
     band. A minimum reasonable value is 3, for the center and the boundaries.
   */
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

   //! Set the render color for the interrogation lines as RGB floats
   void setColor(float c[3]);
   //! Set the render color for the interrogation lines as RGB floats
   void setColor(float, float, float);

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
   //! List of polygonal data representing the interrogation lines
   /*!
     For every line or cylinder in the light cage we get a polyline. These
     lines are stored using a STL list.
   */
   list<vtkPolyData*> lines;// stores the geometry of the lines.
                            // In case of a polygonal net this is a
                            // 3D polyline. We get a polyline
                            // for every light line in cage.
   //! The object interrogated
   InterrogationObject *surfaceNet; 
                            // the surface, for that the lines
                            // are computed
   //! The light cage used to interrogate
   LightCage   *cage;       // the light
   //! The light vector used to interrogate in case of isophotes
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
   virtual void computeScalars(vtkScalars*, list<LightLine>::iterator)=0;  
                            // compute scalars to contour 
                            // Here is the difference!
                            // pure virtual.

   //
   // private function, to convert between vtk lines and Performer
   //
   pfGeoSet* processPrim(vtkPolyData*, vtkCellArray*);
};
#endif
