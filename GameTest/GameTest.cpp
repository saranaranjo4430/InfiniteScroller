//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>  
//------------------------------------------------------------------------
#include "app\app.h"
//------------------------------------------------------------------------
#include "MyGame\UnitTests.h"
#include "MyGame\MyGame.h"
#include "PaperGirl\PaperGirl.h"

//------------------------------------------------------------------------
MyGame* myGame;
UnitTests* unitTests;
PaperGirl* paperGirl;

bool useUnitTests = false;
bool usePaperGirl = true;

bool firstUpdate = false;

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	if (useUnitTests)
	{
		unitTests = new UnitTests();
		unitTests->Init(432, 768);
	}
	else if (usePaperGirl)
	{
        paperGirl = new PaperGirl();
        paperGirl->Init(920, 460);
	}
	else
	{
        myGame = new MyGame();
        myGame->Init(432, 768);
	}
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	if (useUnitTests)
	{
		unitTests->Update(deltaTime);
	}
    else if (usePaperGirl)
    {
        paperGirl->Update(deltaTime);
    }
	else
	{
		myGame->Update(deltaTime);
	}

	firstUpdate = true;
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine,Print, DrawSprite.) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{	
	if (!firstUpdate)
	{
		return;
	}

	if (useUnitTests)
	{
		unitTests->Render();
	}
    else if (usePaperGirl)
    {
        paperGirl->Render();
    }
	else
	{
		myGame->Render();
	}
}
//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{	
	if (useUnitTests)
	{
		unitTests->Shutdown();
	}
    else if (usePaperGirl)
    {
        paperGirl->Shutdown();
    }
	else
	{
		myGame->Shutdown();
	}
}

