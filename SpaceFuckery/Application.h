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

#include <OgreRoot.h>
#include <OgreSingleton.h>
#include <OgreRenderWindow.h>
#include <OISMouse.h>
#include <OISKeyboard.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <iomanip>

#include "FrameListener.h"
#include "WindowEventListener.h"
#include "MouseListener.h"
#include "KeyListener.h"
#include "physicsEngine.h"

// Predefine our class
namespace SpaceFuckery {class Application;}
template class Ogre::Singleton<SpaceFuckery::Application>;

namespace SpaceFuckery
{
  /*! The application class is our application defined as a singleton.
      Only one instance will be allocated and will be accessible throughout our program */
  class Application:
    public Ogre::Singleton<Application>
  {
    public:
      /** \brief */
      Application();

      /** \brief */
      virtual ~Application();

      /** \brief Fires up the application. This is the Application class entry point.
       *
       *  \return false if configuration goes wrong, true otherwise.
       *
       */
      virtual bool go();

      /** \brief
       *
       * \return
       *
       */
      virtual Ogre::RenderWindow* getWindow (void);

      /** \brief
       *
       * \return
       *
       */
      virtual Ogre::SceneManager* getSceneMgr (void);

      /** \brief
       *
       * \return
       *
       */
      virtual bool getShutDown (void);

      /** \brief
       *
       * \return
       *
       */
      virtual void setShutDown (bool value);

      /** \brief
       *
       * \return
       *
       */
      virtual OIS::Mouse* getMouse (void);

      /** \brief
       *
       * \return
       *
       */
      virtual OIS::Keyboard* getKeyboard (void);

      /** \brief
       *
       * \return
       *
       */
      virtual OIS::InputManager* getInputManager (void);

      /** \brief
       *
       * \return
       *
       */
      virtual SpaceFuckery::physicsEngine* getPhysicsEngine (void);

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

      /** \brief
       *
       * \return
       *
       */
      virtual bool loadRessources (Ogre::String Cfg);

      /** \brief
       *
       * \return
       *
       */
      virtual void createListeners (void);

      /** \brief
       *
       * \return
       *
       */
      virtual void createGUI (void);

      /** \brief
       *
       * \return
       *
       */
      virtual void createScene (void);

      /** \brief
       *
       * \return
       *
       */
      virtual bool startRendering (void);

      /** \brief Run when you want to quit the app */
      virtual bool quit (const CEGUI::EventArgs &e);
  };
}

#endif //APPLICATION_H_INCLUDED
