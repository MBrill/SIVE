// --------------------------------------------------------------------
//  C++ class for creating a scene containing an interrogated object 
//  and the interrogationlines. We use geometry to render these lines.
// --------------------------------------------------------------------
//  $RCSfile: GeometryRoom.h,v $
//  $Revision: 1.9 $
//  $Date: 2002/05/30 14:49:10 $
// --------------------------------------------------------------------
#ifndef GEOMETRYROOM_H
#define GEOMETRYROOM_H
#include "Room.h"

// Performer headers
#include <Performer/pf/pfChannel.h>
#include <Performer/pf/pfScene.h>
#include <Performer/pfdu.h>
#include <Performer/pf/pfLightSource.h>
#include <Performer/pf/pfDCS.h>
#include <Performer/pf/pfSwitch.h>
#include <Performer/pr/pfGeoState.h>

#include <vtkPolyData.h>

#include "LightCage.h"
#include "HighlightLines.h"
#include "InterrogationObject.h"

//! A class for a scene for surface interrogation
/*!
  The scene contains an interrogated object, a light cage and
  interrogation lines. The lines are computed as isolines, using
  VTK, and rendered as OpenGL Performer lines.
*/
class GeometryRoom : public Room
{
public:

//! Default constructor
/*!
  The OpenGL Performer scene graph is build. 
  No geometry is loaded in the scene
  graph, only structure is defined.

  The navigation speed is set to 0.2,
  the navigation threshold is set to 0.2. 
*/
GeometryRoom(void);
//! Constructor reading the interrogated object from a file
/*!
  The OpenGL Performer scene graph is build and attached to the Performer 
  channel.

  An vtk file is read, integrated in the scene
  graph and rendered.

  The navigation speed is set to 0.2,
  the navigation threshold is set to 0.2. 
*/
GeometryRoom(pfChannel*, char*);
//! Constructor reading the interrogated object from a file
/*!
  The OpenGL Performer scene graph is build and attached to the Performer 
  channel.

  An vtk file in the directory ../data is read, integrated in the scene
  graph and rendered. The object is attached to an instance of the
  class \link InterrogationLines \endlink. No interrogation lines are
  computed. This is done in \link GeometryRoom::compute() \endlink.

  The navigation speed is set to 0.2,
  the navigation threshold is set to 0.2. 
*/
GeometryRoom(pfChannel*, char*, InterrogationLines*);

//! Interactively transform the light cage
/*!
  Computation of the interrogation lines has to be triggered.
*/
void interact(void);

//! Interactively transform the light vector for isophotes
/*!
  Computation of the interrogation lines has to be triggered.
*/
void isophoteInteract(void);

//! Interactively transform the light cage, immediate display of new interrogatoin lines
void fastInteract(void);

//! Interactively transform the light vector for isophotes, immediate display of new interrogation lines
void isophoteFastInteract(void);

//! Like interact(void), but the eye point is queried and set for reflection lines
void reflectInteract(void);

//! Compute the interrogation lines
void compute(void);

// build light cage
// No computation is done!
// -----------------------
virtual void addLightObjects(bool, bool, int);
virtual void addLightObjects(bool, bool, int, LightLine::Attenuation);
virtual void addLightObjects(bool, bool, int, float, LightLine::Attenuation);
// crisscross only needs one bool.
virtual void addLightObjects(bool, int);
virtual void addLightObjects(bool, int, LightLine::Attenuation);
virtual void addLightObjects(bool, int, float, LightLine::Attenuation);

//! Build a light vector for working with isophotes
/*!
  Render isophotes on the object. We only compute and render
  the lines of highest intensity.
*/
virtual void addLightVector(void);
//! Build a light vector for working with isophotes
/*!
  Render a number of isophotes on the object. A reasonable number is >2.
  Then we compute the lines of highest intensity, and the silhouette, where
  the iso-value is 0.
*/
virtual void addLightVector(int num);


//! Change the render color of the interrotagion lines
void setLinesColor(float, float, float);

//! Save the computed interrogation lines as a OpenGL Performer pfb file
/*!
  The file build with this function can be loaded in Perfly.
*/
void writeLight(char*);

//
// private data
//
private:

//! The Performer group containing the lines geometry
pfGroup       *hlinesGeometry;

//! Create the scene tree, without reading any objects, only structure
void createMasterScene(void);
};
#endif
