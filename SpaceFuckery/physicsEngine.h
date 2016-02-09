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
#include "stddef.h"
#include "vector"
#include "map"
#include "string"

namespace SpaceFuckery
{
  class physicsEngine{
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btBroadphaseInterface* overlappingPairCache;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;
    std::vector<btCollisionShape *> collisionShapes;
    std::map<std::string, btRigidBody *> physicsAccessors;
    int mCollisionObjectCount;
    public:
      physicsEngine();
      btDiscreteDynamicsWorld* getDynamicsWorld(void);
      int getCollisionObjectCount(void);
  };
}

#endif // PHYSICS_H_INCLUDED
