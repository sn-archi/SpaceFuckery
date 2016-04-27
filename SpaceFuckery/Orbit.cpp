/*************************************************
*                                                *
* Project: SpaceFuckery                          *
*                                                *
*   Copyright 2015 - 2016 - Marc-Olivier Barre   *
*              All rights reserved               *
*                                                *
**************************************************/

#include "Orbit.h"
#include "Constants.h"
#include <cmath>
#include <string>
#include <iostream>
#include <iomanip>

namespace SpaceFuckery
{
  Orbit::Orbit (btVector3 Position, btVector3 Velocity, btVector3 centralBodyPos, btScalar mass) :
    Position (Position),
    Velocity (Velocity),
    centralBodyPos (centralBodyPos)
  {
    update(Position, Velocity);
  }

  Orbit::Orbit (btScalar I0, btScalar RAAN, btScalar ARGP, btScalar ECCE, btScalar TA, int Epoch) :
    I0 (I0),
    RAAN (RAAN),
    ARGP (ARGP),
    ECCE (ECCE),
    TA (TA),
    Epoch (Epoch)
  {

  }

  Orbit::~Orbit()
  {

  }

  void Orbit::update (btVector3 Position, btVector3 Velocity)
  {
    /** distance and speed to central body */
    btScalar R = Position.length();
    btScalar V = Velocity.length();

    /** Angular momentum */
    h = Position.cross(Velocity);
    btScalar H = h.length();
    btScalar Hx = h.getX();
    btScalar Hy = h.getY();
    btScalar Hz = h.getZ();

    /** Node vector */
    btVector3 nhat = Khat.cross(h);

    /** Eccentricity vector */
    e = ((V*V - Mu/R)*Position - (Position.dot(Velocity))*Velocity)/Mu;
    std::cout << "e = (" << e.getX() << "," << e.getY() << "," << e.getZ() << ")" << std::endl;
    ECCE = e.length(); // Eccentricity


    /** Specific orbital energy */
    btScalar E = V*V/2 - Mu/R;
    std::cout << "E = " << E << std::endl;

    /** Semi-major axis and periapsis */
    if (e.length() != 1)
    {
      a = -Mu/(2*E);
      Periapsis = a*(1 - e.length()*e.length());
    }
    else
    {
      a = INFINITY;
      Periapsis = H*H/Mu;
    }

    /** I0: inclination */
    I0 = std::acos(h.getZ()/H);

    /** RAAN: Right ascension of ascending node */
    RAAN = std::acos(nhat.getX()/nhat.length());

    /** ARGP: Argument of periapsis */
    RAAN = std::acos(nhat.dot(e)/(nhat.length()*ECCE));

    /** TA: True anomaly*/
    TA = std::acos(e.dot(Position)/(ECCE*Position.length()));

    btScalar PlusMinus = a*ECCE;
    Periapsis = a - PlusMinus; // Periapsis
    Apoapsis = a + PlusMinus; // Apoapsis
  }

  btScalar Orbit::getI0(void)
  {
    return I0;
  }

  btScalar Orbit::getRAAN(void)
  {
    return RAAN;
  }

  btScalar Orbit::getARGP(void)
  {
    return ARGP;
  }

  btScalar Orbit::getECCE(void)
  {
    return ECCE;
  }

  btScalar Orbit::getN0(void)
  {
    return N0;
  }

  btScalar Orbit::getM0(void)
  {
    return M0;
  }

  btScalar Orbit::geta(void)
  {
    return a;
  }

  btVector3 Orbit::getPosition(void)
  {
    return Position;
  }

  btVector3 Orbit::getVelocity(void)
  {
    return Velocity;
  }

  /** Function for writing to a stream.*/
  std::ostream& operator << ( std::ostream& o, const Orbit& v )
  {
      //o << "Orbit(" << "ECCE: " << v.ECCE << ", " << "I0: " << v.I0*180/Pi << ", " << "RAAN: " << v.RAAN*180/Pi << ", " << "ARGP: " << v.ARGP*180/Pi << ", " << "N0: " << v.N0 << ", " << "M0: " << v.M0 << ", " << "a: " << v.a << ", " << "Periapsis: " << v.Periapsis << ", " << "Apoapsis: " << v.Apoapsis << ")";
      o << v.ECCE << ", " << v.Position.length() << ", " << v.Velocity.length() << ", " << v.I0*180/Pi << ", " << v.Periapsis << ", " << v.Apoapsis;
      return o;
  }

    void Orbit::printVector(void)
  {
      std::cout << std::setprecision(6);
      std::cout << Position.getX() << ", " << Position.getY() << ", " << Position.getZ() << " -- " << Velocity.getX() << ", " << Velocity.getY() << ", " << Velocity.getZ() << std::endl;
  }
}