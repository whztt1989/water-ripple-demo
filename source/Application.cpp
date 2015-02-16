#include "Application.h"

#include <OgreSceneManager.h>
#include <OgreViewport.h>
#include <OgreCamera.h>
#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreColourValue.h>

#include "RippleSimulation.h"


using namespace Ogre;

Application::Application()
    : mRipplesEnabled(true)
    , mLastPos(0,0)
{
	
}

Application::~Application()
{
	
}


void Application::createScene()
{
    Ogre::ResourceGroupManager::getSingleton ().addResourceLocation ("../media", "FileSystem");
    Ogre::ResourceGroupManager::getSingleton ().addResourceLocation ("../media/Sinbad.zip", "Zip");
    Ogre::ResourceGroupManager::getSingleton ().addResourceLocation ("../media/cubemap.zip", "Zip");
    Ogre::ResourceGroupManager::getSingleton ().initialiseAllResourceGroups ();

	mSceneMgr = Root::getSingleton().createSceneManager(ST_GENERIC);
	mSceneMgr->setAmbientLight(Ogre::ColourValue(1.f,1.f,1.f));
	
	mCamera = mSceneMgr->createCamera("Camera");
    mCamera->setNearClipDistance (0.05);
    mCamera->setFarClipDistance (5000);

    mCharacterController = new SinbadCharacterController(mCamera);
	
	mViewport = mWindow->addViewport(mCamera);
	mViewport->setBackgroundColour(ColourValue(0.2, 0.2, 0.2, 1.0));

    mRippleSimulation = new RippleSimulation(mSceneMgr);

    // Create the ground
    MeshManager::getSingleton().createPlane("ground", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,  Ogre::Plane(Ogre::Vector3(0,1,0), 0),
        5000, 5000, 1, 1, true, 1, 1000,1000, Vector3::UNIT_Z);
    Ogre::Entity* groundEntity = mSceneMgr->createEntity("ground");
    groundEntity->setMaterialName ("ground");
    Ogre::SceneNode* groundNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    groundNode->attachObject(groundEntity);

    // Create the water
    MeshManager::getSingleton().createPlane("water", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,  Ogre::Plane(Ogre::Vector3(0,1,0), 2.5),
        5000, 5000, 1, 1, true, 1, 100,100, Vector3::UNIT_Z);
    Ogre::Entity* waterEntity = mSceneMgr->createEntity("water");
    waterEntity->setMaterialName ("water");
    Ogre::SceneNode* waterNode = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    waterNode->attachObject(waterEntity);

}

void Application::destroyScene()
{
    delete mRippleSimulation;
}

bool Application::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    mOISKeyboard->capture();
    mOISMouse->capture();

    mCharacterController->addTime (evt.timeSinceLastFrame);

    if (mRipplesEnabled)
    {
        Ogre::Vector3 pos = mCharacterController->getPosition();
        Ogre::Vector2 pos2d (pos.x, pos.z);

        if (mLastPos != pos2d)
            mRippleSimulation->addImpulse(pos2d);

        mLastPos = pos2d;
        mRippleSimulation->update(evt.timeSinceLastFrame, pos2d);
    }

    std::cout << "FPS: " << mWindow->getLastFPS() << std::endl;

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
    if (event.key == OIS::KC_F)
        mWindow->writeContentsToTimestampedFile("Screenshot", ".png");
    if (event.key == OIS::KC_R)
        mRipplesEnabled = !mRipplesEnabled;
	return true;
}

bool Application::keyReleased(const OIS::KeyEvent& event)
{
    mCharacterController->injectKeyUp (event);
	return true;
}

