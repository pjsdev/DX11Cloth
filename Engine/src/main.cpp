////////////////////////////////////////////////////////////////////////////////
// Filename: main.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Engine.h"


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	Engine* system;
	bool result;
	
	
	// Create the system object.
	system = new Engine;
	if(!system)
	{
		return 0;
	}

	// Initialize and run the system object.
	result = system->initialize();
	if(result)
	{
		system->run();
	}

	// Shutdown and release the system object.
	system->shutdown();
	delete system;
	system = 0;

	return 0;
}