#include "SpaceFuckery.h"
#include <OgreException.h>

TutorialApplication::TutorialApplication()
  : mRoot(0),
    mResourcesCfg(Ogre::StringUtil::BLANK),
    mPluginsCfg(Ogre::StringUtil::BLANK)
{
  #ifdef _DEBUG
    mResourcesCfg = "resources_d.cfg";
    mPluginsCfg = "plugins_d.cfg";
  #else
    mResourcesCfg = "resources.cfg";
    mPluginsCfg = "plugins.cfg";
  #endif

  mRoot = new Ogre::Root(mPluginsCfg);
}

TutorialApplication::~TutorialApplication()
{
  delete mRoot;
}

bool TutorialApplication::go()
{
  return true;
}

extern "C" {
  int main(int argc, char *argv[])
  {
    // Create application object
    TutorialApplication app;

    try {
      app.go();
    }
    catch(Ogre::Exception& e)  {
      std::cerr << "An exception has occurred: " << e.getFullDescription().c_str() << std::endl;
    }
    return 0;
  }
}
