#include "SpaceFuckery.h"

#include <OgreException.h>
#include <OgreConfigFile.h>
#include "OgreRenderWindow.h"
#include <OgreSceneManager.h>
#include <OgreCamera.h>
#include "OgreViewport.h"
#include <OgreEntity.h>
#include <OgreWindowEventUtilities.h>
#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

SpaceFuckery::SpaceFuckery()
  : mRoot(0),
    mWindow(0),
    mResourcesCfg(Ogre::StringUtil::BLANK),
    mPluginsCfg(Ogre::StringUtil::BLANK),
    mInputManager(0),
    mMouse(0),
    mKeyboard(0)
{
}

SpaceFuckery::~SpaceFuckery()
{
  //Remove ourself as a Window listener
  Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
  windowClosed(mWindow);
  delete mRoot;
}

//Adjust mouse clipping area
void SpaceFuckery::windowResized(Ogre::RenderWindow* rw)
{
  unsigned int width, height, depth;
  int left, top;
  rw->getMetrics(width, height, depth, left, top);

  const OIS::MouseState &ms = mMouse->getMouseState();
  ms.width = width;
  ms.height = height;
}

//Unattach OIS before window shutdown (very important under Linux)
void SpaceFuckery::windowClosed(Ogre::RenderWindow* rw)
{
  //Only close for window that created OIS (the main window in these demos)
  if(rw == mWindow)
  {
    if(mInputManager)
    {
      mInputManager->destroyInputObject( mMouse );
      mInputManager->destroyInputObject( mKeyboard );

      OIS::InputManager::destroyInputSystem(mInputManager);
      mInputManager = 0;
    }
  }
}

bool SpaceFuckery::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
  if(mWindow->isClosed())
    return false;

  //Need to capture/update each device
  mKeyboard->capture();
  mMouse->capture();

  if(mKeyboard->isKeyDown(OIS::KC_ESCAPE))
    return false;

  return true;
}

bool SpaceFuckery::keyPressed( const OIS::KeyEvent &arg )
{
  return BaseApplication::keyPressed(arg);
}
//-------------------------------------------------------------------------------------
bool SpaceFuckery::keyReleased( const OIS::KeyEvent &arg )
{
  return BaseApplication::keyReleased(arg);
}
//-------------------------------------------------------------------------------------
bool SpaceFuckery::mouseMoved( const OIS::MouseEvent &arg )
{
  return BaseApplication::mouseMoved(arg);
}
//-------------------------------------------------------------------------------------
bool SpaceFuckery::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
  return BaseApplication::mousePressed(arg, id);
}
//-------------------------------------------------------------------------------------
bool SpaceFuckery::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
  return BaseApplication::mouseReleased(arg, id);
}

bool SpaceFuckery::go()
{
#ifdef _DEBUG
  mResourcesCfg = "etc/resources_d.cfg";
  mPluginsCfg = "etc/plugins_d.cfg";
#else
  mResourcesCfg = "etc/resources.cfg";
  mPluginsCfg = "etc/plugins.cfg";
#endif

  mRoot = new Ogre::Root(mPluginsCfg);

  Ogre::ConfigFile cf;
  cf.load(mResourcesCfg);

  Ogre::String name, locType;
  Ogre::ConfigFile::SectionIterator secIt = cf.getSectionIterator();
  while (secIt.hasMoreElements())
  {
    Ogre::ConfigFile::SettingsMultiMap* settings = secIt.getNext();
    Ogre::ConfigFile::SettingsMultiMap::iterator it;
    for (it = settings->begin(); it != settings->end(); ++it)
    {
      locType = it->first;
      name = it->second;
      Ogre::ResourceGroupManager::getSingleton().addResourceLocation(name, locType);
    }
  }

  if(!(mRoot->restoreConfig() || mRoot->showConfigDialog()))
    return false;
  mWindow = mRoot->initialise(true, "SpaceFuckery Render Window");

  mRenderer = &CEGUI::OgreRenderer::bootstrapSystem();
  CEGUI::ImageManager::setImagesetDefaultResourceGroup("Imagesets");
  CEGUI::Font::setDefaultResourceGroup("Fonts");
  CEGUI::Scheme::setDefaultResourceGroup("Schemes");
  CEGUI::WidgetLookManager::setDefaultResourceGroup("LookNFeel");
  CEGUI::WindowManager::setDefaultResourceGroup("Layouts");
  CEGUI::SchemeManager::getSingleton().createFromFile("TaharezLook.scheme");
  CEGUI::System::getSingleton().getDefaultGUIContext().getMouseCursor().setDefaultImage("TaharezLook/MouseArrow");

  Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
  Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

  Ogre::SceneManager* mSceneMgr;
  mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);

  Ogre::Camera* mCamera;
  mCamera = mSceneMgr->createCamera("MainCam");

  mCamera->setPosition(0, 0, 80);
  mCamera->lookAt(0, 0, -300);
  mCamera->setNearClipDistance(5);

  Ogre::Viewport* vp = mWindow->addViewport(mCamera);

  vp->setBackgroundColour(Ogre::ColourValue(0,0,0));

  mCamera->setAspectRatio(
        Ogre::Real(vp->getActualWidth()) /
        Ogre::Real(vp->getActualHeight()));

  Ogre::Entity* ogreEntity = mSceneMgr->createEntity("ogrehead.mesh");

  Ogre::SceneNode* ogreNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
  ogreNode->attachObject(ogreEntity);

  mSceneMgr->setAmbientLight(Ogre::ColourValue(.5, .5, .5));

  Ogre::Light* light = mSceneMgr->createLight("MainLight");
  light->setPosition(20, 80, 50);
  Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
  OIS::ParamList pl;
  size_t windowHnd = 0;
  std::ostringstream windowHndStr;

  mWindow->getCustomAttribute("WINDOW", &windowHnd);
  windowHndStr << windowHnd;
  pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

  mInputManager = OIS::InputManager::createInputSystem( pl );
  mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, false ));
  mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, false ));
  //Set initial mouse clipping size
  windowResized(mWindow);

  //Register as a Window listener
  Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);
  mRoot->addFrameListener(this);
  mRoot->startRendering();
  return true;
}

bool SpaceFuckery::quit(const CEGUI::EventArgs &e)
{
  return true;
}

extern "C" {
int main(int argc, char *argv[])
{
  // Create application object
  SpaceFuckery app;

  try {
    app.go();
  }
  catch(Ogre::Exception& e)  {
    std::cerr << "An exception has occurred: " << e.getFullDescription().c_str() << std::endl;
  }
  return 0;
}
}
