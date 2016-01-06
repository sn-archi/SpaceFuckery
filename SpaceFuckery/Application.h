#ifndef APPLICATION_H_INCLUDED
#define APPLICATION_H_INCLUDED

/******************************************
*                                         *
* Project: SpaceFuckery                   *
* Function:                               *
*                                         *
*   Copyright 2015 - Marc-Olivier Barre   *
*           All rights reserved           *
*                                         *
******************************************/

#include <OgreRoot.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OgreWindowEventUtilities.h>
#include <OgreSingleton.h>
#include <OgreRenderWindow.h>

#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

namespace SpaceFuckery {class Application;}

template class _OgreExport Ogre::Singleton<SpaceFuckery::Application>;

namespace SpaceFuckery
{
  class Application:
    public Ogre::Singleton<Application>,
    public Ogre::WindowEventListener,
    public OIS::KeyListener,
    public OIS::MouseListener
  {
    public:
      static Application& getSingleton(void);
      static Application* getSingletonPtr(void);
      Application();
      virtual ~Application();
      virtual bool go();
      virtual Ogre::RenderWindow* getWindow(void);
      virtual bool getShutDown(void);
      virtual OIS::Mouse* getMouse(void);
      virtual OIS::Keyboard* getKeyboard(void);

    private:
      Ogre::Root* mRoot;
      Ogre::FrameListener* mFrameListener;
      Ogre::RenderWindow* mWindow;
      Ogre::String mResourcesCfg;
      Ogre::String mPluginsCfg;
      OIS::InputManager* mInputManager;
      OIS::Mouse* mMouse;
      OIS::Keyboard* mKeyboard;
      CEGUI::OgreRenderer* mRenderer;
      bool mShutDown;
      virtual bool loadRessources (Ogre::String Cfg);
      virtual void windowResized (Ogre::RenderWindow* rw);
      virtual void windowClosed (Ogre::RenderWindow* rw);
      // Ogre::FrameListener
      virtual void createFrameListener (void);
      // OIS::KeyListener
      virtual bool keyPressed ( const OIS::KeyEvent &arg );
      virtual bool keyReleased ( const OIS::KeyEvent &arg );
      // OIS::MouseListener
      virtual bool mouseMoved ( const OIS::MouseEvent &arg );
      virtual bool mousePressed ( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
      virtual bool mouseReleased ( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
      virtual void createGUI (void);
      virtual void createScene (void);
      virtual bool startRendering (void);
      virtual bool quit (const CEGUI::EventArgs &e);
  };
}

#endif //APPLICATION_H_INCLUDED