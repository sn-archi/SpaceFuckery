/******************************************
*                                         *
* Project: SpaceFuckery                   *
* Interface: WindowEventListener          *
*                                         *
*   Copyright 2015 - Marc-Olivier Barre   *
*           All rights reserved           *
*                                         *
******************************************/

#include "Application.h"
#include "WindowEventListener.h"

namespace SpaceFuckery
{
  WindowEventListener::WindowEventListener()
  {
  }
  //Adjust mouse clipping area
  void WindowEventListener::windowResized (Ogre::RenderWindow* rw)
  {
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics (width, height, depth, left, top);

    const OIS::MouseState &ms = Application::getSingleton().getMouse()->getMouseState();
    ms.width = width;
    ms.height = height;
  }

  //Unattach OIS before window shutdown (very important under Linux)
  void WindowEventListener::windowClosed (Ogre::RenderWindow* rw)
  {
    OIS::Mouse* mMouse = Application::getSingleton().getMouse();
    OIS::Keyboard* mKeyboard = Application::getSingleton().getKeyboard();
    Ogre::RenderWindow* mWindow = Application::getSingleton().getWindow();
    OIS::InputManager* mInputManager = Application::getSingleton().getInputManager();
    //Only close for window that created OIS (the main window in these demos)
    if (rw == mWindow)
      {
        if (mInputManager)
          {
            mInputManager->destroyInputObject ( mMouse );
            mInputManager->destroyInputObject ( mKeyboard );

            OIS::InputManager::destroyInputSystem (mInputManager);
            mInputManager = 0;
          }
      }
  }
}