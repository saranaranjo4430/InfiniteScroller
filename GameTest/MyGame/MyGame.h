#pragma once

#include "Utils\GameViewport.h"

class CGameSprite;

class MyGame
{
public:
    MyGame();
    ~MyGame();

    void Init(int width, int height);
    void Update(float _deltaTime);
    void Render();
    void Shutdown();

private:
    //------------------------------------------------------------------------
    // Example Sprite Code....
    CGameSprite* testSprite;

    //background
    CGameSprite* backgroundSprite;

    //lasers
    CGameSprite* greenlaser1Sprite;
    CGameSprite* bluelaser1Sprite;
    CGameSprite* darkbluelaser1Sprite;
    CGameSprite* purplelaser1Sprite;
    CGameSprite* redlaser1Sprite;
    CGameSprite* yellowlaser1Sprite;

    enum
    {
        ANIM_FORWARDS,
        ANIM_BACKWARDS,
        ANIM_LEFT,
        ANIM_RIGHT,
        FALL,
    };
    //------------------------------------------------------------------------

private:
    GameViewport m_GameVp;
};