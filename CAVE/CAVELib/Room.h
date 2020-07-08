// --------------------------------------------------------------------
//  Virtual base class for creating a scene containing an interrogated
//  object and the interrogationlines. 
// --------------------------------------------------------------------
//  $RCSfile: Room.h,v $
//  $Revision: 2.7 $
//  $Date: 2002/04/26 20:33:34 $
// --------------------------------------------------------------------
#ifndef ROOM_H
#define ROOM_H

// Performer headers
#include <Performer/pf/pfChannel.h>
#include <Performer/pf/pfScene.h>
#include <Performer/pfdu.h>
#include <Performer/pf/pfLightSource.h>
#include <Performer/pf/pfDCS.h>
#include <Performer/pf/pfSwitch.h>
#include <Performer/pr/pfGeoState.h>

#include <vtkPolyData.h>

#include "LightVector.h"
#include "TopParallelLightCage.h"
#include "TopCrissCrossLightCage.h"
#include "InterrogationObject.h"
#include "InterrogationLines.h"

//! Base class for creating and managing a scene for surface interrogation
/*!
  This class manages the scene containing the interrogated object and
  the interrogation lines. 
*/
class Room
{
public:

//! Set the pointer hlines to an existing subclass of \link InterrogationLines \endlink
void setInterrogationLines(InterrogationLines*);

//! Define the interaction in the COVE
/*!
  Interactively transform the light cage, but computing the interrogation
  lines and re-display has to be called explicitely.

  Turn the wand know for rotation. Translate with the translation controls.

  Button 1 toggles the transformation of the whole scene and the light cage.
  Button 2 triggers the computatin and re-display of the interrogation lines.
  Button 3 toggles the display of the interrogated object.
*/
virtual void interact(void)=0;
//! Define the interaction in the COVE in case of isophotes
/*!
  Interactively transform the light vector, but computing the interrogation
  lines and re-display has to be called explicitely.

  Turn the wand know for rotation. Translate with the translation controls.

  Button 1 toggles the transformation of the whole scene and the light vector.
  Button 2 triggers the computatin and re-display of the interrogation lines.
  Button 3 toggles the display of the interrogated object.
*/
virtual void isophoteInteract(void)=0;

// fastInteract: interactively transform the lightcage, the interrogatin
// geometry is recomputed implicitly

//! Define the fast interaction in the COVE
/*!
  Interactively transform the light cage. The changed interrogation lines
  are computed and displayed immediately. 

  Turn the wand know for rotation. Translate with the translation controls.

  Button 1 toggles the transformation of the whole scene and the light cage.
  Button 3 toggles the display of the interrogated object.
*/
virtual void fastInteract(void)=0;

//! Define the fast interaction in the COVE for isophotes
/*!
  Interactively transform the light vector. The changed interrogation lines
  are computed and displayed immediately. 

  Turn the wand know for rotation. Translate with the translation controls.

  Button 1 toggles the transformation of the whole scene and the light vector.
  Button 3 toggles the display of the interrogated object.
*/
virtual void isophoteFastInteract(void)=0;

//! Define the interaction in the COVE for reflection lines
/*!
  Acts like interact. Sets the eye-point, so the view-dependent reflection
  lines can be computed.
*/
virtual void reflectInteract(void)=0;

//! callback to compute the interrogation lines
virtual void compute(void)=0;

//! Build a parallel light cage
/*!
  The first bool is to switch between a parallel light cage in the
  top plane or a cylinder shaped cage (not supported yet).
  The second bool switches between vertical and horizontal lines.
  The int argument defines the number of light lines wanted.

  Only the light cage is build. No interrogation lines are computed!
  The light attenuation is constant. The radius is 0, so lines are used.
*/
virtual void addLightObjects(bool, bool, int)=0;
//! Build light cage
/*!
  The first bool is to switch between a parallel light cage in the
  top plane or a cylinder shaped cage (not supported yet).
  The second bool switches between vertical and horizontal lines.
  The int argument defines the number of light lines wanted.
  The last argument determines the attenuation wanted.

  Only the light cage is build. No interrogation lines are computed!
*/
virtual void addLightObjects(bool, bool, int, LightLine::Attenuation)=0;

//! Build a light vector for working with isophotes
/*!
  Render isophotes on the object. We only compute and render
  the lines of highest intensity.
*/
virtual void addLightVector(void)=0;
//! Build a light vector for working with isophotes
/*!
  Render a number of isophotes on the object. A reasonable number is >2.
  Then we compute the lines of highest intensity, and the silhouette, where
  the iso-value is 0.
*/
virtual void addLightVector(int num)=0;


// Adding the radius 
//! Build light cage
/*!
  The first bool is to switch between a parallel light cage in the
  top plane or a cylinder shaped cage (not supported yet).
  The second bool switches between vertical and horizontal lines.
  The int argument defines the number of light lines wanted.
  The float argument defines the radius of the light cylinders.
  The last argument determines the attenuation wanted.

  Only the light cage is build. No interrogation lines are computed!
*/
virtual void addLightObjects(bool, bool, int, float, LightLine::Attenuation)=0;

//! Build a light cage with 2 sets of parallel light lines
/*!
  The bool switches between a light cage in the top plane of the
  bounding box of the interrogated object, or on a cylinder (not supported
  yet).
  The int argument defines the number of light lines wanted. Actually
  we get twice as much, because 2 sets of parallel light lines are build.

  Only the light cage is build. No interrogation lines are computed!
  The light attenuation is constant. The radius is 0, so lines are used.
*/
virtual void addLightObjects(bool, int)=0;
//! Build a light cage with 2 sets of parallel light lines
/*!
  The bool switches between a light cage in the top plane of the
  bounding box of the interrogated object, or on a cylinder (not supported
  yet).
  The int argument defines the number of light lines wanted. Actually
  we get twice as much, because 2 sets of parallel light lines are build.
  The last argument determines the attenuation wanted.

  Only the light cage is build. No interrogation lines are computed!
  The radius is 0, so lines are used.
*/
virtual void addLightObjects(bool, int, LightLine::Attenuation)=0;

//! Build a light cage with 2 sets of parallel light lines
/*!
  The bool switches between a light cage in the top plane of the
  bounding box of the interrogated object, or on a cylinder (not supported
  yet).
  The int argument defines the number of light lines wanted. Actually
  we get twice as much, because 2 sets of parallel light lines are build.
  The float argument defines the radius of the light cylinders.
  The last argument determines the attenuation wanted.

  Only the light cage is build. No interrogation lines are computed!
*/
virtual void addLightObjects(bool, int, float, LightLine::Attenuation)=0;

//! Toggle the visibility of the interrogation lines
void toggleLight(void);
//! Toggle the visibility of the light cage
void toggleCage(void);
//! Query the visibility of the interrogationlines
bool getLightVisibility(void);
//! Toggle the state value for the computation. Not used ...
void toggleComputation(void);
//! Query the computation state
bool getComputation(void);
//! Toggle the visibility of the interrogated object
void toggleInterrogationObject(void);
//! Query  the visibility of the interrogated object
bool getInterrogationObjectVisibility(void);

//! Attach the scene graph to the OpenGL Performer Channel
void attachChannel(pfChannel *channel);

//! Query a pointer to the scene graph
pfScene* getShowRoom(void);

//! Set the navigation speed
void setNavigationSpeed(float);
//! Set the navigatin theshold
void setNavigationThreshold(float);

//! Query the global transformation matrix of the scene graph
pfDCS* getNavigationInfo(void);
//! Query the global transformation matrix of the scene graph
void getNavigationInfo(pfDCS*);

//! set the radius of all lines in the light cage
void setCageRadius(float);
//
// protected data
//
protected:

//! pointer to the interrogated object
InterrogationObject *IObject;
//! pointer to the used lightcage
LightCage     *cage;
//! pointer to the used interrogation lines
InterrogationLines *hlines;
//! pointer to the used lightvector in case of isophotes
LightVector   *direction;

//! pointer to the root of the OpenGL Performer scene graph
pfScene       *scene;
//! Transformation matrix for the root node of the scene graph
pfDCS         *navigate;
//! Node for the interrogation object. Can be toggled
pfSwitch      *objects;
//! Node for the light cage. A group for all the lines
pfGroup       *cageGeometry;
//! Node for the interrogation lines, if they are displayed as geometry. Can be toggled
pfSwitch      *geometry;
//! Global state for the scene graph
pfGeoState    *state;
//! Global light source
pfLightSource *light;

//! State variable. If true the interrogated geometry is visible
bool          interState;
// interState = true: interrogated geometry is visible.

//! State varible for the interrogation geometry. If true interrogation geometry is visible
bool          lightState;

// lightState = true: light geometry (cage and hlines) is visible.

//! State variable for the light cage. If true the cage is visible
bool          cageState;
// cageState = true: cage is visible

//! State for the interactive transformation using the wand knob.
/*!
  If true only the light cage is transformed using the wand. If false,
  all scene objects are transformed. 

  Default is false.
*/
bool          transformState;
// transformState = true: only light line is transformed,
//                = false: all objects are transformed.

//! State variable for the re-computation of isolines. Not used ...
bool          computeState;
// not used at the moment

//! Speed of transformation computed for wand interaction
float         navigationSpeed;
//! Threshold value for wand interaction
/*!
  If value of wand control is lower than threshold no action occurs.
  Should prevent to busy interaction.
*/
float         navigationThreshold;

//! Create the scene tree, without reading any objects, only structure
virtual void createMasterScene(void)=0;
};

#endif
