/******************************************
*                                         *
* Project: SpaceFuckery                   *
* Function:                               *
*                                         *
*   Copyright 2015 - Marc-Olivier Barre   *
*           All rights reserved           *
*                                         *
******************************************/

#include <OISKeyboard.h>
#include <CEGUI/CEGUI.h>

#include "Application.h"
#include "KeyListener.h"

namespace SpaceFuckery
{
  bool KeyListener::keyPressed ( const OIS::KeyEvent &arg )
  {
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    context.injectKeyDown ( (CEGUI::Key::Scan) arg.key);
    context.injectChar ( (CEGUI::Key::Scan) arg.text);
    if (arg.key == OIS::KC_ESCAPE)
      {
        SpaceFuckery::Application::getSingleton().setShutDown(true);
      }
    return true;
  }

  bool KeyListener::keyReleased ( const OIS::KeyEvent &arg )
  {
    CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp ( (CEGUI::Key::Scan) arg.key);
    return true;
  }
}