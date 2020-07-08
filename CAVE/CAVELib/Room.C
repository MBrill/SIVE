// --------------------------------------------------------------------
//  Implementation file of virtual base class Room.
// --------------------------------------------------------------------
//  $RCSfile: Room.C,v $
//  $Revision: 2.1 $
//  $Date: 2002/02/12 20:01:37 $
// --------------------------------------------------------------------
#include "Room.h"

#include <math.h>
#include <iostream.h>
#include <pfcave.h>

#include <vtkDataSetReader.h>

//
// Set the pointer hlines to an instance of a subclass of
// InterrogationLines!
//
void Room::setInterrogationLines(InterrogationLines *l)
{
  hlines = l;
  hlines->setInterrogationObject(IObject);
  hlines->setLightCage(cage);

}

void Room::toggleInterrogationObject(void)
{
  interState = !interState;
  if (interState) 
       geometry->setVal(PFSWITCH_ON);
  else
       geometry->setVal(PFSWITCH_OFF);
}

bool Room::getInterrogationObjectVisibility(void)
{
  return interState;
}

void Room::attachChannel(pfChannel *channel)
{
  channel->setScene(scene);
}

pfScene* Room::getShowRoom(void)
{
  return scene;
}

void Room::setNavigationSpeed(float speed)
{
  navigationSpeed = speed;
}

void Room::setNavigationThreshold(float t)
{
  navigationThreshold = t;
}

pfDCS* Room::getNavigationInfo(void)
{
  return navigate;
}

void Room::getNavigationInfo(pfDCS *nav)
{
  nav = navigate;
}

void Room::toggleLight(void)
{
  lightState = !lightState;
  if (lightState) 
       objects->setVal(PFSWITCH_ON);
  else
       objects->setVal(PFSWITCH_OFF);
}

void Room::toggleCage(void)
{
  cageState = !cageState;
}

bool Room::getLightVisibility(void)
{
  return lightState;
}

bool Room::getComputation(void)
{
  return computeState;
}

void Room::toggleComputation(void)
{
  computeState = !computeState;
}

void Room::setCageRadius(float r)
{
  cage->setRadius(r);
}
