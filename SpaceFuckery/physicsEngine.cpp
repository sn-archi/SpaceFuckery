/*************************************************
*                                                *
* Project: SpaceFuckery                          *
*                                                *
*   Copyright 2015 - 2016 - Marc-Olivier Barre   *
*              All rights reserved               *
*                                                *
**************************************************/

#include "physicsEngine.h"
#include "Application.h"

namespace SpaceFuckery
{
  // physicsEngine is in fact a custom made dynamicsWorld with spherical gravity
  // that works with Newtons equation
  physicsEngine::physicsEngine (btDispatcher* dispatcher, btBroadphaseInterface* pairCache, btConstraintSolver* solver, btCollisionConfiguration* collisionConfiguration)
    : btDiscreteDynamicsWorld (dispatcher, pairCache, solver, collisionConfiguration),
      mCollisionObjectCount (0)
  {
    // Make sure the default gravity isn't there. We'll be making our own !
    // TODO: Do we need this now that I inherit from btDiscreteDynamicsWorld to override the applyGravity method ?
    btVector3 nullGravity = btVector3 (0, 0, 0);
    this->setGravity (nullGravity);
  }

  physicsEngine::~physicsEngine()
  {
    // TODO: Do I need to do something here ? fire up the btDiscreteDynamicsWorld destructor ?
  }

  /** This is a helper function that gives us the number of objects with defined to iterate over them
      It kind of sucks */
  int physicsEngine::getCollisionObjectCount (void)
  {
    return mCollisionObjectCount;
  }

  /** Run this when you create an object that needs to have gravity to keep count
      TODO: This is really bad code that will lead to funky mistakes. Fix it. */
  void physicsEngine::setCollisionObjectCount (void)
  {
    mCollisionObjectCount++;
  }
}
