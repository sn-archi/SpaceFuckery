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
  physicsEngine::physicsEngine()
  {
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    overlappingPairCache = new btDbvtBroadphase();
    solver = new btSequentialImpulseConstraintSolver();
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
    mCollisionObjectCount = 0;
  }

  physicsEngine::~physicsEngine()
  {
    delete dynamicsWorld;
  }

  btDiscreteDynamicsWorld* physicsEngine::getDynamicsWorld(void)
  {
    return dynamicsWorld;
  }

  int physicsEngine::getCollisionObjectCount(void)
  {
    return mCollisionObjectCount;
  }

  void physicsEngine::linkPhysicsToNode(std::string nodeName, btRigidBody* rigidBody)
  {
    mCollisionObjectCount++;
  }
}