#ifndef WINDOWEVENTLISTENER_H_INCLUDED
#define WINDOWEVENTLISTENER_H_INCLUDED

/******************************************
*                                         *
* Project: SpaceFuckery                   *
* Interface: WindowEventListener          *
*                                         *
*   Copyright 2015 - Marc-Olivier Barre   *
*           All rights reserved           *
*                                         *
******************************************/

#include <OgreWindowEventUtilities.h>
#include "Application.h"

namespace SpaceFuckery
{
  class WindowEventListener:
    public Ogre::WindowEventListener
  {
    public:
      WindowEventListener();
      virtual void windowResized (Ogre::RenderWindow* rw);
      virtual void windowClosed (Ogre::RenderWindow* rw);
  };
}

#endif // WINDOWEVENTLISTENER_H_INCLUDED
