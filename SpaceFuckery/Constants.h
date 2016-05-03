#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

/*************************************************
*                                                *
* Project: SpaceFuckery                          *
*                                                *
*   Copyright 2015 - 2016 - Marc-Olivier Barre   *
*              All rights reserved               *
*                                                *
**************************************************/

const btScalar Pi = 3.14159265358979;
const btScalar twoPi = 2 * Pi;
//const btScalar earthMass = 5.97237e24;
const btScalar earthMass = 5.97237e15;
const btScalar G = 6.6725985e-11;
const btScalar earthMu = G * earthMass;

const btVector3 Ihat = btVector3 (1, 0, 0);
const btVector3 Jhat = btVector3 (0, 1, 0);
const btVector3 Khat = btVector3 (0, 0, 1);

#endif // CONSTANTS_H_INCLUDED
