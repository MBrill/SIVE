// --------------------------------------------------------------------
//  LightWVector
//  A class to represent a light direction for parallel light, with
//  a light source at infinity.
// --------------------------------------------------------------------
//  $RCSfile: LightVector.h,v $
//  $Revision: 1.2 $
//  $Date: 2002/04/26 20:33:34 $
// --------------------------------------------------------------------
#ifndef LIGHTVECTOR_H
#define LIGHTVECTOR_H
#include <Performer/pr/pfLinMath.h>

#include <vtkLineSource.h>
#include <vtkRenderer.h>

//!  A class to represent a light direction for parallel light
/*!
  The main reason for this class are isophotes. There we consider
  isolines of light intensity, for a light source at infinity. So we
  we have parallel light beams, represented by this class.
*/
class LightVector
{
public:

   //! Default constructor
   /*!
     Default constructor for LightVector. 
     direction is the z-axis.
     
     Color is white, render length is 1.0. Render origin is (0,0,0).
   */
   LightVector(void); 
   //! Copy constructor
   LightVector(const LightVector&);
   //! Constructor defining the direction
   /*!
     Render color is white, render length is 1.0. Render origin is (0,0,0).
   */
   LightVector(float[]);
   //! Constructor defining the direction
   /*!
     Render color is white, render length is 1.0. Render origin is (0,0,0).
   */

   LightVector(float, float ,float);
   //! Constructor defining direction, color and render length of a LightVector
   /*!
     Constructor. First vector is the direction, second the color,
     Last float is the length used for rendering purposes. 

     Render origin is (0,0,0).
   */
   LightVector(float[], float[], float);

   //! Constructor defining direction, color and render length of a LightVector
   /*!
     Constructor. First three arguments define the direction, the next
     three  the color.
     followed by the render length of the LightVector. 

     Render origin is (0,0,0).
   */
   LightVector(float, float, float, float, float ,float, float);

   //! Constructor defining direction, render origin, color and render length of a LightVector
   /*!
     Constructor. First vector is the direction, second the render origin,
     third float array is color.
     Last float is the length used for rendering purposes. 

     Render origin is (0,0,0).
   */
   LightVector(float[], float[], float[], float);

   //! Constructor defining direction, render origin, color and render length of a LightVector
   /*!
     Constructor. First three arguments define the direction, the next
     three the render origin, then the color as RGB floats,
     followed by the render length of the LightVector. 
   */
   LightVector(float, float, float, float, float, float, float, float ,float, float);

   //! Compute the isophote function
   /*!
     Compute the isophote line function for an instance of LightVector.
     We need a normalized normal vector.
   */
   float isophoteValue(float surfaceNormal[3]);

   //! Get the geometry of the LightVector as a vtkLineSource
   /*!
     We need a point in space to render the line. renderOrigin is used,
     this can be set with ::setRenderOrigin().
   */
   void   getGeometry(vtkLineSource*);
   //! Get the geometry as a vtkLineSource
   /*!
     We need a point in space to render the line. renderOrigin is used,
     this can be set with ::setRenderOrigin().
   */
   vtkLineSource*  getGeometry(void);
   //! Render with vtk
   /*!
     Use the vtkRenderer to render the LightVector in a VTK window.

     We need a point in space to render the line. renderOrigin is used,
     this can be set with ::setRenderOrigin().
   */
   void render(vtkRenderer*);
  
   //! Render in a OpenGL Performer group.
   /*!
     Render the vectoras a OpenGL Performer group. The group
     contains a pfGeode and pfMaterial, based on the render attributes
     defined.

     To render a line we need a origin somewhere. It actually doesn't matter
     where this point is, we visualize parallel light.
   */
   void render(pfGroup*);
   //! Get vectorline geometry as a Performer geometry node
   /*!
     Render vector in a OpenGL Performer pfGeode. The attributes
     are based on the render attributes in the LightVector.

     To render a line we need a origin. This should be set before
     using setRenderOrigin().
   */
   pfGeode* getLine();
   //! Replace the current OpenGL Performer geometry for interactive upates
   void replaceDirection(pfGroup* group);

   //! Rotate the LightVector
   /*!
     Rotate the LightVector. First argument is the angle in float, the
     other define the rotaion axis.
   */
   void rotate(float degrees, float x, float y, float z);

   //! Set direction of vector
   void   setDirection(float[]);
   //! Get direction of vector
   float* getDirection(void);

   //! Set the render origin used for display
   void setRenderOrigin(float origin[3]);
   //! Set the render origin used for display
   void setRenderOrigin(float, float, float);
   //! Query the render origin used for display
   float* getRenderOrigin(void);
   //! Query the render origin used for display
   void getRenderOrigin(float o[3]);
   //! Set the rendering lenght for the LightVector (default is 1.0)
   void  setLength(float);
   //! Query the rendering length for the LightVector
   float getLength(void);
   //! Set the render color
   /*!
     Set the color as RGB float[3] used for rendering the vector.
   */
   void setColor(float r, float g, float b);
   //! Set the render color
   /*!
     Set the color as RGB float[3] used for rendering the line.
   */
   void setColor(float c[3]);

// ----------------------------------------
//   private
// ----------------------------------------
private:
   //! direction of the parallel light
   /*!
     Direction of the light. 
     The vector data is stored using the OpenGL Performer pfVec3.
     
     The direction vector is always treated as a unit lenght vector!
   */
   pfVec3 direction;

   // Render attributes
   //! Render origin
   /*!
     We only store a direction vector as data. But for rendering purposes
     we need a point and a direction. So we store the renderOrigin.
   */
   pfVec3 renderOrigin;

   //! Render attribute length
   /*!
     Float Value representing the length of the rendered polyline. The
     length is in world coordinates; default value is 1.0.
   */
   float length;   // length of the lines to render; default is 1.0
   //! Render attribute color
   /*!
     The color used in rendering functions. Color is stored as RGB floats
     between 0.0 and 1.0.
     Default color is white = (1.0, 1.0, 1.0)
   */
   float color[3]; // color used for rendering, default is white
};
#endif
