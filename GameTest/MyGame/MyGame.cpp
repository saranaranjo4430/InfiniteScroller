//------------------------------------------------------------------------
#include "stdafx.h"

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

    //------------------------------------------------------------------------
    // Example Sprite Code....
    //------------------------------------------------------------------------
    testSprite = new CGameSprite(".\\TestData\\Test.bmp", 8, 4);
    testSprite->position = Vector2D(0.25f, 0.25f);
    testSprite->scale = Vector2D(1.f, 1.f);
    testSprite->SetCollision(CollisionType::CIRCLE, Vector2D(0.5f, 1.f));
    testSprite->AddFlag(GameSpriteFlags::DRAW_COLLISION);

    float speed = 1.0f / 15.0f;
    testSprite->CreateAnimation(ANIM_BACKWARDS, speed, { 0,1,2,3,4,5,6,7 });
    testSprite->CreateAnimation(ANIM_LEFT, speed, { 8,9,10,11,12,13,14,15 });
    testSprite->CreateAnimation(ANIM_RIGHT, speed, { 16,17,18,19,20,21,22,23 });
    testSprite->CreateAnimation(ANIM_FORWARDS, speed, { 24,25,26,27,28,29,30,31 });

    App::LoadSound(".\\TestData\\Test.wav");
    
    //------------------------------------------------------------------------
    // Example of vectors....
    //------------------------------------------------------------------------
    m_RotatingRect = Square2D(Vector2D(0.5f, 0.75f), 0.5f);
    m_MovingRect = Rect2D(Vector2D(0.75f, 0.25f), 0.25f, 0.125f * gameVp.GetRatio());
    m_MovingCircle = Circle2D(Vector2D(0.75f, 0.5f), 0.125f);
    m_StaticCircle = Circle2D(Vector2D(0.25f, 0.5f), 0.125f);

    m_RotatingRect.Scale(0.5f);
}

void MyGame::Update(float _deltaTime)
{
    gameVp.Update(_deltaTime);

    //------------------------------------------------------------------------
    // Example Sprite Code....
    //------------------------------------------------------------------------
    testSprite->Update(_deltaTime);
    
    //------------------------------------------------------------------------
    // Example Vectors....
    //------------------------------------------------------------------------
    float angle = m_RotatingRect.GetRotation();
    angle += 0.0125f * _deltaTime;
    m_RotatingRect.Rotate(angle);

    //------------------------------------------------------------------------
    // Example Controls....
    //------------------------------------------------------------------------
    const CController* pController = Utils::GetFirstActiveController();
    if (pController)
    {
        if (pController->CheckButton(XINPUT_GAMEPAD_A, true))
        {
            (m_MoveState >= 2)? m_MoveState = 0 : m_MoveState++;
        }

        switch(m_MoveState)
        {
        case 0:
            MoveSprite(_deltaTime, pController);
            break;
        
        case 1:
            MoveRect(_deltaTime, pController);
            break;

        case 2:
            MoveCircle(_deltaTime, pController);
            break;
        }
    }
    else
    {
        testSprite->SetAnimation(-1);
    }
}

void MyGame::MoveRect(float _deltaTime, const CController* _controller)
{
    Vector2D center = m_MovingRect.GetCenter();
    float angle = m_MovingRect.GetRotation();

    //Position
    float HSpeed = 0.75f * _deltaTime / 1000.f;
    float VSpeed = HSpeed * gameVp.GetRatio();

    if (_controller->GetLeftThumbStickX() > STICK_THRESHOLD || _controller->GetLeftThumbStickX() < -STICK_THRESHOLD)
    {
        center.x += HSpeed * _controller->GetLeftThumbStickX();
    }
    if (_controller->GetLeftThumbStickY() > STICK_THRESHOLD || _controller->GetLeftThumbStickY() < -STICK_THRESHOLD)
    {
        center.y += VSpeed * _controller->GetLeftThumbStickY();
    }

    m_MovingRect.Move(center);

    //Rotation
    float RSpeed = 20.f * _deltaTime / 1000.f;
    if (_controller->GetRightThumbStickX() > STICK_THRESHOLD || _controller->GetRightThumbStickX() < -STICK_THRESHOLD)
    {
        angle += RSpeed * _controller->GetRightThumbStickX();
    }

    m_MovingRect.Rotate(angle);
}

void MyGame::MoveCircle(float _deltaTime, const CController* _controller)
{
    Vector2D center = m_MovingCircle.GetCenter();
    
    //Position
    float HSpeed = 0.75f * _deltaTime / 1000.f;
    float VSpeed = HSpeed * gameVp.GetRatio();

    if (_controller->GetLeftThumbStickX() > STICK_THRESHOLD || _controller->GetLeftThumbStickX() < -STICK_THRESHOLD)
    {
        center.x += HSpeed * _controller->GetLeftThumbStickX();
    }
    if (_controller->GetLeftThumbStickY() > STICK_THRESHOLD || _controller->GetLeftThumbStickY() < -STICK_THRESHOLD)
    {
        center.y += VSpeed * _controller->GetLeftThumbStickY();
    }
    
    m_MovingCircle.Move(center);
}

void MyGame::MoveSprite(float _deltaTime, const CController* _controller)
{
    float HSpeed = 0.75f * _deltaTime / 1000.f;
    float VSpeed = HSpeed * gameVp.GetRatio();

    if (_controller->GetLeftThumbStickX() > STICK_THRESHOLD)
    {
        testSprite->SetAnimation(ANIM_RIGHT);
        testSprite->position.x += HSpeed * _controller->GetLeftThumbStickX();
    }
    if (_controller->GetLeftThumbStickX() < -STICK_THRESHOLD)
    {
        testSprite->SetAnimation(ANIM_LEFT);
        testSprite->position.x += HSpeed * _controller->GetLeftThumbStickX();
    }
    if (_controller->GetLeftThumbStickY() > STICK_THRESHOLD)
    {
        testSprite->SetAnimation(ANIM_FORWARDS);
        testSprite->position.y += VSpeed * _controller->GetLeftThumbStickY();
    }
    if (_controller->GetLeftThumbStickY() < -STICK_THRESHOLD)
    {
        testSprite->SetAnimation(ANIM_BACKWARDS);
        testSprite->position.y += VSpeed * _controller->GetLeftThumbStickY();
    }
    if (_controller->CheckButton(XINPUT_GAMEPAD_LEFT_SHOULDER, false))
    {
        testSprite->scale.x += HSpeed;
        testSprite->scale.y += HSpeed;
    }
    if (_controller->CheckButton(XINPUT_GAMEPAD_RIGHT_SHOULDER, false))
    {
        testSprite->scale.x -= VSpeed;
        testSprite->scale.y -= VSpeed;
    }
    if (_controller->GetLeftTrigger() > STICK_THRESHOLD)
    {
        testSprite->angle += 1.f * _controller->GetLeftTrigger();
    }
    if (_controller->GetRightTrigger() > STICK_THRESHOLD)
    {
        testSprite->angle -= 1.f * _controller->GetRightTrigger();
    }

    if (_controller->CheckButton(XINPUT_GAMEPAD_B, true))
    {
        App::PlaySound(".\\TestData\\Test.wav");
    }
}

void MyGame::Render()
{
    gameVp.Render();

    //------------------------------------------------------------------------
    // Example Sprite Code....
    //------------------------------------------------------------------------
    testSprite->Render();
    
    //------------------------------------------------------------------------
    // Example Text....
    //------------------------------------------------------------------------
    float x = gameVp.GetX(0.5f);
    float y = gameVp.GetY(0.95f);
    App::Print(x, y, "My Super Game");

    //------------------------------------------------------------------------
    // Example Vectors....
    //------------------------------------------------------------------------
    {
        bool rectOverlapRotatingRect = m_MovingRect.Overlap(m_RotatingRect);
        bool rectOverlapStaticCircle = m_MovingRect.Overlap(m_StaticCircle);
        
        bool circleOverlapMovingRect = m_MovingCircle.Overlap(m_MovingRect);
        bool circleOverlapRotatingRect = m_MovingCircle.Overlap(m_RotatingRect);
        bool circleOverlapStaticCircle = m_MovingCircle.Overlap(m_StaticCircle);

        bool spriteOverlapRotatingRect = testSprite->Overlap(m_RotatingRect);
        bool spriteOverlapStaticCircle = testSprite->Overlap(m_StaticCircle);
        bool spriteOverlapMovingRect = testSprite->Overlap(m_MovingRect);
        bool spriteOverlapMovingCircle = testSprite->Overlap(m_MovingCircle);

        (rectOverlapRotatingRect || circleOverlapRotatingRect || spriteOverlapRotatingRect)? m_RotatingRect.Draw(Utils::Color_Grey) : m_RotatingRect.Draw(Utils::Color_Red);
        (rectOverlapRotatingRect || circleOverlapMovingRect || rectOverlapStaticCircle || spriteOverlapMovingRect)? m_MovingRect.Draw(Utils::Color_Grey) : m_MovingRect.Draw(Utils::Color_Green);
        (circleOverlapMovingRect || circleOverlapRotatingRect || circleOverlapStaticCircle || spriteOverlapMovingCircle) ? m_MovingCircle.Draw(Utils::Color_Grey) : m_MovingCircle.Draw(Utils::Color_Yellow);
        (rectOverlapStaticCircle || circleOverlapStaticCircle || spriteOverlapStaticCircle) ? m_StaticCircle.Draw(Utils::Color_Grey) : m_StaticCircle.Draw(Utils::Color_White);

        Vector2D center = Vector2D(0.5f, 0.5f);

        Vector2D v1 = Vector2D(0.25f, 0.f * gameVp.GetRatio());
        v1.Draw(center, Utils::Color_Red);

        Vector2D v2 = Vector2D(0.f, 0.25f * gameVp.GetRatio());
        v2.Draw(center, Utils::Color_Green);

        v1.Rotate(m_RotatingRect.GetRotation());
        v1.Draw(center, Utils::Color_White);
    }
}

void MyGame::Shutdown()
{
    //------------------------------------------------------------------------
    // Example Sprite Code....
    delete testSprite;
    //------------------------------------------------------------------------

    gameVp.Shutdown();
}