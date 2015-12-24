#include "SpaceFuckery.h"

#include <OgreException.h>
#include <OgreConfigFile.h>

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
