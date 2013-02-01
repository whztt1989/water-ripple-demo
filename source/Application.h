#include "BaseApplication.h"

#include "SinbadCharacterController.h"

class RippleSimulation;

namespace Ogre
{
    class SceneManager;
    class Viewport;
    class Camera;
}

class Application : public BaseApplication
{
public:
	Application();
	~Application();
	
protected:
	virtual void createScene();
    virtual void destroyScene();

	virtual bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	
	Ogre::SceneManager* mSceneMgr;
	Ogre::Viewport* mViewport;
	Ogre::Camera* mCamera;

    RippleSimulation* mRippleSimulation;

    SinbadCharacterController* mCharacterController;

	virtual bool mouseMoved(const OIS::MouseEvent&);
	virtual bool mousePressed(const OIS::MouseEvent&, OIS::MouseButtonID);
	virtual bool mouseReleased(const OIS::MouseEvent&, OIS::MouseButtonID);
	virtual bool keyPressed(const OIS::KeyEvent&);
	virtual bool keyReleased(const OIS::KeyEvent&);

    bool mRipplesEnabled;
};
