#pragma once

#include "..\app\SimpleSprite.h"
#include "..\app\SimpleSound.h"

#include "Utils\GameViewport.h"
#include <string>

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
    void Restart();

private:
    //------------------------------------------------------------------------
  
    // Example Sprite Code....
    CGameSprite* bunnySprite;

    //background
    CGameSprite* backgroundSprite1;
    CGameSprite* backgroundSprite2;
    CGameSprite* backgroundSprite3;


    //lasers 1
    CGameSprite* greenlaser1Sprite;
    CGameSprite* bluelaser1Sprite;
    CGameSprite* darkbluelaser1Sprite;
    CGameSprite* purplelaser1Sprite;
    CGameSprite* redlaser1Sprite;
    CGameSprite* yellowlaser1Sprite;

    //lasers 2
    CGameSprite* greenlaser2Sprite;
    CGameSprite* bluelaser2Sprite;
    CGameSprite* darkbluelaser2Sprite;
    CGameSprite* purplelaser2Sprite;
    CGameSprite* redlaser2Sprite;
    CGameSprite* yellowlaser2Sprite;

    //lasers 3
    CGameSprite* greenlaser3Sprite;
    CGameSprite* bluelaser3Sprite;
    CGameSprite* darkbluelaser3Sprite;
    CGameSprite* purplelaser3Sprite;
    CGameSprite* redlaser3Sprite;
    CGameSprite* yellowlaser3Sprite;
    

    enum
    {
        ANIM_FORWARDS,
        ANIM_BACKWARDS,
        ANIM_LEFT,
        ANIM_RIGHT,
        FALL,
    };

    float m_TotalPlayTimeInSeconds = 0.f;
    //------------------------------------------------------------------------
};