// Tetris3D.cpp : Defines the entry point for the application.
//

#include <windows.h>
#include "Stdafx.h"
#include "CD3DDisplay.h"
#include "CApplication.h"

CD3DDisplay *pDisplay;
//CD3DDisplay *pDisplay;

// Runs the application
void Run();


// Windows equivalant to main()
int WINAPI WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   PSTR pCmdLine,
				   int nShowCmd)
{

	// Create and configure our display (D3D)

	pDisplay = new CD3DDisplay();

	//Create and initialise application display code with defaults, width and heigh are optional
	if(!pDisplay->InitialiseDisplay(hInstance,0,1024,768))
	{
		// On error display this message and exit
		::MessageBox(0, L"Display Init - Failed", L"Error", MB_OK);
		return 0;
	}

	// Run the simulation
	Run();

	// App is static so will be cleaned up automatically at end of programe
	return 0;
}

void Run()
{
	CApplication *application = new CApplication(pDisplay);
	application->Start();

	if(application)
	{
		delete application;
	}
}