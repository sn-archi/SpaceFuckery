/******************************************
*                                         *
* Project: SpaceFuckery                   *
* Function:                               *
*                                         *
*   Copyright 2015 - Marc-Olivier Barre   *
*           All rights reserved           *
*                                         *
******************************************/

#include "frameListener.h"
#include "Application.h"

namespace SpaceFuckery
{
    bool FrameListener::frameRenderingQueued(const Ogre::FrameEvent& evt)
  {
    SpaceFuckery::Application* Application;
    Ogre::RenderWindow* mWindow = Application->getSingleton().getWindow();
    bool mShutDown = Application->getSingleton().getShutDown();
    OIS::Mouse* mMouse = Application->getSingleton().getMouse();
    OIS::Keyboard* mKeyboard = Application->getSingleton().getKeyboard();

    if (mWindow->isClosed() )
      return false;

    if (mShutDown)
      return false;

    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();

    return true;
  }
}