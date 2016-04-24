/*************************************************
*                                                *
* Project: SpaceFuckery                          *
*                                                *
*   Copyright 2015 - 2016 - Marc-Olivier Barre   *
*              All rights reserved               *
*                                                *
**************************************************/

#include "Orbit.h"
#include <cmath>
#include <string>
#include <iostream>

const btScalar Pi = 3.14159265358979;
const btScalar twoPi = 2*Pi;
const btScalar earthMass = 5.97237e24;
const btScalar G = 6.6725985e-11;
const btScalar Mu = G*earthMass;

namespace SpaceFuckery
{
  Orbit::Orbit (btVector3 Position, btVector3 Velocity, btVector3 centralBodyPos, btScalar mass) :
    Position (Position),
    Velocity (Velocity),
    centralBodyPos (centralBodyPos)
  {
    btScalar R = Position.length(); // Distance to central body
    btScalar V = Velocity.length(); // Linear speed
    a = 1/(2/R - V*V/Mu); //  semi-major axis

    /** Angular momentum */
    h = Position.cross(Velocity);
    btScalar H = h.length();
    btScalar Hx = h.getX();
    btScalar Hy = h.getY();
    btScalar Hz = h.getZ();

    btScalar q = Position.dot(Velocity);

    e = Velocity.cross(h)/Mu-Position/Position.length(); // Eccentricity vector
    ECCE = e.length(); // Eccentricity
    I0 = std::acos(Hz/H); //inclination
    RAAN = (I0 == 0)?0:std::atan2(Hx, -Hy); // Right ascension of ascending node

    btScalar TAx = H*H/(R*Mu) - 1;
    btScalar TAy = H*q/(R*Mu);
    btScalar TA = std::atan2(TAy, TAx);
    btScalar Cw = (Position.getX()*std::cos(RAAN) + Position.getY()*std::sin(RAAN))/R;

    btScalar Sw = ( I0 == 0 || I0 == Pi)?(Position.getY()*std::cos(RAAN) - Position.getX()*std::sin(RAAN))/R:Position.getZ()/(R*std::sin(I0));

    ARGP = std::atan2(Sw, Cw) - TA; // Argument of periapsis
    if (ARGP < 0) { ARGP = twoPi + ARGP; };

    btScalar PlusMinus = a*ECCE;
    Periapsis = a - PlusMinus; // Periapsis
    Apoapsis = a + PlusMinus; // Apoapsis
    //btScalar u = std::atan2(e.getY(), e.getX()); // Eccentric anomaly
    btScalar u = e.angle(btVector3(1,0,0)); // Eccentric anomaly
    M0 = u - ECCE*std::sin(u); // Mean anomaly
    btScalar TL = ARGP + TA + RAAN; // True longitude
    N0 = twoPi*std::sqrt((a*a*a / (G*(mass + earthMass)))); // Mean motion or period
  }

  Orbit::Orbit (btScalar I0, btScalar RAAN, btScalar ARGP, btScalar ECCE, btScalar N0, btScalar M0) :
    I0 (I0),
    RAAN (RAAN),
    ARGP (ARGP),
    ECCE (ECCE),
    N0 (N0),
    M0 (M0)
  {

  }

  Orbit::~Orbit()
  {

  }

  void Orbit::update (btVector3 Position, btVector3 Velocity)
  {

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
      o << "Orbit(" << "ECCE: " << v.ECCE << ", " << "I0: " << v.I0*180/Pi << ", " << "RAAN: " << v.RAAN*180/Pi << ", " << "ARGP: " << v.ARGP*180/Pi << ", " << "N0: " << v.N0 << ", " << "M0: " << v.M0 << ", " << "a: " << v.a << ", " << "Periapsis: " << v.Periapsis << ", " << "Apoapsis: " << v.Apoapsis << ")";
      return o;
  }
}