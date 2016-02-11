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

  bool FrameListener::frameStarted (const Ogre::FrameEvent& evt)
  {
    if (Application::getSingleton().getPhysicsEngine() != NULL){
    Application::getSingleton().getPhysicsEngine()->getDynamicsWorld()->stepSimulation(1.0f/60.0f); //suppose you have 60 frames per second

      for (int i = 0; i< Application::getSingleton().getPhysicsEngine()->getCollisionObjectCount(); i++) {
        btCollisionObject* obj = Application::getSingleton().getPhysicsEngine()->getDynamicsWorld()->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);

        if (body && body->getMotionState()){
          btTransform trans;
          body->getMotionState()->getWorldTransform(trans);

          void *userPointer = body->getUserPointer();
          if (userPointer) {
            btQuaternion orientation = trans.getRotation();
            Ogre::SceneNode *sceneNode = static_cast<Ogre::SceneNode *>(userPointer);
            sceneNode->setPosition(Ogre::Vector3(trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
            sceneNode->setOrientation(Ogre::Quaternion(orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));
          }
        }
      }
    }
    return true;
  }

  bool FrameListener::frameEnded (const Ogre::FrameEvent& evt)
  {
    return true;
  }
}
