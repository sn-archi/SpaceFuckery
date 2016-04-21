#ifndef WINDOWEVENTLISTENER_H_INCLUDED
#define WINDOWEVENTLISTENER_H_INCLUDED

/*************************************************
*                                                *
* Project: SpaceFuckery                          *
*                                                *
*   Copyright 2015 - 2016 - Marc-Olivier Barre   *
*              All rights reserved               *
*                                                *
**************************************************/

#include "OgreWindowEventUtilities.h"

namespace SpaceFuckery
{
  class WindowEventListener:
    public Ogre::WindowEventListener
  {
    public:
      virtual void windowResized (Ogre::RenderWindow* rw);
      virtual void windowClosed (Ogre::RenderWindow* rw);
  };
}

#endif // WINDOWEVENTLISTENER_H_INCLUDED
