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
    enum
    {
        ANIM_FORWARDS,
        ANIM_BACKWARDS,
        ANIM_LEFT,
        ANIM_RIGHT,
    };
    //------------------------------------------------------------------------

private:
    GameViewport m_GameVp;
};