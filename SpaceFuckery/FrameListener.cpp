/*************************************************
*                                                *
* Project: SpaceFuckery                          *
*                                                *
*   Copyright 2015 - 2016 - Marc-Olivier Barre   *
*              All rights reserved               *
*                                                *
**************************************************/

#include <CEGUI/CEGUI.h>
#include <mKOST.h>
#include <cmath>
#include <string>

#include "Application.h"
#include "FrameListener.h"
#include "Constants.h"

namespace SpaceFuckery
{
  FrameListener::FrameListener() :
    mTimer (0),
    lastFrameTime (0),
    nowTime (0)
  {
    mTimer = new Ogre::Timer;
    lastFrameTime = mTimer->getMicroseconds();
    mOrbit = new mKOST::Orbit;
    mOrbit->setMu(MU);
  }

  /** Calculate the gravity force to apply. This is so wrong */
  btVector3 FrameListener::calcForce (const btRigidBody* ship)
  {
    btScalar shipMass (1.0);
    btVector3 currentPos (ship->getCenterOfMassPosition());
    btVector3 earthPos (0.0, 0.0, 0.0);
    btVector3 totalForce (0.0, 0.0, 0.0);
    btScalar Fg ((earthMu * shipMass) / currentPos.distance2 (earthPos));
    totalForce += Fg * -(currentPos - earthPos).normalized();
    CEGUI::Window* flightWin (CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow());
    CEGUI::Window* altitudeText (flightWin->getChild ("Altitude"));
    altitudeText->setText (std::to_string (mOrbit->getParams().PeD));
    return totalForce;
  }

  btVector3 FrameListener::updatePos (const btRigidBody* ship)
  {
    //mKOST::StateVectors sv (mOrbit->elements2StateVectorAtTime(MeL, timeSinceEpoch, EPSILON, 1000000));
    //return sv.pos;
    return btVector3 (0.0, 0.0, 0.0);
  }

  bool FrameListener::frameRenderingQueued (const Ogre::FrameEvent &evt)
  {
    if (Application::getSingleton().getWindow()->isClosed())
      return false;

    if (Application::getSingleton().getShutDown())
      return false;

    // We need to capture/update each device
    Application::getSingleton().getKeyboard()->capture();
    Application::getSingleton().getMouse()->capture();

    return true;
  }

  bool FrameListener::frameStarted (const Ogre::FrameEvent &evt)
  {
    nowTime = mTimer->getMicroseconds();
    btScalar lastFrameLength ((nowTime - lastFrameTime) * 1.0e-6);

    if (Application::getSingleton().getPhysicsEngine() != NULL)
      {
        for (int i (0); i < Application::getSingleton().getPhysicsEngine()->getCollisionObjectCount(); ++i)
          {
            btCollisionObject* obj (Application::getSingleton().getPhysicsEngine()->getCollisionObjectArray() [i]);
            btRigidBody* body (btRigidBody::upcast (obj));

            btVector3 currentForce (calcForce (body));
            body->applyCentralForce (currentForce);
            mKOST::StateVectors sv;
            sv.pos = body->getCenterOfMassPosition();
            sv.vel = body->getLinearVelocity();
            mOrbit->refreshFromStateVectors(&sv);
          }

        // Whatever happens, step the simulation
        Application::getSingleton().getPhysicsEngine()->stepSimulation (lastFrameLength, 10, 1.f / 240.f);

        for (int i (0); i < Application::getSingleton().getPhysicsEngine()->getCollisionObjectCount(); ++i)
          {
            btCollisionObject* obj (Application::getSingleton().getPhysicsEngine()->getCollisionObjectArray() [i]);
            btRigidBody* body (btRigidBody::upcast (obj));

            if (body && body->getMotionState())
              {
                btTransform trans;
                body->getMotionState()->getWorldTransform (trans);

                void* userPointer (body->getUserPointer());

                if (userPointer)
                  {
                    btQuaternion orientation = trans.getRotation();
                    Ogre::SceneNode* sceneNode = static_cast<Ogre::SceneNode*> (userPointer);
                    sceneNode->setPosition (Ogre::Vector3 (trans.getOrigin().getX(), trans.getOrigin().getY(), trans.getOrigin().getZ()));
                    sceneNode->setOrientation (Ogre::Quaternion (orientation.getW(), orientation.getX(), orientation.getY(), orientation.getZ()));
                  }
              }
          }
      }

    lastFrameTime = mTimer->getMicroseconds();
    return true;
  }

  bool FrameListener::frameEnded (const Ogre::FrameEvent &evt)
  {
    return true;
  }
}
