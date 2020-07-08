// --------------------------------------------------------------------
//  Class for the management of the interrogated geometry.
// --------------------------------------------------------------------
//  $RCSfile: InterrogationObject.h,v $
//  $Revision: 1.7 $
//  $Date: 2002/05/30 13:58:01 $
// --------------------------------------------------------------------
#ifndef INTERROGATIONOBJECT_H
#define INTERROGATIONOBJECT_H

#include "LightCage.h"

#include <vtkPolyData.h>
#include <vtkCellArray.h>
#include <vtkRenderer.h>

#include <Performer/pf/pfGroup.h>
#include <Performer/pr/pfGeoSet.h>
#include <Performer/pr/pfTexture.h>

//! A class to represent the interrogation object
/*!
  The class InterrogationObject manages the interrogated object, 
  especially there are functions to render it using a texture map,
  related to interogation lines.
*/
class InterrogationObject 
{
// -------------------------------------------
// public
// -------------------------------------------
public:
//! Default constructor
InterrogationObject(void);
//! Copy Constructor
InterrogationObject(const InterrogationObject&);
//! Constructor based on a vtk file
/*!
  The Contructor reads the polygonal data in VTK format. If bool is
  true the interrogation lines will be rendered as a texture map.

  The render color of the object is set to red.
*/
InterrogationObject(char*, bool);
//! Constructor based on a vtk file
/*!
  The Contructor reads the polygonal data in VTK format. No texture map
  is used for the rendering of the interrogation lines.
  
  The render color of the object is set to red.
*/
InterrogationObject(char*);
//! Destructor
/*!
  Delete the vtkPolyData.
*/
~InterrogationObject(void);

//! Read the polygonal data from a file in VTK format 
void readObject(char*);

//! Query the bounding box of the interrogation object
/*!
  The bounding box is given as (xmin, xmax, ymin, ymax, zmin, zmax).
*/
float* getBoundingBox(void);
//! Query the bounding box of the interrogation object
/*!
  The bounding box is given as (xmin, xmax, ymin, ymax, zmin, zmax).
*/
void   getBoundingBox(float b[6]);

//! Query the center point of the bounding box
float* getCenter(void);
//! Query the center point of the bounding box
void   getCenter(float c[3]);

//! Render with VTK 
/*!
  Build a vtkPolyDataMapper, a vtkActor and give that to a vtkRenderer.
*/
void render(vtkRenderer*);
//! Render interrogated object in a group node of OpenGL Performer
void render(pfGroup*);
//! Render interrogated object in a group node of OpenGL Performer
pfGroup* render(void);
//! Replace the geometry contained in the group node 
/*!
  Update the content of the group node, to render the new interrogation
  lines as a texture map.
*/
void replaceObject(pfGroup*);

//! Set the texture mapping objects for OpenGL Performer
/*!
  OpenGL Performer needs a texture map, a texture environment, and
  a pointer to a texture coordinate generator.
*/
void setTextureObjects(pfTexture *tex, pfTexEnv *tEnv, 
                              pfTexGen *tGen);

//! Set the texture map for OpenGL Performer
void setTexture(pfTexture *tex);

//! Replace the texture map for OpenGL Performer
/*!
  The old texture map is deleted.
*/
void replaceTexture(pfTexture *tex);

//! Determine, if the interrogation lines should be rendered with texture mapping
inline void setTextureState(bool t)
{
   textured = t;
}

//! Set the color of the interrogated object
void setColor(float c[3]);
//! Set the color of the interrogated object
void setColor(float r, float g, float b);

//! Query the render color 
float* getColor(void);
//! Query the render color 
void getColor(float c[3]);

//! set the polygonale data to vtkPolyData
void setObject(vtkPolyData *o);

//! Query the number of vertices in the polygonal data
int getNumberOfPoints(void);

//! Query the vertex with id i
float* getPoint(int i);

//! Query the normal with id i
float* getNormal(int i);

//! Query the polygonal data as vtkPolyData
vtkPolyData* getObject(void);

/////////////////////////////
// private
/////////////////////////////

private:

//! The vtkPolyData representing the object
vtkPolyData *object;       // Polygonal data of our object
//! The render color
/*!
  The default color is red.
*/
float color[3];            // Global render color, default is red (1,0,0) ...
//! Textured or not textured - Hamlet?
bool textured;
// private members to render with textures
//! Pointer to OpenGL Performer texture map
pfTexture *texture;        // the Texture map,
//! Pointer to a OpenGL Performer texture environment
pfTexEnv  *texEnv;         // the Texture environment,
//! Pointer to the OpenGL Performer texture coordinate generator for sphere mapping and reflection lines
pfTexGen  *texGen;         // the Texture coordinate generator for environment
                           // mapping.

// Private function for Performer rendering, got from vtkActorToPf
pfGeoSet* processPrimitive(vtkCellArray*, int);
pfGeoSet* processTexturedPrimitive(vtkCellArray*, int);
};
#endif
