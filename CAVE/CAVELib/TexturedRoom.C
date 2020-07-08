// --------------------------------------------------------------------
//  Derived from Room.C. Implementation. 
//  We use textures for rendering.
// --------------------------------------------------------------------
//  $RCSfile: TexturedRoom.C,v $
//  $Revision: 1.13 $
//  $Date: 2002/05/30 13:57:15 $
// --------------------------------------------------------------------
#include <math.h>
#include <iostream.h>
#include <pfcave.h>

#include <vtkDataSetReader.h>

#include "TexturedRoom.h"

//
// Interaction function for highlight lights. The recomputation and display of
// interrogation results has to be triggered manually.
//
void TexturedRoom::interact(void)
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
// Interaction function for isophotes. The recomputation and display of
// interrogation results has to be triggered manually.
//
void TexturedRoom::isophoteInteract(void)
{
  // get the Cave navigation
  float jx = CAVE_JOYSTICK_X, jy = CAVE_JOYSTICK_Y;
  float w[3], mult = jy*navigationSpeed;

  // if the joystick is rotated above a threshold in X, rotate
  // original in nav: z-axis!
  if (fabs(jx) > navigationThreshold)
  {
      if (transformState) {
          // rotate the light vector
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

      // translate all objects. Translation of the light vector
      // does not change the interrogation results.
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

//
// Fast redisplay of interrogation information for highlight
// lines. It has not to be triggerd
// by a button press. Only feasible, if the object is not too big!
//
void TexturedRoom::fastInteract()
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
// Fast redisplay of interrogation information for isophotes.
// It has not to be triggerd
// by a button press. Only feasible, if the object is not too big!
//
void TexturedRoom::isophoteFastInteract()
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

      // Translating the light vector doesn not change the interrogation results,
      // so we ignore it.
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


// Interaction function in case of reflection lines. The eye-point has to be
// set using CAVEGetPosition and ReflectionLines::setEyePoint.
// In opposite to the GeometryRoom version, reflectInteract is interactive,
// we don't have to trigger the re-display.
//
void TexturedRoom::reflectInteract(void)
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
          // change the sign to be consistent with world coordinates!
          cage->translate(-w[0]*mult, -w[1] * mult, -w[2] * mult);
          cage->replaceCage(cageGeometry);
          compute();
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
  // If Button 3 is pressed, toggle InterrogationObject visibility
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
void TexturedRoom::compute(void)
{
  // replace the Performer geometry to update the graphics
  // Compute the texture objects for the rendering of the interrogated
  // object in Performer. The interrogationLines know how to do this.
  hlines->computeTextureCoordinates();
  IObject->replaceTexture(hlines->computeTexture(bitmapSize));
  IObject->replaceObject(geometry);
}

TexturedRoom::TexturedRoom(void)
{
  createMasterScene();
  IObject = new InterrogationObject;
  IObject->setTextureState(true);

  interState = false;
  lightState = false;
  cageState  = false;
  transformState = false;
 
  navigationSpeed     = 0.2f;
  navigationThreshold = 0.2f;

  bitmapSize = 256;
}

TexturedRoom::TexturedRoom(pfChannel *channel, char *geomFile)
{
  // The object isn't rendered, because we have no textures at that moment.
  // Rendering of the IObject is done if we have build a light cage, so we
  // can compute the texture map and the Performer texture objects.
  createMasterScene();
  IObject = new InterrogationObject(geomFile, true);

  objects->setVal(PFSWITCH_OFF);
  lightState = false;
  cageState = false;

  geometry->setVal(PFSWITCH_OFF);
  interState = true;
  computeState = true;

  channel->setScene(scene);
 
  transformState = false;

  navigationSpeed     = 0.2f;
  navigationThreshold = 0.2f;

  bitmapSize = 256;
}

TexturedRoom::TexturedRoom(pfChannel *channel, char *geomFile, InterrogationLines *l)
{
  // The object isn't rendered, because we have no textures at that moment.
  // Rendering of the IObject is done if we have build a light cage, so we
  // can compute the texture map and the Performer texture objects.
  createMasterScene();
  IObject = new InterrogationObject(geomFile, true);

  hlines = l;
  hlines->setInterrogationObject(IObject);

  objects->setVal(PFSWITCH_OFF);
  lightState = false;
  cageState = false;

  geometry->setVal(PFSWITCH_OFF);
  interState = true;
  computeState = true;

  channel->setScene(scene);
 
  transformState = false;

  navigationSpeed     = 0.2f;
  navigationThreshold = 0.2f;

  bitmapSize = 256;
}

TexturedRoom::TexturedRoom(pfChannel *channel, char *geomFile, int size)
{
  // The object isn't rendered, because we have no textures at that moment.
  // Rendering of the IObject is done if we have build a light cage, so we
  // can compute the texture map and the Performer texture objects.
  createMasterScene();
  IObject = new InterrogationObject(geomFile, true);

  objects->setVal(PFSWITCH_OFF);
  lightState = false;
  cageState = false;

  geometry->setVal(PFSWITCH_OFF);
  interState = true;
  computeState = true;

  channel->setScene(scene);
 
  transformState = false;

  navigationSpeed     = 0.2f;
  navigationThreshold = 0.2f;

  bitmapSize = size;
}

TexturedRoom::TexturedRoom(pfChannel *channel, char *geomFile, InterrogationLines *l, int size)
{
  // The object isn't rendered, because we have no textures at that moment.
  // Rendering of the IObject is done if we have build a light cage, so we
  // can compute the texture map and the Performer texture objects.
  createMasterScene();
  IObject = new InterrogationObject(geomFile, true);

  hlines = l;
  hlines->setInterrogationObject(IObject);
  hlines->setLightCage(cage);

  objects->setVal(PFSWITCH_OFF);
  lightState = false;
  cageState = false;

  geometry->setVal(PFSWITCH_OFF);
  interState = true;
  computeState = true;

  channel->setScene(scene);
 
  transformState = false;

  navigationSpeed     = 0.2f;
  navigationThreshold = 0.2f;

  bitmapSize = size;
}

// ------------------------------------------------------------
// build light cage
// ------------------------------------------------------------

void TexturedRoom::addLightObjects(bool plane, bool vertical, int number)
{
  float BBox[6];
  float w[3];

  IObject->getBoundingBox(BBox);

  if (plane) cage = new TopParallelLightCage(BBox, vertical, number);

  // Get that in Performer to display
  cage->getCage(cageGeometry);
  lightState = true;
  objects->setVal(PFSWITCH_ON);
  cageState = true;

  // setup the interrogation lines
  hlines->setLightCage(cage);
  CAVEGetPosition(CAVE_HEAD, w);
  hlines->setEyePoint(w);

  // Compute the texture objects for the rendering of the interrogated
  // object in Performer. The interrogationLines know how to do this.
  hlines->computeTextureCoordinates();
  IObject->setTextureObjects(hlines->computeTexture(bitmapSize), 
                             hlines->buildTexEnv(), 
                             hlines->buildTexGen());
  IObject->render(geometry);
  geometry->setVal(PFSWITCH_ON);
}


// crisscross version
void TexturedRoom::addLightObjects(bool plane, int number)
{
  float BBox[6];
  float w[3];

  IObject->getBoundingBox(BBox);

  if (plane) cage = new TopCrissCrossLightCage(BBox, number);

  // Get that in Performer to display
  cage->getCage(cageGeometry);
  lightState = true;
  objects->setVal(PFSWITCH_ON);
  cageState = true;

  // setup the interrogation lines
  hlines->setLightCage(cage);
  CAVEGetPosition(CAVE_HEAD, w);
  hlines->setEyePoint(w);

  // Compute the texture objects for the rendering of the interrogated
  // object in Performer. The interrogationLines know how to do this.
  hlines->computeTextureCoordinates();
  IObject->setTextureObjects(hlines->computeTexture(bitmapSize),
                             hlines->buildTexEnv(),
                             hlines->buildTexGen());
  IObject->render(geometry);
  geometry->setVal(PFSWITCH_ON);
}

void TexturedRoom::addLightObjects(bool plane, bool vertical, 
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

  // Compute the texture objects for the rendering of the interrogated
  // object in Performer. The interrogationLines know how to do this.
  hlines->computeTextureCoordinates();
  IObject->setTextureObjects(hlines->computeTexture(bitmapSize), 
                             hlines->buildTexEnv(), 
                             hlines->buildTexGen());
  IObject->render(geometry);
  geometry->setVal(PFSWITCH_ON);
}

// crisscross verions
void TexturedRoom::addLightObjects(bool plane,
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

  // Compute the texture objects for the rendering of the interrogated
  // object in Performer. The interrogationLines know how to do this.
  hlines->computeTextureCoordinates();
  IObject->setTextureObjects(hlines->computeTexture(bitmapSize),
                             hlines->buildTexEnv(),
                             hlines->buildTexGen());
  IObject->render(geometry);
  geometry->setVal(PFSWITCH_ON);
}

void TexturedRoom::addLightObjects(bool plane, bool vertical, 
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

  // Compute the texture objects for the rendering of the interrogated
  // object in Performer. The interrogationLines know how to do this.
  hlines->computeTextureCoordinates();
  IObject->setTextureObjects(hlines->computeTexture(bitmapSize), 
                             hlines->buildTexEnv(), 
                             hlines->buildTexGen());
  IObject->render(geometry);
  geometry->setVal(PFSWITCH_ON);
}

// crisscross version
void TexturedRoom::addLightObjects(bool plane,
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

  // Compute the texture objects for the rendering of the interrogated
  // object in Performer. The interrogationLines know how to do this.
  hlines->computeTextureCoordinates();
  IObject->setTextureObjects(hlines->computeTexture(bitmapSize),
                             hlines->buildTexEnv(),
                             hlines->buildTexGen());
  IObject->render(geometry);
  geometry->setVal(PFSWITCH_ON);
}

//
// The functions for isophotes, supporting a light vector
//
void TexturedRoom::addLightVector(void)
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

  // Compute the texture objects for the rendering
  hlines->computeTextureCoordinates();
  IObject->setTextureObjects(hlines->computeTexture(bitmapSize),
                            hlines->buildTexEnv(),
                            hlines->buildTexGen());


  IObject->render(geometry);
  geometry->setVal(PFSWITCH_ON);
}

void TexturedRoom::addLightVector(int num)
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
  if (num>1) {
    hlines->setNumberOfLines(num);
    hlines->setRadius(1.0f); // dummy
  }

  // Compute the texture objects for the rendering
  hlines->computeTextureCoordinates();
  IObject->setTextureObjects(hlines->computeTexture(bitmapSize),
                            hlines->buildTexEnv(),
                            hlines->buildTexGen());


  IObject->render(geometry);
  geometry->setVal(PFSWITCH_ON);
}

//
// private
//
void TexturedRoom::createMasterScene(void)
{
  // Build the private scene data
  scene          = new pfScene;
  state          = new pfGeoState;
  // root of the geometry subtree
  navigate       = new pfDCS;
  // root of the lightobjects subtree
  objects        = new pfSwitch;
  // geometry of lightcage
  cageGeometry   = new pfGroup;
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
  // using default position 0,0,1,0
  // thats overhead, infinitely away

  // build the scene graph
  scene->addChild(light);
  scene->addChild(navigate);

  navigate->addChild(objects);

  objects->addChild(cageGeometry);

  navigate->addChild(geometry);
}
