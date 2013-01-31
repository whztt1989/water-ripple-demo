#include <OgreWindowEventUtilities.h>
#include <OgreFrameListener.h>

#include <OIS.h>

namespace Ogre { class Root; class RenderWindow; }

class Window; class InputManager;

class BaseApplication : public Ogre::WindowEventListener, public Ogre::FrameListener, public OIS::KeyListener, public OIS::MouseListener
{
public:
	BaseApplication();
	~BaseApplication();
	
	void run();
	
protected:
	void windowResized(Ogre::RenderWindow* rw);
	void windowClosed(Ogre::RenderWindow* rw);
			
	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt) = 0;

	bool mShutdown;

	Ogre::Root* mRoot;

	OIS::InputManager* mOIS;
	OIS::Mouse* mOISMouse;
	OIS::Keyboard* mOISKeyboard;

	Ogre::RenderWindow* mWindow;

	virtual void createScene() = 0;
		
	virtual bool mouseMoved(const OIS::MouseEvent&) = 0;
	virtual bool mousePressed(const OIS::MouseEvent&, OIS::MouseButtonID) = 0;
	virtual bool mouseReleased(const OIS::MouseEvent&, OIS::MouseButtonID) = 0;
	virtual bool keyPressed(const OIS::KeyEvent&) = 0;
	virtual bool keyReleased(const OIS::KeyEvent&) = 0;
};
