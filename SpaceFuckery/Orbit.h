#ifndef ORBIT_H
#define ORBIT_H

/*************************************************
*                                                *
* Project: SpaceFuckery                          *
*                                                *
*   Copyright 2015 - 2016 - Marc-Olivier Barre   *
*              All rights reserved               *
*                                                *
**************************************************/

#include "LinearMath/btVector3.h"
#include <string>

namespace SpaceFuckery
{
  class Orbit
  {
    public:
      /** State vector contrusctor */
      Orbit (btVector3 Position, btVector3 Velocity, btVector3 centralBodyPos, btScalar mass);
      /** Keplerian contrusctor */
      Orbit (btScalar I0, btScalar RAAN, btScalar ARGP, btScalar ECCE, btScalar TA, int Epoch);
      /** Default destructor */
      virtual ~Orbit();
      /** Update an orbit with a new position and velocity */
      void update (btVector3 Position, btVector3 Velocity);
      /** Accessors */
      btScalar getI0(void);
      btScalar getRAAN(void);
      btScalar getARGP(void);
      btScalar getECCE(void);
      btScalar getN0(void);
      btScalar getM0(void);
      btScalar geta(void);
      btVector3 getPosition(void);
      btVector3 getVelocity(void);
      friend std::ostream& operator << ( std::ostream& o, const Orbit& v );
      void printVector(void);

    private:
      btVector3 Position;       // Instantaneous Position
      btVector3 Velocity;       // Instantaneous Velocity
      btVector3 centralBodyPos; // What you're orbiting around
      btVector3 e;              // Eccentricity vector
      btVector3 h;              // Angular momentum vector
      btScalar I0;              // Inclination
      btScalar RAAN;            // Right Ascension of Ascending Node
      btScalar ARGP;            // Argument of Periapsis
      btScalar ECCE;            // Eccentricity
      btScalar N0;              // Mean Anomaly
      btScalar M0;              // Mean Motion
      btScalar a;               // Semi-major Axis
      btScalar TA;              // True anomaly
      btScalar Periapsis;
      btScalar Apoapsis;
      int Epoch;
  };
}
#endif // ORBIT_H
