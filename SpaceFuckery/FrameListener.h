#ifndef FRAMELISTENER_H_INCLUDED
#define FRAMELISTENER_H_INCLUDED

/******************************************
*                                         *
* Project: SpaceFuckery                   *
* Interface: FrameListener                *
*                                         *
*   Copyright 2015 - Marc-Olivier Barre   *
*           All rights reserved           *
*                                         *
******************************************/

#include "OgreFrameListener.h"

namespace SpaceFuckery
{
  class FrameListener:
    public Ogre::FrameListener
  {
    private:
      virtual bool frameRenderingQueued (const Ogre::FrameEvent& evt);
      virtual bool frameStarted (const Ogre::FrameEvent& evt);
      virtual bool frameEnded (const Ogre::FrameEvent& evt);
  };
}

#endif // FRAMELISTENER_H_INCLUDED
