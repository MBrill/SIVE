// --------------------------------------------------------------------
//  C++ class for creating a scene containing an interrogated object 
//  and the interrogationlines. We use textures to render these lines.
// --------------------------------------------------------------------
//  $RCSfile: TexturedRoom.h,v $
//  $Revision: 1.9 $
//  $Date: 2002/05/30 14:48:47 $
// --------------------------------------------------------------------
#ifndef TEXTUREDROOM_H
#define TEXTUREDROOM_H
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
#include "InterrogationObject.h"
#include "InterrogationLines.h"

//! A class for a scene for surface interrogation
/*!
  The scene contains an interrogated object and the light cage. The lines
  are computed and rendered as a OpenGL Performer texture map on the
  interrogated object.
*/
class TexturedRoom : public Room
{
public:

//! Default constructor
/*!
  The OpenGL Performer scene graph is build. No geometry is loaded in the
  scene graph, only structure is defined.

  The navigation speed is set to 0.2, the navigation threshold is set to 0.2.

  The size of the bitmap is set to 256 x 256 pixel for 2D and to 256 x 1
  pixel for 1D bitmaps.
*/
TexturedRoom(void);
//! Constructor reading the interrogated object from a file
/*!
  The OpenGL Performer scene graph is build and attached to a Performer
  channel.

  An vtk file is read, integrated in the scene graph and rendered.

  The navigation speed is set to 0.2, the navigation threshold is set to 0.2.

  The size of the bitmap is set to 256 x 256 pixel for 2D and to 256 x 1
  pixel for 1D bitmaps.
*/
TexturedRoom(pfChannel*, char*);
//! Constructor reading the interrogated object from a file
/*!
  The OpenGL Performer scene graph is build and attached to the Performer
  channel.

  An vtk file is read, integrated in the scene
  graph and rendered. The object is attached to an instance of the
  class \link InterrogationLines \endlink. No interrogation lines are
  computed. This is done in \link TexturedRoom::compute() \endlink.

  The navigation speed is set to 0.2,
  the navigation threshold is set to 0.2.

  The size of the bitmap is set to 256 x 256 pixel for 2D and to 256 x 1
  pixel for 1D bitmaps.
*/
TexturedRoom(pfChannel*, char*, InterrogationLines*);

// Giving a bitmapsize as a power of 2. Default is 256.
//! Constructor reading the interrogated object from a file
/*!
  The OpenGL Performer scene graph is build and attached to the Performer
  channel.

  An vtk file is read, integrated in the scene
  graph and rendered. No interrogation lines are
  computed. This is done in \link TexturedRoom::compute() \endlink.

  The navigation speed is set to 0.2,
  the navigation threshold is set to 0.2.

  The size of the bitmap is set to int x int pixel for 2D and to int x 1
  pixel for 1D bitmaps.
*/
TexturedRoom(pfChannel*, char*, int);
//! Constructor reading the interrogated object from a file
/*!
  The OpenGL Performer scene graph is build and attached to the Performer
  channel.

  An vtk file is read, integrated in the scene
  graph and rendered. The object is attached to an instance of the
  class \link InterrogationLines \endlink. No interrogation lines are
  computed. This is done in \link TexturedRoom::compute() \endlink.

  The navigation speed is set to 0.2,
  the navigation threshold is set to 0.2.

  The size of the bitmap is set to int x int pixel for 2D and to int x 1
  pixel for 1D bitmaps.
*/
TexturedRoom(pfChannel*, char*, InterrogationLines*, int);

// interact: interactively transform the lightcage, but compute has to
// be called explicitly
//! Interactively transform the light cage
/*!
  Computation of the interrogation lines has to be triggered.
*/
virtual void interact(void);

//! Interactively transform the light vector for isophotes
/*!
  Computation of the interrogation lines has to be triggered.
*/
void isophoteInteract(void);

// fastInteract: interactively transform the lightcage, the interrogatin
// geometry is recomputed implicitly
//! Interactively transform the light cage, immediate display of new interrogatoin lines
virtual void fastInteract(void);

//! Interactively transform the light vector for isophotes, immediate display of new interrogation lines
void isophoteFastInteract(void);

// reflectInteract: set the eye-Point for ReflectionLines
//! Like interact(void), but the eye point is queried and set for reflection lines
virtual void reflectInteract(void);

//! Compute the interrogation lines
/*!
  The computed lines are used to compute a texture map.
*/
virtual void compute(void);

// build light cage
// No computation is done!
// -----------------------
virtual void addLightObjects(bool, bool, int);
virtual void addLightObjects(bool, bool, int, LightLine::Attenuation);
virtual void addLightObjects(bool, bool, int, float, LightLine::Attenuation);
// for crisscross, we only need one bool
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

//! Set the size of the bitmap in pixel
/*!
  The size of the bitmap should always be a power of 2. Thats a restriction
  we get from OpenGL Performer and OpenGL.
*/
inline void setBitMapSize(int n)
{
   bitmapSize = n;
}

//
// private data
//
private:
//! The size of the bitmap used to render the interrogation lines
/*!
  The size of the bitmap should always be a power of 2. Thats a restriction
  we get from OpenGL Performer and OpenGL.
*/
int bitmapSize; // how big is the bitmap we use. Should be a power of 2!

// Create the scene tree, without reading any objects, only structure
virtual void createMasterScene(void);
};

#endif
