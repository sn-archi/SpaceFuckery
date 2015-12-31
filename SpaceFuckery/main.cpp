#include "SpaceFuckery.h"

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
