/******************************************
*                                         *
* Project: SpaceFuckery                   *
* Function:                               *
*                                         *
*   Copyright 2015 - Marc-Olivier Barre   *
*           All rights reserved           *
*                                         *
******************************************/

#include "physicsEngine.h"
#include "Application.h"

namespace SpaceFuckery
{
  physicsEngine::physicsEngine(btDispatcher* dispatcher,btBroadphaseInterface* pairCache,btConstraintSolver* solver,btCollisionConfiguration* collisionConfiguration)
    :btDiscreteDynamicsWorld(dispatcher, pairCache, solver, collisionConfiguration),
    mCollisionObjectCount (0)
  {
    btVector3 nullGravity = btVector3(0,0,0);
    this->setGravity(nullGravity);
  }

  physicsEngine::~physicsEngine()
  {
  }

  int physicsEngine::getCollisionObjectCount(void)
  {
    return mCollisionObjectCount;
  }

  void physicsEngine::setCollisionObjectCount(void)
  {
    mCollisionObjectCount++;
  }
}