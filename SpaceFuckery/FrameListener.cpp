/*************************************************
*                                                *
* Project: SpaceFuckery                          *
*                                                *
*   Copyright 2015 - 2016 - Marc-Olivier Barre   *
*              All rights reserved               *
*                                                *
**************************************************/

#include "Application.h"
#include "FrameListener.h"
#include "Orbit.h"
#include "Constants.h"
#include "CEGUI/CEGUI.h"
#include <cmath>
#include <string>

namespace SpaceFuckery
{
  FrameListener::FrameListener() :
    mTimer (0),
    lastFrameTime (0),
    nowTime (0)
  {
    mTimer = new Ogre::Timer;
    lastFrameTime = mTimer->getMicroseconds();
  }

  btVector3 FrameListener::calcForce (const btRigidBody* ship)
  {
    btScalar shipMass = 1.;
    btVector3 currentPos = ship->getCenterOfMassPosition();
    btVector3 earthPos = btVector3 (0., 0., 0.);
    btVector3 totalForce = btVector3 (0., 0., 0.);
    btVector3 localDistVect = currentPos - earthPos;
    localDistVect.normalize();
    btScalar Fg = (Mu * shipMass) / currentPos.distance2 (earthPos);
    totalForce += Fg * -localDistVect;
    CEGUI::Window* flightWin = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
    CEGUI::Window* altitudeText = flightWin->getChild ("Altitude");
    altitudeText->setText (std::to_string (currentPos.distance (earthPos)));
    return totalForce;
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
    btScalar lastFrameLength = (nowTime - lastFrameTime) * 1.0e-6;

    if (Application::getSingleton().getPhysicsEngine() != NULL)
      {
        for (int i = 0; i < Application::getSingleton().getPhysicsEngine()->getCollisionObjectCount(); i++)
          {
            btCollisionObject* obj = Application::getSingleton().getPhysicsEngine()->getCollisionObjectArray() [i];
            btRigidBody* body = btRigidBody::upcast (obj);

            Orbit suzzyOrbit = Orbit (body->getCenterOfMassPosition(), body->getLinearVelocity(), btVector3 (0, 0, 0), 1);
            std::cout << std::setprecision (5);
            std::cout << suzzyOrbit << std::endl;
//        suzzyOrbit.printVector();

            btVector3 currentForce = calcForce (body);
//        std::cout << currentForce.getX() << ", " << currentForce.getY() << ", " << currentForce.getZ() << std::endl;

            body->applyCentralForce (currentForce);
          }

        Application::getSingleton().getPhysicsEngine()->stepSimulation (lastFrameLength, 10, 1.f / 240.f);

        for (int i = 0; i < Application::getSingleton().getPhysicsEngine()->getCollisionObjectCount(); i++)
          {
            btCollisionObject* obj = Application::getSingleton().getPhysicsEngine()->getCollisionObjectArray() [i];
            btRigidBody* body = btRigidBody::upcast (obj);

            if (body && body->getMotionState())
              {
                btTransform trans;
                body->getMotionState()->getWorldTransform (trans);

                void* userPointer = body->getUserPointer();

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
    //std::cout << 1/lastFrameLength << std::endl;
    return true;
  }

  bool FrameListener::frameEnded (const Ogre::FrameEvent &evt)
  {
    return true;
  }
}
