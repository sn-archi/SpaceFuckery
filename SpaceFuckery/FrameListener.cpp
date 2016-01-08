/******************************************
*                                         *
* Project: SpaceFuckery                   *
* Interface: FrameListener                *
*                                         *
*   Copyright 2015 - Marc-Olivier Barre   *
*           All rights reserved           *
*                                         *
******************************************/

#include "Application.h"
#include "FrameListener.h"

namespace SpaceFuckery
{
  bool FrameListener::frameRenderingQueued (const Ogre::FrameEvent& evt)
  {
    if (Application::getSingleton().getWindow()->isClosed() )
      return false;

    if (Application::getSingleton().getShutDown() )
      return false;

    //Need to capture/update each device
    Application::getSingleton().getKeyboard()->capture();
    Application::getSingleton().getMouse()->capture();

    return true;
  }
}
