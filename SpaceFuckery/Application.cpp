#include "Application.h"
#include "FrameListener.h"
#include "WindowEventListener.h"

#include <OgreException.h>
#include <OgreConfigFile.h>
#include <OgreRenderWindow.h>
#include <OgreSceneManager.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreEntity.h>
#include <OgreWindowEventUtilities.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#include <OgreStringVector.h>

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
  template<> SpaceFuckery::Application* Ogre::Singleton<SpaceFuckery::Application>::msSingleton = 0;

  Application::Application()
    : mRoot (0),
      mFrameListener (0),
      mWindowEventListener (0),
      mWindow (0),
      mResourcesCfg (Ogre::StringUtil::BLANK),
      mPluginsCfg (Ogre::StringUtil::BLANK),
      mInputManager (0),
      mMouse (0),
      mKeyboard (0),
      mRenderer (0),
      mShutDown (false)
  {
  }

  Application::~Application()
  {
    //Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener (mWindow, mWindowEventListener);
    mWindowEventListener->windowClosed (mWindow);
    delete mRoot;
  }

  // Resource configuration loader
  bool Application::loadRessources (Ogre::String Cfg)
  {
    Ogre::ConfigFile cf;
    cf.load (Cfg);

    Ogre::String name, locType, secName;
    Ogre::ConfigFile::SectionIterator secIt = cf.getSectionIterator();
    while (secIt.hasMoreElements() )
      {
        secName = secIt.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator it;
        for (it = settings->begin(); it != settings->end(); ++it)
          {
            locType = it->first;
            name = it->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation (name, locType, secName);
          }
      }
    return true;
  }

  void Application::createFrameListener (void)
  {
    Ogre::LogManager::getSingletonPtr()->logMessage ("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    mWindow->getCustomAttribute ("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert (std::make_pair (std::string ("WINDOW"), windowHndStr.str() ) );

    mInputManager = OIS::InputManager::createInputSystem ( pl );
    mKeyboard = static_cast<OIS::Keyboard*> (
                  mInputManager->createInputObject ( OIS::OISKeyboard, true ) );
    mMouse = static_cast<OIS::Mouse*> (
               mInputManager->createInputObject ( OIS::OISMouse, true ) );

    mMouse->setEventCallback (this);
    mKeyboard->setEventCallback (this);

    mWindowEventListener = new WindowEventListener;
    //Set initial mouse clipping size
    mWindowEventListener->windowResized (mWindow);

    //Register a Window listener for the main window
    std::cout << "WindowEventListener...\n";
    Ogre::WindowEventUtilities::addWindowEventListener (mWindow, mWindowEventListener);

    std::cout << "FrameListener...\n";
    mFrameListener = new FrameListener;
    mRoot->addFrameListener (mFrameListener);
  }

  bool Application::keyPressed ( const OIS::KeyEvent &arg )
  {
    CEGUI::GUIContext& context = CEGUI::System::getSingleton().getDefaultGUIContext();
    context.injectKeyDown ( (CEGUI::Key::Scan) arg.key);
    context.injectChar ( (CEGUI::Key::Scan) arg.text);
    if (arg.key == OIS::KC_ESCAPE)
      {
        mShutDown = true;
      }
    return true;
  }

  bool Application::keyReleased ( const OIS::KeyEvent &arg )
  {
    CEGUI::System::getSingleton().getDefaultGUIContext().injectKeyUp ( (CEGUI::Key::Scan) arg.key);
    return true;
  }

  bool Application::mouseMoved ( const OIS::MouseEvent &arg )
  {
    CEGUI::System &sys = CEGUI::System::getSingleton();
    sys.getDefaultGUIContext().injectMouseMove (arg.state.X.rel, arg.state.Y.rel);
    // Scroll wheel.
    if (arg.state.Z.rel)
      sys.getDefaultGUIContext().injectMouseWheelChange (arg.state.Z.rel / 120.0f);
    return true;
  }

  bool Application::mousePressed ( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
  {
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonDown (convertButton (id) );
    return true;
  }

  bool Application::mouseReleased ( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
  {
    CEGUI::System::getSingleton().getDefaultGUIContext().injectMouseButtonUp (convertButton (id) );
    return true;
  }

  void Application::createGUI (void)
  {
    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
    CEGUI::ImageManager::setImagesetDefaultResourceGroup ("Imagesets");
    CEGUI::Font::setDefaultResourceGroup ("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup ("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup ("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup ("Layouts");

    CEGUI::SchemeManager::getSingleton().createFromFile ("TaharezLook.scheme");
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage ("TaharezLook/MouseArrow");
    CEGUI::WindowManager &wmgr = CEGUI::WindowManager::getSingleton();
    CEGUI::Window* flightWin = wmgr.loadLayoutFromFile ("flight.layout");
    CEGUI::Window *quitButton = flightWin->getChild ("QuitButton");
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow (flightWin);

    quitButton->subscribeEvent (CEGUI::PushButton::EventClicked,
                                CEGUI::Event::Subscriber (&Application::quit, this) );
  }

  void Application::createScene (void)
  {
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps (5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    Ogre::SceneManager* mSceneMgr;
    mSceneMgr = mRoot->createSceneManager (Ogre::ST_GENERIC);

    Ogre::Camera* mCamera;
    mCamera = mSceneMgr->createCamera ("MainCam");
    mCamera->setPosition (0, 0, 80);
    mCamera->lookAt (0, 0, -300);
    mCamera->setNearClipDistance (5);

    Ogre::Viewport* vp = mWindow->addViewport (mCamera);
    vp->setBackgroundColour (Ogre::ColourValue (0, 0, 0) );

    mCamera->setAspectRatio (
      Ogre::Real (vp->getActualWidth() ) /
      Ogre::Real (vp->getActualHeight() ) );

    Ogre::Entity* ogreEntity = mSceneMgr->createEntity ("ogrehead.mesh");

    Ogre::SceneNode* ogreNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    ogreNode->attachObject (ogreEntity);

    mSceneMgr->setAmbientLight (Ogre::ColourValue (.5, .5, .5) );

    Ogre::Light* light = mSceneMgr->createLight ("MainLight");
    light->setPosition (20, 80, 50);
  }

  bool Application::startRendering (void)
  {
    //  std::cout << "Starting the rendering loop";
    while (true)
      {
        //    std::cout << "Looping...";
        Ogre::WindowEventUtilities::messagePump();
        //    std::cout << "WindowEventUtilities::messagePump()";
        if (!mRoot->renderOneFrame() ) return false;
        //    std::cout << "mRoot->renderOneFrame()";
      }
  }

  bool Application::go()
  {
    mResourcesCfg = "etc/resources.cfg";
    mPluginsCfg = "etc/plugins.cfg";

    mRoot = new Ogre::Root (mPluginsCfg);

    Application::loadRessources (mResourcesCfg);

    if (! (mRoot->restoreConfig() || mRoot->showConfigDialog() ) )
      return false;

    mWindow = mRoot->initialise (true, "SpaceFuckery");

    Application::createScene();
    Application::createFrameListener();
    Application::createGUI();
    Application::startRendering();
    return true;
  }

  bool Application::quit (const CEGUI::EventArgs &e)
  {
    mShutDown = true;
    return true;
  }

  Ogre::RenderWindow* Application::getWindow (void)
  {
    return mWindow;
  }

  bool Application::getShutDown (void)
  {
    return mShutDown;
  }

  OIS::Mouse* Application::getMouse (void)
  {
    return mMouse;
  }

  OIS::Keyboard* Application::getKeyboard (void)
  {
    return mKeyboard;
  }

  OIS::InputManager* Application::getInputManager (void)
  {
    return mInputManager;
  }
}