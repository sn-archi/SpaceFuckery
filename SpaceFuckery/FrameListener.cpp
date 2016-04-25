/*************************************************
*                                                *
* Project: SpaceFuckery                          *
*                                                *
*   Copyright 2015 - 2016 - Marc-Olivier Barre   *
*              All rights reserved               *
*                                                *
**************************************************/

#include "CEGUI/CEGUI.h"
#include "Application.h"
#include "FrameListener.h"
#include "OgreVector3.h"
#include "Orbit.h"
#include <cmath>
#include <string>

const btScalar earthMass = 5.97237e24;
const btScalar G = 6.6725985e-11;
const btScalar Mu = G*earthMass;

namespace SpaceFuckery {
  const btVector3 calcForce (const btRigidBody* ship) {
    double shipMass = 1.;
    btVector3 currentPos = ship->getCenterOfMassPosition();
    Ogre::Vector3 ogreCurrentPos = Ogre::Vector3 (currentPos.getX(), currentPos.getY(), currentPos.getZ() );
    Ogre::Vector3 earthPos = Ogre::Vector3::ZERO;
    Ogre::Vector3 totalForce = Ogre::Vector3::ZERO;
    Ogre::Vector3 localDistVect = earthPos - ogreCurrentPos;
    localDistVect.normalise();
    double squaredDist = ogreCurrentPos.squaredDistance (earthPos);
    double Fg = G * earthMass * shipMass / squaredDist;
    totalForce += Fg * localDistVect;
    btVector3 result = btVector3 (totalForce.x, totalForce.y, totalForce.z);
    CEGUI::Window* flightWin = CEGUI::System::getSingleton().getDefaultGUIContext().getRootWindow();
    CEGUI::Window* altitudeText = flightWin->getChild("Altitude");
    altitudeText->setText(std::to_string(std::sqrt(squaredDist)));
    Orbit suzzyOrbit = Orbit(ship->getCenterOfMassPosition(),ship->getLinearVelocity(), btVector3(0,0,0),1);
    std::cout << suzzyOrbit << std::endl;
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
        body->applyCentralForce(calcForce(body));

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
