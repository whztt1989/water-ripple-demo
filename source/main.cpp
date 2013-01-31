#include "Application.h"

#include <OgrePlatform.h>
#include <OgreException.h>
#include <OISException.h>

int main(int argc, char* argv[])
{	
	Application app;
	
	try
	{
		app.run();
	}
	catch (Ogre::Exception& e)
	{
		std::cerr << e.getFullDescription().c_str() << std::endl;
	}
	catch (OIS::Exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
		
	return 0;
}
