#ifndef PHYSICS_H_INCLUDED
#define PHYSICS_H_INCLUDED

/******************************************
*                                         *
* Project: SpaceFuckery                   *
* Function:                               *
*                                         *
*   Copyright 2015 - Marc-Olivier Barre   *
*           All rights reserved           *
*                                         *
******************************************/

#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include <vector>
#include <map>
#include <string>

namespace SpaceFuckery
{
  class physicsEngine{
  private:
      btDefaultCollisionConfiguration* collisionConfiguration;
      btCollisionDispatcher* dispatcher;
      btBroadphaseInterface* overlappingPairCache;
      btSequentialImpulseConstraintSolver* solver;
      btDiscreteDynamicsWorld* dynamicsWorld;
      int mCollisionObjectCount;
    public:
      physicsEngine();
      virtual ~physicsEngine();
      virtual btDiscreteDynamicsWorld* getDynamicsWorld(void);
      virtual int getCollisionObjectCount(void);
      virtual void countPhysicsObject(void);
  };
}

#endif // PHYSICS_H_INCLUDED
