#pragma once

#include "Utils\GameViewport.h"
#include "Utils\Vectors.h"

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
    //------------------------------------------------------------------------

    //------------------------------------------------------------------------
    // Example Vectors....
    Square2D m_RotatingRect;
    Rect2D m_MovingRect;
    Circle2D m_MovingCircle;
    Circle2D m_StaticCircle;
    //------------------------------------------------------------------------

    //------------------------------------------------------------------------
    // Example Controls....
    int m_MoveState = 0;

    void MoveRect(float _deltaTime, const CController* _controller);
    void MoveCircle(float _deltaTime, const CController* _controller);
    void MoveSprite(float _deltaTime, const CController* _controller);
    //------------------------------------------------------------------------
};