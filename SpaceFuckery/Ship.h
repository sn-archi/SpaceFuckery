/******************************************
*                                         *
* Project: SpaceFuckery                   *
* Function:                               *
*                                         *
*   Copyright 2015 - Marc-Olivier Barre   *
*           All rights reserved           *
*                                         *
******************************************/

#ifndef SHIP_H
#define SHIP_H

#include <mKOST.h>

namespace SpaceFuckery
{
  class Ship
  {
    public:
      /** Default constructor */
      Ship();

      /** Default destructor */
      virtual ~Ship();

    private:
      mKOST::Orbit mOrbit;

  };
}
#endif // SHIP_H