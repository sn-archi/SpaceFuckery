/*************************************************
*                                                *
* Project: SpaceFuckery                          *
*                                                *
*   Copyright 2015 - 2016 - Marc-Olivier Barre   *
*              All rights reserved               *
*                                                *
**************************************************/

#include "OgreException.h"
#include "OgreConfigFile.h"
#include "OgreSceneManager.h"
#include "OgreCamera.h"
#include "OgreViewport.h"
#include "OgreEntity.h"
#include "OgreStringVector.h"

#include "OISEvents.h"

#include "CEGUI/CEGUI.h"

#include "Application.h"
#include "FrameListener.h"
#include "WindowEventListener.h"
#include "MouseListener.h"
#include "KeyListener.h"

namespace SpaceFuckery
{
  template<> SpaceFuckery::Application* Ogre::Singleton<SpaceFuckery::Application>::msSingleton = 0;

  Application::Application ()
    : mRoot (0),
      mFrameListener (0),
      mWindowEventListener (0),
      mMouseListener (0),
      mKeyListener (0),
      mMouse (0),
      mKeyboard (0),
      mWindow (0),
      mSceneMgr (0),
      mResourcesCfg (Ogre::StringUtil::BLANK),
      mPluginsCfg (Ogre::StringUtil::BLANK),
      mInputManager (0),
      mRenderer (0),
      mPhysicsEngine (0),
      mShutDown (false)
  {
    //Initialise our physics engine
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    pairCache = new btDbvtBroadphase();
    solver = new btSequentialImpulseConstraintSolver();
    mPhysicsEngine = new SpaceFuckery::physicsEngine(dispatcher, pairCache, solver, collisionConfiguration);
  }

  Application::~Application ()
  {
    delete collisionConfiguration;
    delete dispatcher;
    delete pairCache;
    delete solver;
    delete mPhysicsEngine;
    // Remove ourself as a Window listener
    // For some reasons that needs investigating this is the right way to do it.
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, mWindowEventListener);
    mWindowEventListener->windowClosed(mWindow);
    delete mRoot;
  }

  // Resource configuration loader
  // Parses our config file for resource locations
  bool Application::loadRessources (Ogre::String Cfg)
  {
    Ogre::ConfigFile cf;
    cf.load(Cfg);

    Ogre::String name, locType, secName;
    Ogre::ConfigFile::SectionIterator secIt(cf.getSectionIterator() );
    while (secIt.hasMoreElements() )
      {
        secName = secIt.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap* settings(secIt.getNext());
        Ogre::ConfigFile::SettingsMultiMap::iterator it;
        for (it = settings->begin(); it != settings->end(); ++it)
          {
            locType = it->first;
            name = it->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, locType, secName);
          }
      }
    return true;
  }

  // Create and activate the various listeners that will handle
  // mouse, keyboard or frame events
  void Application::createListeners (void)
  {
    OIS::ParamList pl;
    size_t windowHnd (0);
    std::ostringstream windowHndStr;
    mMouseListener = new SpaceFuckery::MouseListener;
    mKeyListener = new SpaceFuckery::KeyListener;
    mWindowEventListener = new WindowEventListener;
    mFrameListener = new FrameListener;

    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert (std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    // Setup the mouse so that it doesn't get stuck inside the window
    #if defined OIS_WIN32_PLATFORM
    pl.insert(std::make_pair(std::string("w32_mouse"),std::string("DISCL_FOREGROUND") ));
    pl.insert(std::make_pair(string("w32_mouse"),string("DISCL_EXCLUSIVE") ));
    pl.insert(std::make_pair(std::string("w32_keyboard"),std::string("DISCL_FOREGROUND") ));
    pl.insert(std::make_pair(string("w32_keyboard"),string("DISCL_EXCLUSIVE") ));
    #elif defined OIS_LINUX_PLATFORM
    pl.insert(std::make_pair(std::string("x11_mouse_grab"),std::string("false") ));
    pl.insert(std::make_pair(std::string("x11_keyboard_grab"),std::string("false") ));
    #endif

    // Setup our input listeners
    mInputManager = OIS::InputManager::createInputSystem(pl);
    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject(OIS::OISKeyboard, true));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject(OIS::OISMouse, true));
    mMouse->setEventCallback(mMouseListener);
    mKeyboard->setEventCallback(mKeyListener);

    //Set initial mouse clipping size
    mWindowEventListener->windowResized(mWindow);

    //Register a window listener for the main window
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, mWindowEventListener);

    //Register a frame listener
    mRoot->addFrameListener(mFrameListener);
  }

  // Load our GUI objects from our XML CEGUI layout
  void Application::createGUI (void)
  {
    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
    CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

    CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
    CEGUI::Window* flightWin = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("flight.layout");
    CEGUI::Window* quitButton = flightWin->getChild("QuitButton");
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(flightWin);

    quitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Application::quit, this));
  }

  // Goto function to create a testing scene inside the Application.
  // Should disapear at some point.
  void Application::createScene (void)
  {
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
    Ogre::Camera* mCamera;
    mCamera = mSceneMgr->createCamera("MainCam");
    mCamera->setPosition(5, 0, 15);
    mCamera->lookAt(0, 0, 0);
    mCamera->setNearClipDistance(1);

    Ogre::Viewport* vp = mWindow->addViewport(mCamera);
    vp->setBackgroundColour (Ogre::ColourValue(0, 0, 0) );

    mCamera->setAspectRatio(
      Ogre::Real(vp->getActualWidth() ) /
      Ogre::Real(vp->getActualHeight() ) );

    Ogre::Entity* suzzyEntity = mSceneMgr->createEntity("Suzanne.mesh");

    Ogre::SceneNode* suzzyNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Suzzy");
    suzzyNode->attachObject(suzzyEntity);

    mSceneMgr->setAmbientLight(Ogre::ColourValue (.5, .5, .5));

    Ogre::Light* light = mSceneMgr->createLight("MainLight");
    mSceneMgr->createLight();
    light->setPosition(20, 80, 50);

    // Setup some physics for our objects
    btTransform suzzyTransform;
    suzzyTransform.setIdentity();
    suzzyTransform.setOrigin(btVector3(-5, 0, 0));

    btScalar suzzyMass(1);
    btVector3 localSuzzyInertia(1, 1, 1);

    btCollisionShape *suzzyShape = new btBoxShape(btVector3(btScalar (1.), btScalar (1.), btScalar (1.)));
    btDefaultMotionState *suzzyMotionState = new btDefaultMotionState(suzzyTransform);

    suzzyShape->calculateLocalInertia(suzzyMass, localSuzzyInertia);

    btRigidBody::btRigidBodyConstructionInfo suzzyRBInfo(suzzyMass, suzzyMotionState, suzzyShape, localSuzzyInertia);
    btRigidBody* suzzyBody = new btRigidBody(suzzyRBInfo);

    suzzyBody->setUserPointer(suzzyNode);

    //add the body to the dynamics world
    mPhysicsEngine->addRigidBody(suzzyBody);
    mPhysicsEngine->setCollisionObjectCount();

    // Give it speed
    suzzyBody->setLinearVelocity(btVector3(0, 2, 10));
  }

  // Rendering loop kinda happens here
  bool Application::startRendering (void)
  {
    while (true)
      {
        Ogre::WindowEventUtilities::messagePump();
        if (!mRoot->renderOneFrame()) return false;
      }
  }

  // Fires up the application. This is the Application class entry point.
  bool Application::go()
  {
    mResourcesCfg = "etc/resources.cfg";
    mPluginsCfg = "etc/plugins.cfg";

    mRoot = new Ogre::Root(mPluginsCfg);

    Application::loadRessources(mResourcesCfg);

    if (!(mRoot->restoreConfig() || mRoot->showConfigDialog()))
      return false;

    mWindow = mRoot->initialise(true, "SpaceFuckery");

    Application::createScene();
    Application::createGUI();
    Application::createListeners();
    Application::startRendering();
    return true;
  }

  // Run when you want to qui the app.
  bool Application::quit (const CEGUI::EventArgs &e)
  {
    mShutDown = true;
    return true;
  }


  // Accessors go here
  Ogre::RenderWindow* Application::getWindow (void)
  {
    return mWindow;
  }

  Ogre::SceneManager* Application::getSceneMgr (void)
  {
    return mSceneMgr;
  }

  bool Application::getShutDown (void)
  {
    return mShutDown;
  }

  void Application::setShutDown (bool value)
  {
    mShutDown = value;
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

  SpaceFuckery::physicsEngine* Application::getPhysicsEngine (void)
  {
    return mPhysicsEngine;
  }
}
