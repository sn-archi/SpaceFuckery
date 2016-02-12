#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

/******************************************
*                                         *
* Project: SpaceFuckery                   *
* Singleton: Application                  *
*                                         *
*   Copyright 2015 - Marc-Olivier Barre   *
*           All rights reserved           *
*                                         *
******************************************/

#include "OgreRoot.h"
#include "OgreSingleton.h"
#include "OgreRenderWindow.h"
#include "OISMouse.h"
#include "OISKeyboard.h"
#include "CEGUI/RendererModules/Ogre/Renderer.h"

#include "FrameListener.h"
#include "WindowEventListener.h"
#include "MouseListener.h"
#include "KeyListener.h"
#include "physicsEngine.h"

namespace SpaceFuckery {class Application;}

template class _OgreExport Ogre::Singleton<SpaceFuckery::Application>;

namespace SpaceFuckery
{
  class Application:
    public Ogre::Singleton<Application>
  {
    public:
      Application();
      virtual ~Application();
      virtual bool go();
      virtual Ogre::RenderWindow* getWindow(void);
      virtual Ogre::SceneManager* getSceneMgr(void);
      virtual bool getShutDown(void);
      virtual void setShutDown(bool value);
      virtual OIS::Mouse* getMouse(void);
      virtual OIS::Keyboard* getKeyboard(void);
      virtual OIS::InputManager* getInputManager(void);
      virtual SpaceFuckery::physicsEngine* getPhysicsEngine(void);

    private:
      Ogre::Root* mRoot;
      SpaceFuckery::FrameListener* mFrameListener;
      SpaceFuckery::WindowEventListener* mWindowEventListener;
      SpaceFuckery::MouseListener* mMouseListener;
      SpaceFuckery::KeyListener* mKeyListener;
      OIS::Mouse* mMouse;
      OIS::Keyboard* mKeyboard;
      Ogre::RenderWindow* mWindow;
      Ogre::SceneManager* mSceneMgr;
      Ogre::String mResourcesCfg;
      Ogre::String mPluginsCfg;
      OIS::InputManager* mInputManager;
      CEGUI::OgreRenderer* mRenderer;
      SpaceFuckery::physicsEngine* mPhysicsEngine;
      bool mShutDown;
      virtual bool loadRessources (Ogre::String Cfg);
      // SpaceFuckery::FrameListener
      virtual void createListeners (void);
      // OIS::KeyListener

      // OIS::MouseListener
      virtual void createGUI (void);
      virtual void createScene (void);
      virtual bool startRendering (void);
      virtual bool quit (const CEGUI::EventArgs &e);
  };
}

#endif //APPLICATION_H_INCLUDED