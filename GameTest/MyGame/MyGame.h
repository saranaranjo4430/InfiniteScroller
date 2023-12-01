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
    enum
    {
        ANIM_FORWARDS,
        ANIM_BACKWARDS,
        ANIM_LEFT,
        ANIM_RIGHT,
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