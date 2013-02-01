#include "BaseApplication.h"

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OIS/OIS.h>

using namespace Ogre;

BaseApplication::BaseApplication() :
	mWindow(0), mOISMouse(0), mOISKeyboard(0), mShutdown(false)
{

}

BaseApplication::~BaseApplication()
{
	
}

void BaseApplication::run()
{	
	// Construct Ogre::Root
	// We handle our stuff manually, so don't want to use any of the files
	mRoot = new Root(
		/* plugins.cfg file*/	"",
		/* config file */ 		"", 
		/* log file */ 			""
	);
	
	// Set render system
    mRoot->loadPlugin(OGRE_PLUGIN_DIR_REL + std::string("/RenderSystem_GL"));
	RenderSystem* rs = mRoot->getRenderSystemByName("OpenGL Rendering Subsystem");
	mRoot->setRenderSystem(rs);
	
	// Fire up Ogre::Root
	mRoot->initialise(false);

	// Create the application window
	NameValuePairList settings;
	settings.insert(std::make_pair("title", "OGRE Application"));
	settings.insert(std::make_pair("FSAA", "0"));
    settings.insert(std::make_pair("vsync", "false"));

	mWindow = Root::getSingleton().createRenderWindow("OGRE Application", 800, 600, false, &settings);
	WindowEventUtilities::addWindowEventListener(mWindow, this);
	
	size_t windowID;
	mWindow->getCustomAttribute("WINDOW", &windowID);

	// Create input system
	OIS::ParamList pl;
	std::ostringstream windowHndStr;
	windowHndStr << windowID;
	pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
    #ifdef OIS_LINUX_PLATFORM
	pl.insert(std::make_pair(std::string("x11_mouse_grab"), std::string("false")));
	pl.insert(std::make_pair(std::string("x11_mouse_hide"), std::string("false")));
	pl.insert(std::make_pair(std::string("x11_keyboard_grab"), std::string("false")));
    pl.insert(std::make_pair(std::string("XAutoRepeatOn"), std::string("true")));
    #endif
	mOIS = OIS::InputManager::createInputSystem( pl );
	// Create devices
	if (mOIS->getNumberOfDevices(OIS::OISKeyboard) > 0)
	{
		mOISKeyboard = static_cast<OIS::Keyboard*>(mOIS->createInputObject(OIS::OISKeyboard, true));
		mOISKeyboard->setEventCallback(this);
	}
	if (mOIS->getNumberOfDevices(OIS::OISMouse) > 0)
	{
		mOISMouse = static_cast<OIS::Mouse*>(mOIS->createInputObject(OIS::OISMouse, true));
		mOISMouse->setEventCallback(this);
	}
		
	// Start the rendering loop
	createScene();
	mRoot->addFrameListener(this);
	mRoot->startRendering();
	
	// Shutdown
    destroyScene();
	if (mOISMouse)
		mOIS->destroyInputObject(mOISMouse);
	if (mOISKeyboard)
		mOIS->destroyInputObject(mOISKeyboard);
	mOIS->destroyInputSystem(mOIS);
	mRoot->removeFrameListener(this);
	delete mRoot;
}

void BaseApplication::windowResized(RenderWindow* rw)
{
	
}

void BaseApplication::windowClosed(RenderWindow* rw)
{
	mShutdown = true;
}
