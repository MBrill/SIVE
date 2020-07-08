// --------------------------------------------------------------------
//  LightLine
//! A class to represent a single lightline.
// --------------------------------------------------------------------
//  $RCSfile: LightLine.h,v $
//  $Revision: 2.8 $
//  $Date: 2002/04/24 20:23:47 $
// --------------------------------------------------------------------
//  Version >= 2.0: Using performer classes for vectors and matrices
//
//  There ARE functions for axis-parallel lines. Therefore we should
//  code a derived class ParallelLightLine.
// --------------------------------------------------------------------
#ifndef LightLine_H
#define LightLine_H
#include <Performer/pf/pfGroup.h>
#include <Performer/pr/pfLinMath.h>

#include <vtkLineSource.h>
#include <vtkRenderer.h>

/*!
  LightLine
  A class to represent a single lightline 
  or a single lightcylinder
  for surface interrogation. Note
  the class contains functions for axis-parallel lines, although this
  class should represent general lines.

  We have a line for radius = 0, radius > 0 models a cylinder.
*/
class LightLine
{
public:
   // public enum attenuation, to define
   // several forms of attenuation functions
   // Outside this header file, these constants
   // have to be accessed with the scope-Operator:
   //   line.setAttenuation(LightLine::Linear);
   //! enum Attenuation.
   /*!
     enum Attenuation
     Using this enum the attenuation of light in a lightcylinder
     is modeled. Possible values:
        - Constant,
        - Linear,
        - Quadratic,
        - Polynomial

     The default value ist Constant.
   */
   enum Attenuation {Constant, Linear, Quadratic, Polynomial};
   // Default is Constant.

   //! Default constructor
   /*!
     Default constructor for LightLine. 
     point is the origin, direction
     is parallel to the x-axis, radius is 0.
   */
   LightLine(void); // Default, point = 0, direction = e_1
   //! Constructor defining point and direction of a LightLine
   /*!
     Constructor. First argument is point[3], second argument is
     direction[3].
   */
   LightLine(float[], float[]);
   //! Constructor defining point and direction of a LightLine
   /*!
     Constructor. First three arguments are point coordinates,
     followed by the coordinates of the direction of the line.
   */
   LightLine(float, float, float, float, float ,float);
   //! Constructor defining point, direction and radius of a LightLine
   /*!
     Constructor. First vector is the point, second the direction,
     followed by the radius of the cylinder. radius should be non-negative.
   */
   LightLine(float[], float[], float);
   //! Constructor defining point, direction, radius and attenuation of a LightLine
   /*!
     Constructor. First vector is the point, second the direction,
     followed by the radius of the cylinder. radius should be non-negative.
     Last argument is attenuation.
   */
   LightLine(float[], float[], float, Attenuation);
   //! Constructor defining point, direction, radius and render length of a LightLine
   /*!
     Constructor. First vector is the point, second the direction,
     followed by the radius of the cylinder. radius should be non-negative.
     Last float is the length used for rendering purposes.
   */
   LightLine(float[], float[], float, float);
   //! Constructor defining point, direction, radius, attenuation and render length of a LightLine
   /*!
     Constructor. First vector is the point, second the direction,
     followed by the radius of the cylinder. radius should be non-negative.
     Attenuation defines the attenuation in case of radius >0.
     Last float is the length used for rendering purposes.
   */
   LightLine(float[], float[], float, Attenuation, float);
   //! Constructor defining point, direction and radius of a LightLine
   /*!
     Constructor. First three arguments define the point, the next
     three  the direction.
     followed by the radius of the cylinder. radius should be non-negative.
   */
   LightLine(float, float, float, float, float ,float, float);
   //! Constructor defining point, direction, radius and attenuation of a LightLine
   /*!
     Constructor. First three arguments define the point, the next
     three  the direction.
     followed by the radius of the cylinder. radius should be non-negative.
     Last argument is attenuation in case of radius >0.
   */
   LightLine(float, float, float, float, float ,float, float, 
             Attenuation);
   //! Constructor, reading the definition from a ASCII file.
   /*!
     Constructor. Reading the definition of a light line from ASCII file.
     Not all attributes are supported. The file should look like this:
     x y z of the point
     x y z of the direction
     radius
     length

     The attenuation is set to LightLine::Constant.
   */
   LightLine(const char*); // Read the definition from file

   //! Compute point on line for a given parameter value.
   /*!
     Compute a point on the lightline, for a given parameter value.
   */
   void getPointOnLine(float, float[3]); 

   // perpendicularDistance, the general functions
   //! Compute the perpendicular distance
   /*!
     Compute the perpendicular distance between an instance of LightLine
     and the line given by origin and linedirection. Version for
     Performer data using pfVec3.
   */
   float perpendicularDistance(pfVec3 origin,
                               pfVec3 linedirection);

   //! Compute the perpendicular distance
   /*!
     Compute the perpendicular distance between an instance of LightLine
     and the line given by origin and linedirection. Version for
     float[3] arrays.
   */
   float perpendicularDistance(float origin[3],
                               float linedirection[3]);

   // compute the perpendicular distance of point to line
   // that's the official highligh-routine!
   //! Compute the perpendicular distance for highlightlines
   /*!
     Compute the perpendicular distance between an instance of LightLine
     and the line given by origin and direction. Version for
     float[3] arrays. Thats the "official" highlightlines function.
   */
   float highlightValue(float surfacePoint[3],
                            float surfaceNormal[3]);

   //! Compute the reflection function
   /*!
     Compute the reflection line function for an instance of LightLine.
     We need a line, given bz surfacePoint and surfaceNormal, plus
     the coordinates of the eye point in world coordinates.
   */
   float reflectionValue(float surfacePoint[3],
                         float surfaceNormal[3],
                         float eyePoint[3]);

   //! Compute the isophote function
   /*!
     Compute the isophote line function for an instance of LightLine.
     We need a normalized normal vector.
   */
   float isophoteValue(float surfaceNormal[3]);

   //! Compute the luminance value, corresponding to the light form
   /*!
     Compute the luminance value for an instance of LightLine. We need
     a line given by origin and direction. Verion for Performer
     pfVec3 arrays. Used in texture mapping.
   */
   float luminance(pfVec3, pfVec3);
   // general 2D case
   //! Function in the lightplane, for 1D textures; general case.
   /*!
     compute the luminance in the lightplane. c and d are coordinates
     of a point in the the light plane.

     We use orthogonal projection, so we assume, the light plane is one of the
     coordinate axis!
   */
   float luminance(float c, float d);
   //! Function in the light plane, horizontal or vertical case.
   /*!
     compute the luminance in the lightplane. luv is a coordinate of a point
     in the the light plane. i gives us the information, if we have to
     compute the luminance for horizontal or vertical lightlines. Thats a
     hack, but it works.
   */
   float luminance(int, float);
   //! Get the geometry as a vtkLineSource
   /*!
     Get the geometry of the LightLine as a vtkLineSource.
   */
   void   getGeometry(vtkLineSource*);
   //! Get the geometry as a vtkLineSource
   /*!
     Get the geometry of the LightLine as a vtkLineSource.
   */
   vtkLineSource*  getGeometry(void);
   //! Render with vtk
   /*!
     Use the vtkRenderer to render the LightLine in a VTK window.
   */
   void render(vtkRenderer*);

   //! Set the render color
   /*!
     Set the color as RGB float[3] used for rendering the line.
   */
   void setColor(float r, float g, float b);
   //! Set the render color
   /*!
     Set the color as RGB float[3] used for rendering the line.
   */
   void setColor(float c[3]);
  
   //! Render in a OpenGL Performer group.
   /*!
     Render the line as a OpenGL Performer group. The group
     contains a pfGeode and pfMaterial, based on the render attributes
     defined.
   */
   void render(pfGroup*);
   //! Get the line geometry as a Performer geometry node
   /*!
     Render the light in a OpenGL Performer pfGeode. The attributes
     are based on the render attributes in the LightLine.
   */
   pfGeode* getLine(void);

   //! Translate the lightline
   /*!
     Translate the lightline
   */
   void translate(float x, float y, float z);

   //! Rotate the lightline
   /*!
     Rotate the lightline. First argument is the angle in float, the
     other define the rotaion axis.
   */
   void rotate(float degrees, float x, float y, float z);

   // Get and set the line-definitions
   //! set line point
   void   setPoint(float[]);
   //! Get line point 
   float* getPoint(void);
   //! Set direction of line
   void   setDirection(float[]);
   //! Get direction of line
   float* getDirection(void);
   //! Set radius of the cylinder
   void   setRadius(float);
   //! Get radius of the cylinder
   float  getRadius(void);
   //! Get the attenuation
   Attenuation getAttenuation(void);
   //! Set the attenuation
   void        setAttenuation(Attenuation);   

   //! Determine if the lightcylinder should be rendered as a cylinder
   void setCylinder(bool);
   //! Query the rendering form of the lightcylinder
   bool getCylinder(void);

   //! Set the rendering lenght for the LightLine (default is 1.0)
   void  setLength(float);
   //! Query the rendering length for the LightLine
   float getLength(void);

private:
   // direction is always a unit length vector!
   //! Point and direction of the line.
   /*!
     Point and direction of the light line. 
     The vector data are stored using the OpenGL Performer pfVec3.
     
     The direction vector is always treated as a unit lenght vector!
   */
   pfVec3 point, direction;
   //! Radius of a lightcylinder.
   /*!
     The radiu of the lightcylinder. Default is radius == 0.0;
     It is not predictable what happens for radius < 0.
   */
   float radius; // lightcylinders, default is radius == 0.0
   //! Attenuation of a lightcylinder.
   /*!
     Attenuation of a lightcylinder. Values are
     LightLine::Constant, LightLine::Linear, LightLine::Quadratic,
     LightLine::Polynomial.

     For all attenuation values other than Constant the intensity of
     light is maximum in the center of the cylinder and drops to 0 at the
     border in linear, quadratic or cubic way.
   */
   Attenuation lightform; // only interesting iff radius > 0.0 .

   // Render attributes
   //! Render attribute color
   /*!
     The color used in rendering functions. Color is stored as RGB floats
     between 0.0 and 1.0.
     Default color is white = (1.0, 1.0, 1.0)
   */
   float color[3]; // color used for rendering, default is white
   //! Render attribute determining how lightcylinders are rendered
   /*!
     Boolean value, determining how lightcylinders are rendered.
     true means the cylinder is rendered as a cylinder, false means
     only the centerline is rendered as a line.
     The Default value is false.
   */
   bool  cylinder; // true = rendered as cylinder, false = rendered as lines
                   // default is false


   //! Render attribute length
   /*!
     Float Value representing the length of the rendered polyline. The
     length is in world coordinates; default value is 1.0.
   */
   float length;   // length of the lines to render; default is 1.0

   void write(const char*);
   void read(const char*);
};
#endif