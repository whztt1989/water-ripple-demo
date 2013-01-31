#include "Application.h"

#include <OgreSceneManager.h>
#include <OgreViewport.h>
#include <OgreCamera.h>
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreColourValue.h>


using namespace Ogre;

Application::Application()
{
	
}

Application::~Application()
{
	
}


void Application::createScene()
{
    Ogre::ResourceGroupManager::getSingleton ().addResourceLocation ("../media", "FileSystem");
    Ogre::ResourceGroupManager::getSingleton ().addResourceLocation ("../media/Sinbad.zip", "Zip");
    Ogre::ResourceGroupManager::getSingleton ().initialiseAllResourceGroups ();

	mSceneMgr = Root::getSingleton().createSceneManager(ST_GENERIC);
	
	mCamera = mSceneMgr->createCamera("Camera");
    mCamera->setNearClipDistance (0.05);
    mCamera->setFarClipDistance (5000);

    mCharacterController = new SinbadCharacterController(mCamera);
	
	mViewport = mWindow->addViewport(mCamera);
	mViewport->setBackgroundColour(ColourValue(0.2, 0.2, 0.2, 1.0));

    // Create the ground
    MeshManager::getSingleton().createPlane("ground", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,  Ogre::Plane(Ogre::Vector3(0,1,0), 0),
        1000, 1000, 1, 1, true, 1, 10,10, Vector3::UNIT_Z);
    Ogre::Entity* groundEntity = mSceneMgr->createEntity("ground");
    groundEntity->setMaterialName ("ground");
    Ogre::SceneNode* groundNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    groundNode->attachObject(groundEntity);

    // Create the water
    MeshManager::getSingleton().createPlane("water", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,  Ogre::Plane(Ogre::Vector3(0,1,0), 1),
        1000, 1000, 1, 1, true, 1, 10,10, Vector3::UNIT_Z);
    Ogre::Entity* waterEntity = mSceneMgr->createEntity("water");
    waterEntity->setMaterialName ("water");
    Ogre::SceneNode* waterNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    waterNode->attachObject(waterEntity);

}

bool Application::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    mOISKeyboard->capture();
    mOISMouse->capture();

    mCharacterController->addTime (evt.timeSinceLastFrame);

	return !mShutdown;
}


bool Application::mouseMoved(const OIS::MouseEvent& event)
{
    mCharacterController->injectMouseMove (event);

	return true;
}

bool Application::mousePressed(const OIS::MouseEvent& event, OIS::MouseButtonID id)
{
    mCharacterController->injectMouseDown (event, id);
	return true;
}

bool Application::mouseReleased(const OIS::MouseEvent& event, OIS::MouseButtonID id)
{
	return true;
}

bool Application::keyPressed(const OIS::KeyEvent& event)
{
    mCharacterController->injectKeyDown (event);

    if (event.key == OIS::KC_ESCAPE)
		mShutdown = true;
	return true;
}

bool Application::keyReleased(const OIS::KeyEvent& event)
{
    mCharacterController->injectKeyUp (event);
	return true;
}

