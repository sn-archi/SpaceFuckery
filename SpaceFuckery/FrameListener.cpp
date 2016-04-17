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
#include "OgreVector3.h"

namespace SpaceFuckery {
  const btVector3 calcForce (const btVector3 currentPos) {
    double G = 1.0;
    double earthMass = 10.0;
    double shipMass = 1.0;
    Ogre::Vector3 ogreCurrentPos = Ogre::Vector3 (currentPos.getX(), currentPos.getY(), currentPos.getZ() );
    Ogre::Vector3 earthPos = Ogre::Vector3 (0.0, 0.0, 0.0);
    Ogre::Vector3 totalForce = Ogre::Vector3 (0.0, 0.0, 0.0);
    Ogre::Vector3 localDistVect = ogreCurrentPos - earthPos;
    localDistVect.normalise();
    double squaredDist = ogreCurrentPos.squaredDistance (earthPos);
    double Fg = G * earthMass * shipMass / squaredDist;
    totalForce += Fg * localDistVect;
    btVector3 result = btVector3 (totalForce.x, totalForce.y, totalForce.z);
    return result;
    }

  bool FrameListener::frameRenderingQueued (const Ogre::FrameEvent& evt) {
    if (Application::getSingleton().getWindow()->isClosed() )
      return false;

    if (Application::getSingleton().getShutDown() )
      return false;

    // We need to capture/update each device
    Application::getSingleton().getKeyboard()->capture();
    Application::getSingleton().getMouse()->capture();

    return true;
    }

  bool FrameListener::frameStarted (const Ogre::FrameEvent& evt) {
    if (Application::getSingleton().getPhysicsEngine() != NULL) {
      Application::getSingleton().getPhysicsEngine()->stepSimulation (1.0f / 60.0f); //suppose you have 60 frames per second

      for (int i = 0; i < Application::getSingleton().getPhysicsEngine()->getCollisionObjectCount(); i++) {
        btCollisionObject* obj = Application::getSingleton().getPhysicsEngine()->getCollisionObjectArray() [i];
        btRigidBody* body = btRigidBody::upcast (obj);
//        btVector3 currentPos = body->getCenterOfMassPosition();
//        body->applyCentralForce(calcForce(currentPos));

        if (body && body->getMotionState() ) {
          btTransform trans;
          body->getMotionState()->getWorldTransform (trans);

          void *userPointer = body->getUserPointer();
          if (userPointer) {
            btQuaternion orientation = trans.getRotation();
            Ogre::SceneNode *sceneNode = static_cast<Ogre::SceneNode *> (userPointer);
            sceneNode->setPosition (Ogre::Vector3 (trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ() ) );
            sceneNode->setOrientation (Ogre::Quaternion (orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ() ) );
            }
          }
        }
      }
    return true;
    }

  bool FrameListener::frameEnded (const Ogre::FrameEvent& evt) {
    return true;
    }
  }
