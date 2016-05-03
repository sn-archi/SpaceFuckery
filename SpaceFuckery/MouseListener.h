#ifndef MOUSELISTENER_H_INCLUDED
#define MOUSELISTENER_H_INCLUDED

/*************************************************
*                                                *
* Project: SpaceFuckery                          *
*                                                *
*   Copyright 2015 - 2016 - Marc-Olivier Barre   *
*              All rights reserved               *
*                                                *
**************************************************/

#include "OISInputManager.h"

namespace SpaceFuckery
{
  class MouseListener:
    public OIS::MouseListener
  {
    private:
      virtual bool mouseMoved (const OIS::MouseEvent &arg);
      virtual bool mousePressed (const OIS::MouseEvent &arg, OIS::MouseButtonID id);
      virtual bool mouseReleased (const OIS::MouseEvent &arg, OIS::MouseButtonID id);
  };
}

#endif // MOUSELISTENER_H_INCLUDED
