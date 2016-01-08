#ifndef KEYLISTENER_H_INCLUDED
#define KEYLISTENER_H_INCLUDED

/******************************************
*                                         *
* Project: SpaceFuckery                   *
* Function:                               *
*                                         *
*   Copyright 2015 - Marc-Olivier Barre   *
*           All rights reserved           *
*                                         *
******************************************/

#include <OISInputManager.h>

namespace SpaceFuckery
{
  class KeyListener:
    public OIS::KeyListener
  {
    private:
      virtual bool keyPressed ( const OIS::KeyEvent &arg );
      virtual bool keyReleased ( const OIS::KeyEvent &arg );
  };
}

#endif // KEYLISTENER_H_INCLUDED
