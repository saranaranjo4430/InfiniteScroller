//------------------------------------------------------------------------
#include <stdafx.h>

#include "..\app\app.h"

#include "Utils\Utils.h"
#include "Utils\GameSprite.h"

#include "MyGame.h"
//------------------------------------------------------------------------

MyGame::MyGame() 
{
}

MyGame::~MyGame()
{
}

void MyGame::Init(int width, int height)
{
    gameVp.Init(width, height);
    gameVp.AddFlag(ViewportFlags::DRAW_VIEWPORT);
    //m_GameVp.AddFlag(ViewportFlags::DRAW_BORDERS);
}

void MyGame::Update(float _deltaTime)
{
    gameVp.Update(_deltaTime);
}

void MyGame::Render()
{
    gameVp.Render();
}

void MyGame::Shutdown()
{
    gameVp.Shutdown();
}