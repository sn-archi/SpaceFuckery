/******************************************
*                                         *
* Project: SpaceFuckery                   *
* Singleton: Application                  *
*                                         *
*   Copyright 2015 - Marc-Olivier Barre   *
*           All rights reserved           *
*                                         *
******************************************/

#include <OgreException.h>
#include <OgreConfigFile.h>
#include <OgreSceneManager.h>
#include <OgreCamera.h>
#include <OgreViewport.h>
#include <OgreEntity.h>
#include <OgreStringVector.h>

#include <OISEvents.h>

#include <CEGUI/CEGUI.h>

#include "Application.h"
#include "FrameListener.h"
#include "WindowEventListener.h"
#include "MouseListener.h"
#include "KeyListener.h"

namespace SpaceFuckery
{
  template<> SpaceFuckery::Application* Ogre::Singleton<SpaceFuckery::Application>::msSingleton = 0;

  Application::Application()
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
    mPhysicsEngine = new SpaceFuckery::physicsEngine();
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

  void Application::createListeners (void)
  {
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
    mMouseListener = new SpaceFuckery::MouseListener;
    mKeyListener = new SpaceFuckery::KeyListener;
    mWindowEventListener = new WindowEventListener;
    mFrameListener = new FrameListener;

    mWindow->getCustomAttribute ("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert (std::make_pair (std::string ("WINDOW"), windowHndStr.str() ) );

    // Setup our input listeners
    mInputManager = OIS::InputManager::createInputSystem ( pl );
    mKeyboard = static_cast<OIS::Keyboard*> (mInputManager->createInputObject (OIS::OISKeyboard, true));
    mMouse = static_cast<OIS::Mouse*> (mInputManager->createInputObject (OIS::OISMouse, true));
    mMouse->setEventCallback (mMouseListener);
    mKeyboard->setEventCallback (mKeyListener);

    //Set initial mouse clipping size
    mWindowEventListener->windowResized (mWindow);

    //Register a window listener for the main window
    Ogre::WindowEventUtilities::addWindowEventListener (mWindow, mWindowEventListener);

    //Register a frame listener
    mRoot->addFrameListener (mFrameListener);
  }

  void Application::createGUI (void)
  {
    mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
    CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
    CEGUI::Font::setDefaultResourceGroup("Fonts");
    CEGUI::Scheme::setDefaultResourceGroup("Schemes");
    CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
    CEGUI::WindowManager::setDefaultResourceGroup("Layouts");

    CEGUI::SchemeManager::getSingleton().createFromFile ("TaharezLook.scheme");
    CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");
    CEGUI::Window* flightWin = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("flight.layout");
    CEGUI::Window* quitButton = flightWin->getChild ("QuitButton");
    CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow (flightWin);

    quitButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&Application::quit, this));
  }

  void Application::createScene (void)
  {
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
    Ogre::Camera* mCamera;
    mCamera = mSceneMgr->createCamera ("MainCam");
    mCamera->setPosition (2, 2, 6);
    mCamera->lookAt (0, 0, 0);
    mCamera->setNearClipDistance (1);

    Ogre::Viewport* vp = mWindow->addViewport (mCamera);
    vp->setBackgroundColour (Ogre::ColourValue (0, 0, 0) );

    mCamera->setAspectRatio (
      Ogre::Real (vp->getActualWidth() ) /
      Ogre::Real (vp->getActualHeight() ) );

    Ogre::Entity* suzzyEntity = mSceneMgr->createEntity ("Suzanne.mesh");

    Ogre::SceneNode* suzzyNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Suzzy");
    std::cout << "Prout lol";
    suzzyNode->attachObject (suzzyEntity);

    mSceneMgr->setAmbientLight (Ogre::ColourValue (.5, .5, .5) );

    Ogre::Light* light = mSceneMgr->createLight ("MainLight");
    mSceneMgr->createLight();
    light->setPosition (20, 80, 50);

    // Setup some physics for our objects
    btTransform suzzyTransform;
    suzzyTransform.setIdentity();
    suzzyTransform.setOrigin(btVector3(0, 0, 0));

    btScalar suzzyMass(1);
    btVector3 localSuzzyInertia(0, 0, 0);

    btCollisionShape *suzzyShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));
    btDefaultMotionState *suzzyMotionState = new btDefaultMotionState(suzzyTransform);

    suzzyShape->calculateLocalInertia(suzzyMass, localSuzzyInertia);

    btRigidBody::btRigidBodyConstructionInfo suzzyRBInfo(suzzyMass, suzzyMotionState, suzzyShape, localSuzzyInertia);
    btRigidBody* suzzyBody = new btRigidBody(suzzyRBInfo);

    suzzyBody->setUserPointer(suzzyNode);

    //add the body to the dynamics world
    mPhysicsEngine->getDynamicsWorld()->addRigidBody(suzzyBody);
    mPhysicsEngine->countPhysicsObject();
  }

  bool Application::startRendering (void)
  {
    while (true)
      {
        Ogre::WindowEventUtilities::messagePump();
        if (!mRoot->renderOneFrame() ) return false;
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
    Application::createListeners();
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

  Ogre::SceneManager* Application::getSceneMgr(void)
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

  SpaceFuckery::physicsEngine* Application::getPhysicsEngine(void)
  {
    return mPhysicsEngine;
  }
}