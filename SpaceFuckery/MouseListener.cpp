/*************************************************
*                                                *
* Project: SpaceFuckery                          *
*                                                *
*   Copyright 2015 - 2016 - Marc-Olivier Barre   *
*              All rights reserved               *
*                                                *
**************************************************/

#include "CEGUI/CEGUI.h"

#include "Application.h"
#include "MouseListener.h"

CEGUI::MouseButton convertButton (OIS::MouseButtonID buttonID)
{
  switch (buttonID)
    {
    case OIS::MB_Left:
      return CEGUI::LeftButton;

    case OIS::MB_Right:
      return CEGUI::RightButton;

    case OIS::MB_Middle:
      return CEGUI::MiddleButton;

    default:
      return CEGUI::LeftButton;
    }
}

namespace SpaceFuckery
{
  bool MouseListener::mouseMoved (const OIS::MouseEvent &arg)
  {
    CEGUI::System &sys = CEGUI::System::getSingleton();
    sys.getDefaultGUIContext().injectMousePosition (arg.state.X.abs, arg.state.Y.abs);

    // Scroll wheel.
    if (arg.state.Z.rel)
      sys.getDefaultGUIContext().injectMouseWheelChange (arg.state.Z.rel / 120.0f);

    return true;
  }

  bool MouseListener::mousePressed (const OIS::MouseEvent &arg, OIS::MouseButtonID id)
  {
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown (convertButton (id));
    return true;
  }

  bool MouseListener::mouseReleased (const OIS::MouseEvent &arg, OIS::MouseButtonID id)
  {
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp (convertButton (id));
    return true;
  }
}
