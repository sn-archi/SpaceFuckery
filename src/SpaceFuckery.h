#include <OgreRoot.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>
#include <OgreWindowEventUtilities.h>
#include "OgreRenderWindow.h"


#include <CEGUI/CEGUI.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>

class SpaceFuckery:
    public Ogre::WindowEventListener,
    public Ogre::FrameListener,
    public OIS::KeyListener,
    public OIS::MouseListener
{
  public:
    SpaceFuckery();
    virtual ~SpaceFuckery();
    bool go();
  private:
    Ogre::Root* mRoot;
    Ogre::String mResourcesCfg;
    Ogre::String mPluginsCfg;
    Ogre::RenderWindow* mWindow;
    OIS::InputManager* mInputManager;
    OIS::Mouse* mMouse;
    OIS::Keyboard* mKeyboard;
    CEGUI::OgreRenderer* mRenderer;
    bool mShutDown;
    virtual bool loadRessources(Ogre::String Cfg);
    virtual void windowResized(Ogre::RenderWindow* rw);
    virtual void windowClosed(Ogre::RenderWindow* rw);
    // Ogre::FrameListener
    virtual void createFrameListener(void);
    virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
    // OIS::KeyListener
    virtual bool keyPressed( const OIS::KeyEvent &arg );
    virtual bool keyReleased( const OIS::KeyEvent &arg );
    // OIS::MouseListener
    virtual bool mouseMoved( const OIS::MouseEvent &arg );
    virtual bool mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual bool mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id );
    virtual void createGUI(void);
    virtual void createScene(void);
    virtual bool startRendering(void);
    virtual bool quit(const CEGUI::EventArgs &e);
};
