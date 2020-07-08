// --------------------------------------------------------------------
//  Derived from Room.C. Implementation.
// --------------------------------------------------------------------
//  $RCSfile: GeometryRoom.C,v $
//  $Revision: 1.10 $
//  $Date: 2002/04/26 20:32:27 $
// --------------------------------------------------------------------
#include <math.h>
#include <iostream.h>
#include <pfcave.h>

#include <vtkDataSetReader.h>

#include "GeometryRoom.h"

//
// Interaction function for highlight lines, recomputation has
// to be triggered manually.
//
void GeometryRoom::interact(void)
{
  // get the Cave navigation
  float jx = CAVE_JOYSTICK_X, jy = CAVE_JOYSTICK_Y;
  float w[3], mult = jy*navigationSpeed;

  // if the joystick is rotated above a threshold in X, rotate
  // original in nav: z-axis!
  if (fabs(jx) > navigationThreshold) 
  {
      if (transformState) {
          // rotate the lights
          cage->rotate(-5.0*jx, 0.0, 0.0, 1.0);
          cage->replaceCage(cageGeometry);
      }
      else 
          // rotate all objects
          CAVENavRot(-5.0f*jx, 'x');
  }

  // if the joystick is pressed above a threshold in Y, translate
  // along wands front vector
  if (fabs(jy) > navigationThreshold) {
      CAVEGetVector(CAVE_WAND_FRONT, w);

      if (transformState) {
          // translate the lights
          // change the sign to be consistent with world coordinates!
          cage->translate(-w[0]*mult, -w[1] * mult, -w[2] * mult);
          cage->replaceCage(cageGeometry);
      }
      else 
          // translate all objects
          CAVENavTranslate(w[0]*mult, w[1]*mult, w[2]*mult);
  }

  // If Button 1 is pressed toggle the transformation behaviour
  // of light cage.
  if (CAVEBUTTON1) {
          cerr << "Button 1 is pressed" << endl;
          transformState = !transformState;
  }
  // If Button 2 is pressed do the computation
  if (CAVEBUTTON2)  {
          cerr << "Button 2 is pressed" << endl;
          compute();
  }
  // If Button 3 is pressed, toggle InterrotagionObject visibility
  if (CAVEBUTTON3) {
          cerr << "Button 3 is pressed" << endl;
          toggleInterrogationObject();
  }

  // Store the CAVE navigation matrix in the DCS
  // If transformState is true, store it in the DCS navigate,
  // if it is false, store it in cageTransform.
  if (!transformState)
     pfCAVEDCSNavTransform(navigate); 
}

//
// Interaction function for isophotes, recomputation has
// to be triggered manually. Translation of light has no effect, so
// it is ignored.
//
void GeometryRoom::isophoteInteract()
{
  // get the Cave navigation
  float jx = CAVE_JOYSTICK_X, jy = CAVE_JOYSTICK_Y;
  float w[3], mult = jy*navigationSpeed;

  // if the joystick is rotated above a threshold in X, rotate
  // original in nav: z-axis!
  if (fabs(jx) > navigationThreshold)
  {
      if (transformState) {
          // rotate the lights
          direction->rotate(-5.0*jx, 0.0, 1.0, 0.0);
          direction->replaceDirection(cageGeometry);
      }
      else
          // rotate all objects
          CAVENavRot(-5.0f*jx, 'x');
  }

  // if the joystick is pressed above a threshold in Y, translate
  // along wands front vector
  if (fabs(jy) > navigationThreshold) {
      CAVEGetVector(CAVE_WAND_FRONT, w);

      // translating the light vector for isophotes changes nothing, so ignore it
      if (!transformState) 
          CAVENavTranslate(w[0]*mult, w[1]*mult, w[2]*mult);
  }

  // If Button 1 is pressed toggle the transformation behaviour
  // of light cage.
  if (CAVEBUTTON1) {
          cerr << "Button 1 is pressed" << endl;
          transformState = !transformState;
  }
  // If Button 2 is pressed do the computation
  if (CAVEBUTTON2)  {
          cerr << "Button 2 is pressed" << endl;
          compute();
  }
  // If Button 3 is pressed, toggle InterrotagionObject visibility
  if (CAVEBUTTON3) {
          cerr << "Button 3 is pressed" << endl;
          toggleInterrogationObject();
  }

  // Store the CAVE navigation matrix in the DCS
  // If transformState is true, store it in the DCS navigate,
  // if it is false, store it in cageTransform.
  if (!transformState)
     pfCAVEDCSNavTransform(navigate);
}

void GeometryRoom::fastInteract()
{
  // get the Cave navigation
  float jx = CAVE_JOYSTICK_X, jy = CAVE_JOYSTICK_Y;
  float w[3], mult = jy*navigationSpeed;

  // if the joystick is rotated above a threshold in X, rotate
  // original in nav: z-axis!
  if (fabs(jx) > navigationThreshold)
  {
      if (transformState) {
          // rotate the lights
          cage->rotate(-5.0*jx, 0.0, 0.0, 1.0);
          cage->replaceCage(cageGeometry);
          compute();
      }
      else
          // rotate all objects
          CAVENavRot(-5.0f*jx, 'x');
  }

  // if the joystick is pressed above a threshold in Y, translate
  // along wands front vector
  if (fabs(jy) > navigationThreshold) {
      CAVEGetVector(CAVE_WAND_FRONT, w);

      if (transformState) {
          // translate the lights
          cage->translate(-w[0]*mult, -w[1] * mult, -w[2] * mult);
          cage->replaceCage(cageGeometry);
          compute();
      }
      else
          // translate all objects
          CAVENavTranslate(w[0]*mult, w[1]*mult, w[2]*mult);
  }

  // If Button 1 is pressed build one centered lightline, vertical
  if (CAVEBUTTON1) {
          cerr << "Button 1 is pressed" << endl;
          transformState = !transformState;
  }
  // If Button 2 is pressed do the computation
  if (CAVEBUTTON2)  {
          cerr << "Button 2 is pressed" << endl;
          compute();
  }
  // If Button 3 is pressed, toggle InterrotagionObject visibility
  if (CAVEBUTTON3) {
          cerr << "Button 3 is pressed" << endl;
          toggleInterrogationObject();
  }

  // Store the CAVE navigation matrix in the DCS
  // If transformState is true, store it in the DCS navigate,
  // if it is false, store it in cageTransform.
  if (!transformState)
     pfCAVEDCSNavTransform(navigate);
}

//
// Interaction function for isophotes, the fast case.
//
void GeometryRoom::isophoteFastInteract(void)
{
  // get the Cave navigation
  float jx = CAVE_JOYSTICK_X, jy = CAVE_JOYSTICK_Y;
  float w[3], mult = jy*navigationSpeed;

  // if the joystick is rotated above a threshold in X, rotate
  // original in nav: z-axis!
  if (fabs(jx) > navigationThreshold)
  {
      if (transformState) {
          // rotate the lights
          direction->rotate(-5.0*jx, 0.0, 1.0, 0.0);
          direction->replaceDirection(cageGeometry);
          compute();
      }
      else
          // rotate all objects
          CAVENavRot(-5.0f*jx, 'x');
  }

  // if the joystick is pressed above a threshold in Y, translate
  // along wands front vector
  if (fabs(jy) > navigationThreshold) {
      CAVEGetVector(CAVE_WAND_FRONT, w);

      // translate all objects, for isophote translation of the
      // light changes nothing.
      if (!transformState) 
          CAVENavTranslate(w[0]*mult, w[1]*mult, w[2]*mult);
  }

  // If Button 1 is pressed build one centered lightline, vertical
  if (CAVEBUTTON1) {
          cerr << "Button 1 is pressed" << endl;
          transformState = !transformState;
  }
  // If Button 2 is pressed do the computation
  if (CAVEBUTTON2)  {
          cerr << "Button 2 is pressed" << endl;
          compute();
  }
  // If Button 3 is pressed, toggle InterrotagionObject visibility
  if (CAVEBUTTON3) {
          cerr << "Button 3 is pressed" << endl;
          toggleInterrogationObject();
  }

  // Store the CAVE navigation matrix in the DCS
  // If transformState is true, store it in the DCS navigate,
  // if it is false, store it in cageTransform.
  if (!transformState)
     pfCAVEDCSNavTransform(navigate);
}

//
// Interaction function in case of reflection lines. The eye-point has to be
// set using CAVEGetPosition and ReflectionLines::setEyePoint.
//
void GeometryRoom::reflectInteract(void)
{
  // get the Cave navigation
  float jx = CAVE_JOYSTICK_X, jy = CAVE_JOYSTICK_Y;
  float w[3], mult = jy*navigationSpeed;

  // if the joystick is rotated above a threshold in X, rotate
  // original in nav: z-axis!
  if (fabs(jx) > navigationThreshold) 
  {
      if (transformState) {
          // rotate the lights
          cage->rotate(-5.0*jx, 0.0, 0.0, 1.0);
          cage->replaceCage(cageGeometry);
      }
      else 
          // rotate all objects
          CAVENavRot(-5.0f*jx, 'x');
  }

  // if the joystick is pressed above a threshold in Y, translate
  // along wands front vector
  if (fabs(jy) > navigationThreshold) {
      CAVEGetVector(CAVE_WAND_FRONT, w);

      if (transformState) {
          // translate the lights
          // change the sign to be consistent with world coordinates!
          cage->translate(-w[0]*mult, -w[1] * mult, -w[2] * mult);
          cage->replaceCage(cageGeometry);
      }
      else 
          // translate all objects
          CAVENavTranslate(w[0]*mult, w[1]*mult, w[2]*mult);
  }

  // If Button 1 is pressed toggle the transformation behaviour
  // of light cage.
  if (CAVEBUTTON1) {
          cerr << "Button 1 is pressed" << endl;
          transformState = !transformState;
  }
  // If Button 2 is pressed do the computation
  if (CAVEBUTTON2)  {
          cerr << "Button 2 is pressed" << endl;
          CAVEGetPosition(CAVE_HEAD, w);
          hlines->setEyePoint(w);
          compute();
  }
  // If Button 3 is pressed, toggle InterrotagionObject visibility
  if (CAVEBUTTON3) {
          cerr << "Button 3 is pressed" << endl;
          toggleInterrogationObject();
  }

  // Store the CAVE navigation matrix in the DCS
  // If transformState is true, store it in the DCS navigate,
  // if it is false, store it in cageTransform.
  if (!transformState)
     pfCAVEDCSNavTransform(navigate); 
}

// Compute the InterrogationLines
void GeometryRoom::compute(void)
{
  hlines->clearLines();
  hlines->compute();
  // replace the Performer geometry to update the graphics
  //hlines->getLines(hlinesGeometry);
  hlinesGeometry->replaceChild(hlinesGeometry->getChild(0),
                                   hlines->getLines());
}

GeometryRoom::GeometryRoom(void)
{
  createMasterScene();
  IObject = new InterrogationObject;

  interState = false;
  lightState = false;
  cageState  = false;
  transformState = false;
 
  navigationSpeed     = 0.2f;
  navigationThreshold = 0.2f;
}

GeometryRoom::GeometryRoom(pfChannel *channel, char *geomFile)
{
  createMasterScene();
  IObject = new InterrogationObject(geomFile);

  objects->setVal(PFSWITCH_OFF);
  lightState = false;
  cageState = false;

  IObject->render(geometry);
  geometry->setVal(PFSWITCH_ON);
  interState = true;
  computeState = true;

  channel->setScene(scene);
 
  transformState = false;

  navigationSpeed     = 0.2f;
  navigationThreshold = 0.2f;
}

GeometryRoom::GeometryRoom(pfChannel *channel, char *geomFile, InterrogationLines *l)
{
  createMasterScene();
  IObject = new InterrogationObject(geomFile);

  hlines = l;
  hlines->setInterrogationObject(IObject);

  objects->setVal(PFSWITCH_OFF);
  lightState = false;
  cageState = false;

  IObject->render(geometry);
  geometry->setVal(PFSWITCH_ON);
  interState = true;
  computeState = true;

  channel->setScene(scene);
 
  transformState = false;

  navigationSpeed     = 0.2f;
  navigationThreshold = 0.2f;
}

// build light cage
// No computation is done!
// -----------------------

void GeometryRoom::addLightObjects(bool plane, bool vertical, int number)
{
  float BBox[6]; 
  float w[3];

  IObject->getBoundingBox(BBox);

  if (plane) cage = new TopParallelLightCage(BBox, vertical, number);
    
  // false for plane not supported, until we implement the
  // cylinder-shaped cage.

  // Get that in Performer to display
  
  // add the lines as geodes to the pfGroup cageGeometry
  cage->getCage(cageGeometry);
  lightState = true;
  objects->setVal(PFSWITCH_ON);
  cageState = true;

  // setup the interrogation lines
  hlines->setLightCage(cage);
  CAVEGetPosition(CAVE_HEAD, w);
  hlines->setEyePoint(w);

  hlines->compute();
  hlinesGeometry->addChild(hlines->getLines());
}

void GeometryRoom::addLightObjects(bool plane, int number)
{
  float BBox[6];
  float w[3];

  IObject->getBoundingBox(BBox);

  if (plane) cage = new TopCrissCrossLightCage(BBox, number);
   
  // false for plane not supported, until we implement the
  // cylinder-shaped cage.

  // Get that in Performer to display
 
  // add the lines as geodes to the pfGroup cageGeometry
  cage->getCage(cageGeometry);
  lightState = true;
  objects->setVal(PFSWITCH_ON);
  cageState = true;

  // setup the interrogation lines
  hlines->setLightCage(cage);
  CAVEGetPosition(CAVE_HEAD, w);
  hlines->setEyePoint(w);

  hlines->compute();
  hlinesGeometry->addChild(hlines->getLines());
}


void GeometryRoom::addLightObjects(bool plane, bool vertical, 
                                   int number, LightLine::Attenuation att)
{
  float BBox[6];
  float w[3];

  IObject->getBoundingBox(BBox);

  if (plane) cage = new TopParallelLightCage(BBox, vertical, number);

  // Set the lightform
  cage->setCageAttenuation(att);

  // Get that in Performer to display
  // add the lines as geodes to the pfGroup cageGeometry
  cage->getCage(cageGeometry);
  lightState = true;
  objects->setVal(PFSWITCH_ON);
  cageState = true;

  // setup the interrogation lines
  hlines->setLightCage(cage);
  CAVEGetPosition(CAVE_HEAD, w);
  hlines->setEyePoint(w);

  hlines->compute();
  hlinesGeometry->addChild(hlines->getLines());
}

// crisscrossverion
void GeometryRoom::addLightObjects(bool plane, 
                                   int number, LightLine::Attenuation att)
{
  float BBox[6];
  float w[3];

  IObject->getBoundingBox(BBox);

  if (plane) cage = new TopCrissCrossLightCage(BBox, number);

  // Set the lightform
  cage->setCageAttenuation(att);

  // Get that in Performer to display
  // add the lines as geodes to the pfGroup cageGeometry
  cage->getCage(cageGeometry);
  lightState = true;
  objects->setVal(PFSWITCH_ON);
  cageState = true;

  // setup the interrogation lines
  hlines->setLightCage(cage);
  CAVEGetPosition(CAVE_HEAD, w);
  hlines->setEyePoint(w);

  hlines->compute();
  hlinesGeometry->addChild(hlines->getLines());
}

void GeometryRoom::addLightObjects(bool plane, bool vertical, 
                                   int number, float rad,
                                   LightLine::Attenuation att)
{
  float BBox[6];
  float w[3];

  IObject->getBoundingBox(BBox);

  if (plane) cage = new TopParallelLightCage(BBox, vertical, number);

  // Set the lightform
  cage->setCageAttenuation(att);
  cage->setCageRadius(rad);

  // Get that in Performer to display
  // add the lines as geodes to the pfGroup cageGeometry
  cage->getCage(cageGeometry);
  lightState = true;
  objects->setVal(PFSWITCH_ON);
  cageState = true;

  // setup the interrogation lines
  hlines->setLightCage(cage);
  CAVEGetPosition(CAVE_HEAD, w);
  hlines->setEyePoint(w);
  // if the radius is > 0, we want to see several isolines
  if (rad > 0) {
     hlines->setNumberOfLines(5);
     hlines->setRadius(rad);
  }

  hlines->compute();
  hlinesGeometry->addChild(hlines->getLines());
}

// crisscross version
void GeometryRoom::addLightObjects(bool plane,
                                   int number, float rad,
                                   LightLine::Attenuation att)
{
  float BBox[6];
  float w[3];

  IObject->getBoundingBox(BBox);

  if (plane) cage = new TopCrissCrossLightCage(BBox, number);

  // Set the lightform
  cage->setCageAttenuation(att);
  cage->setCageRadius(rad);

  // Get that in Performer to display
  // add the lines as geodes to the pfGroup cageGeometry
  cage->getCage(cageGeometry);
  lightState = true;
  objects->setVal(PFSWITCH_ON);
  cageState = true;

  // setup the interrogation lines
  hlines->setLightCage(cage);
  CAVEGetPosition(CAVE_HEAD, w);
  hlines->setEyePoint(w);
  // if the radius is > 0, we want to see several isolines
  if (rad > 0) {
     hlines->setNumberOfLines(5);
     hlines->setRadius(rad);
  }

  hlines->compute();
  hlinesGeometry->addChild(hlines->getLines());
}

//
// The functions for isophotes, supporting a light vector
//
void GeometryRoom::addLightVector(void)
{
  float delta, BBox[6], w[3];

  IObject->getBoundingBox(BBox);

  // construct the direction for the parallel light.
  // Initially, we take a line orthogonal to the bounding box
  // of the object. We render it as a line on top of the
  // bounding box.
  direction = new LightVector(0.0f, 0.0f, 1.0f);

  delta = 0.5f*(BBox[5] - BBox[4]);
  w[0] = 0.5f*(BBox[0]+BBox[1]);
  w[1] = 0.5f*(BBox[2]+BBox[3]);
  w[2] = BBox[5] + 0.1f*delta;

  direction->setRenderOrigin(w);
  direction->setLength(delta);

  direction->render(cageGeometry);
  lightState=true;
  objects->setVal(PFSWITCH_ON);
  cageState = true;

  // setup the interrogation lines
  hlines->setLightVector(direction);

  hlines->compute();
  hlinesGeometry->addChild(hlines->getLines());
}


void GeometryRoom::addLightVector(int num)
{
  float delta, BBox[6], w[3];

  IObject->getBoundingBox(BBox);
  // construct the direction for the parallel light.
  // Initially, we take a line orthogonal to the bounding box
  // of the object. We render it as a line on top of the
  // bounding box.
  direction = new LightVector(0.0f, 0.0f, 1.0f);

  delta = 0.5f*(BBox[5] - BBox[4]);
  w[0] = 0.5f*(BBox[0]+BBox[1]);
  w[1] = 0.5f*(BBox[2]+BBox[3]);
  w[2] = BBox[5] + 0.1f*delta;

  direction->setRenderOrigin(w);
  direction->setLength(delta);

  direction->render(cageGeometry);
  lightState=true;
  objects->setVal(PFSWITCH_ON);
  cageState = true;

  // setup the interrogation lines
  hlines->setLightVector(direction);

  // if the number is > 1, we want to see several isophotes
  if (num>1) 
     hlines->setNumberOfLines(num);

  hlines->compute();
  hlinesGeometry->addChild(hlines->getLines());
}

// set the color of computed interrogation lines
// Only supported for geometric lights. Should be possible
// for textures, too.
void GeometryRoom::setLinesColor(float r, float g, float b)
{
  hlines->setColor(r,g,b);
}

void GeometryRoom::writeLight(char *performerFileName)
{
  hlines->writeLinesAsPerformerFile(performerFileName);
}

//
// private
//
void GeometryRoom::createMasterScene(void)
{
  scene          = new pfScene;
  state          = new pfGeoState;
  // root of the geometry subtree
  navigate       = new pfDCS;
  // root of the lightobjects subtree
  objects        = new pfSwitch;
  // geometry of lightcage
  cageGeometry   = new pfGroup;
  // Geometry of the computed interrogationlines
  hlinesGeometry = new pfGroup;
  // containing the interrogated object
  geometry       = new pfSwitch;
  // some light ..
  light          = new pfLightSource;

  //
  // Set the global Gstate
  //
  state->setMode(PFSTATE_ENLIGHTING, PF_ON);
  state->setMode(PFSTATE_CULLFACE, PFCF_OFF);
  state->setMode(PFSTATE_TRANSPARENCY, PFTR_OFF);
  scene->setGState(state);

  light->setColor(PFLT_DIFFUSE, 1.0, 1.0, 1.0);
  light->setAmbient(0.3, 0.3, 0.3);
  light->setPos(0.0, 0.0, 3.0, 1.0);
  // using default position 0,0,1,0
  // thats overhead, infinitely away

  // build the scene graph
  scene->addChild(light);
  scene->addChild(navigate);

  navigate->addChild(objects);

  objects->addChild(cageGeometry);

  objects->addChild(hlinesGeometry);

  navigate->addChild(geometry);
}
