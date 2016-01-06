#include "Application.h"

extern "C" {
  int main (int argc, char *argv[])
  {
    // Create the  singleton application object
    new SpaceFuckery::Application;

    try
      {
        SpaceFuckery::Application::getSingleton().go();
      }
    catch (Ogre::Exception& e)
      {
        std::cerr << "An exception has occurred: " << e.getFullDescription().c_str() << std::endl;
      }
    return 0;
  }
}
