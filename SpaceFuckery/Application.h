#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

/*************************************************
*                                                *
* Project: SpaceFuckery                          *
*                                                *
*   Copyright 2015 - 2016 - Marc-Olivier Barre   *
*              All rights reserved               *
*                                                *
**************************************************/

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

// Predefine a our class
namespace SpaceFuckery {class Application;}

// Do I really need to export this ?
template class _OgreExport Ogre::Singleton<SpaceFuckery::Application>;

namespace SpaceFuckery
{
  // The application class is our application defined as a singleton.
  // Only one instance will be allocated and will be accessible throughout our program
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
      btDefaultCollisionConfiguration* collisionConfiguration;
      btCollisionDispatcher* dispatcher;
      btBroadphaseInterface* pairCache;
      btSequentialImpulseConstraintSolver* solver;
      SpaceFuckery::physicsEngine* mPhysicsEngine;
      bool mShutDown;
      virtual bool loadRessources (Ogre::String Cfg);
      virtual void createListeners (void);
      virtual void createGUI (void);
      virtual void createScene (void);
      virtual bool startRendering (void);
      virtual bool quit (const CEGUI::EventArgs &e);
  };
}

#endif //APPLICATION_H_INCLUDED