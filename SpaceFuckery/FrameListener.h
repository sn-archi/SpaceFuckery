#ifndef FRAMELISTENER_H_INCLUDED
#define FRAMELISTENER_H_INCLUDED

/*************************************************
*                                                *
* Project: SpaceFuckery                          *
*                                                *
*   Copyright 2015 - 2016 - Marc-Olivier Barre   *
*              All rights reserved               *
*                                                *
**************************************************/

#include "btBulletDynamicsCommon.h"
#include "OgreFrameListener.h"

namespace SpaceFuckery
{
  class FrameListener:
    public Ogre::FrameListener
  {
    public:
      FrameListener();
    private:
      virtual bool frameRenderingQueued (const Ogre::FrameEvent& evt);
      virtual bool frameStarted (const Ogre::FrameEvent& evt);
      virtual bool frameEnded (const Ogre::FrameEvent& evt);
      btVector3 calcForce (const btRigidBody* ship);
      Ogre::Timer* mTimer;
      unsigned long lastFrameTime;
      unsigned long nowTime;
  };
}

#endif // FRAMELISTENER_H_INCLUDED
