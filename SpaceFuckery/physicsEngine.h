#ifndef PHYSICS_H_INCLUDED
#define PHYSICS_H_INCLUDED

/*************************************************
*                                                *
* Project: SpaceFuckery                          *
*                                                *
*   Copyright 2015 - 2016 - Marc-Olivier Barre   *
*              All rights reserved               *
*                                                *
**************************************************/

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>

#include <vector>
#include <map>
#include <string>

namespace SpaceFuckery
{
  class physicsEngine:
    public btDiscreteDynamicsWorld
  {
    private:
      int mCollisionObjectCount;
    public:
      physicsEngine (btDispatcher* dispatcher, btBroadphaseInterface* pairCache, btConstraintSolver* solver, btCollisionConfiguration* collisionConfiguration);
      virtual ~physicsEngine();
      virtual int getCollisionObjectCount (void);
      virtual void setCollisionObjectCount (void);
  };
}

#endif // PHYSICS_H_INCLUDED
