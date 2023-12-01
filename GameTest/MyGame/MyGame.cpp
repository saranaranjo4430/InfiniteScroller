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
    //gameVp.AddFlag(ViewportFlags::DRAW_VIEWPORT);
    //gameVp.AddFlag(ViewportFlags::DRAW_BORDERS);
    float speed = 1.0f / 15.0f;

    //Background 
    backgroundSprite = new CGameSprite(".\\TestData\\Background3.bmp", 1, 1);

    //init lasers 1
    greenlaser1Sprite = new CGameSprite(".\\TestData\\green_laser1.bmp", 1, 1);
    bluelaser1Sprite = new CGameSprite(".\\TestData\\blue_laser1.bmp", 1, 1);
    darkbluelaser1Sprite = new CGameSprite(".\\TestData\\darkblue_laser1.bmp", 1, 1);
    purplelaser1Sprite = new CGameSprite(".\\TestData\\purple_laser1.bmp", 1, 1);
    redlaser1Sprite = new CGameSprite(".\\TestData\\red_laser1.bmp", 1, 1);
    yellowlaser1Sprite = new CGameSprite(".\\TestData\\yellow_laser1.bmp", 1, 1);

    //init lasers 2 
    greenlaser2Sprite = new CGameSprite(".\\TestData\\green_laser2.bmp", 1, 1);
    bluelaser2Sprite = new CGameSprite(".\\TestData\\blue_laser2.bmp", 1, 1);
    darkbluelaser2Sprite = new CGameSprite(".\\TestData\\darkblue_laser2.bmp", 1, 1);
    purplelaser2Sprite = new CGameSprite(".\\TestData\\purple_laser2.bmp", 1, 1);
    redlaser2Sprite = new CGameSprite(".\\TestData\\red_laser2.bmp", 1, 1);
    yellowlaser2Sprite = new CGameSprite(".\\TestData\\yellow_laser2.bmp", 1, 1);

    //init lasers 3
    greenlaser3Sprite = new CGameSprite(".\\TestData\\green_laser3.bmp", 1, 1);
    bluelaser3Sprite = new CGameSprite(".\\TestData\\blue_laser3.bmp", 1, 1);
    darkbluelaser3Sprite = new CGameSprite(".\\TestData\\darkblue_laser3.bmp", 1, 1);
    purplelaser3Sprite = new CGameSprite(".\\TestData\\purple_laser3.bmp", 1, 1);
    redlaser3Sprite = new CGameSprite(".\\TestData\\red_laser3.bmp", 1, 1);
    yellowlaser3Sprite = new CGameSprite(".\\TestData\\yellow_laser3.bmp", 1, 1);

    //Init positions of lasers 

    //green laser

    greenlaser1Sprite->position = Vector2D(0.1f, 1.f);
    greenlaser1Sprite->CreateAnimation(FALL, speed, { 0 });
    //blue laser

    bluelaser1Sprite->position = Vector2D(0.2f, 1.f);
    bluelaser1Sprite->CreateAnimation(FALL, speed, { 0 });
    //dark blue  laser

    darkbluelaser1Sprite->position = Vector2D(0.3f, 1.f);
    darkbluelaser1Sprite->CreateAnimation(FALL, speed, { 0 });
    //purple laser

    purplelaser1Sprite->position = Vector2D(0.4f, 1.f);
    purplelaser1Sprite->CreateAnimation(FALL, speed, { 0 });
    //red laser

    redlaser1Sprite->position = Vector2D(0.5f, 1.f);
    redlaser1Sprite->CreateAnimation(FALL, speed, { 0 });
    //yellow laser

    yellowlaser1Sprite->position = Vector2D(0.6f, 1.f);
    yellowlaser1Sprite->CreateAnimation(FALL, speed, { 0 });

    //------------------------------------------------------------------------
    // Example Sprite Code....
    //------------------------------------------------------------------------
    testSprite = new CGameSprite(".\\TestData\\Test.bmp", 8, 4);
    testSprite->position = Vector2D(0.f, 0.f);
    testSprite->scale = Vector2D(1.f, 1.f);
    testSprite->SetCollision(CollisionType::BOX, Vector2D(0.5f, 1.f));
    testSprite->AddFlag(GameSpriteFlags::DRAW_COLLISION);

    testSprite->CreateAnimation(ANIM_BACKWARDS, speed, { 0,1,2,3,4,5,6,7 });
    testSprite->CreateAnimation(ANIM_LEFT, speed, { 8,9,10,11,12,13,14,15 });
    testSprite->CreateAnimation(ANIM_RIGHT, speed, { 16,17,18,19,20,21,22,23 });
    testSprite->CreateAnimation(ANIM_FORWARDS, speed, { 24,25,26,27,28,29,30,31 });

    App::LoadSound(".\\TestData\\Test.wav");

    //------------------------------------------------------------------------
    // Example of vectors....
    //------------------------------------------------------------------------
    m_RotatingRect = Square2D(Vector2D(0.5f, 0.75f), 0.5f);
    m_MovingRect = Rect2D(Vector2D(0.5f, 0.25f), 0.25f, 0.125f * gameVp.GetRatio());
    m_MovingCircle = Circle2D(Vector2D(0.75f, 0.5f), 0.125f);
    m_StaticCircle = Circle2D(Vector2D(0.25f, 0.5f), 0.125f);

    m_RotatingRect.Scale(0.5f, 0.5f);
}

void MyGame::Update(float _deltaTime)
{
    gameVp.Update(_deltaTime);

    float HSpeed = 0.01f;
    float VSpeed = HSpeed * gameVp.GetRatio();

    //Background
    backgroundSprite->Update(_deltaTime);

    //update lasers
    greenlaser1Sprite->Update(_deltaTime);
    bluelaser1Sprite->Update(_deltaTime);
    darkbluelaser1Sprite->Update(_deltaTime);
    purplelaser1Sprite->Update(_deltaTime);
    redlaser1Sprite->Update(_deltaTime);
    yellowlaser1Sprite->Update(_deltaTime);

    //lasers FALL
    greenlaser1Sprite->SetAnimation(FALL);
    greenlaser1Sprite->position.y -= VSpeed;
    bluelaser1Sprite->SetAnimation(FALL);
    bluelaser1Sprite->position.y -= VSpeed;
    darkbluelaser1Sprite->SetAnimation(FALL);
    darkbluelaser1Sprite->position.y -= VSpeed;
    purplelaser1Sprite->SetAnimation(FALL);
    purplelaser1Sprite->position.y -= VSpeed;
    redlaser1Sprite->SetAnimation(FALL);
    redlaser1Sprite->position.y -= VSpeed;
    yellowlaser1Sprite->SetAnimation(FALL);
    yellowlaser1Sprite->position.y -= VSpeed;

    //------------------------------------------------------------------------
    // Example Sprite Code....
    //------------------------------------------------------------------------
    testSprite->Update(_deltaTime);

    const CController* pController = Utils::GetFirstActiveController();
    if (!pController)
    {
        testSprite->SetAnimation(-1);
    }

    //------------------------------------------------------------------------
    // Example Vectors....
    //------------------------------------------------------------------------
    {
        float angle = m_RotatingRect.GetRotation();
        angle += 0.0125f * _deltaTime;
        m_RotatingRect.Rotate(angle);
    }

    //------------------------------------------------------------------------
    // Example Controls....
    //------------------------------------------------------------------------
    if (pController)
    {
        if (pController->CheckButton(XINPUT_GAMEPAD_A, true))
        {
            (m_MoveState >= 2) ? m_MoveState = 0 : m_MoveState++;
        }

        switch (m_MoveState)
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
}

void MyGame::MoveRect(float _deltaTime, const CController* _controller)
{
    Vector2D center = m_MovingRect.GetCenter();
    float angle = m_MovingRect.GetRotation();

    //Position
    float HSpeed = 0.01f;
    float VSpeed = HSpeed * gameVp.GetRatio();

    if ((_controller->GetLeftThumbStickX() > 0.5f) || _controller->CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false))
    {
        center.x += HSpeed;
    }
    if ((_controller->GetLeftThumbStickX() < -0.5f) || _controller->CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false))
    {
        center.x -= HSpeed;
    }
    if ((_controller->GetLeftThumbStickY() > 0.5f) || _controller->CheckButton(XINPUT_GAMEPAD_DPAD_UP, false))
    {
        center.y += VSpeed;
    }
    if ((_controller->GetLeftThumbStickY() < -0.5f) || _controller->CheckButton(XINPUT_GAMEPAD_DPAD_DOWN, false))
    {
        center.y -= VSpeed;
    }

    m_MovingRect.Move(center);

    //Rotation
    float RSpeed = 0.05f * _deltaTime;
    if (_controller->GetRightThumbStickX() > 0.5f)
    {
        angle -= RSpeed;
    }
    if (_controller->GetRightThumbStickX() < -0.5f)
    {
        angle += RSpeed;
    }

    m_MovingRect.Rotate(angle);
}

void MyGame::MoveCircle(float _deltaTime, const CController* _controller)
{
    Vector2D center = m_MovingCircle.GetCenter();

    //Position
    float HSpeed = 0.01f;
    float VSpeed = HSpeed * gameVp.GetRatio();

    if ((_controller->GetLeftThumbStickX() > 0.5f) || _controller->CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false))
    {
        center.x += HSpeed;
    }
    if ((_controller->GetLeftThumbStickX() < -0.5f) || _controller->CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false))
    {
        center.x -= HSpeed;
    }
    if ((_controller->GetLeftThumbStickY() > 0.5f) || _controller->CheckButton(XINPUT_GAMEPAD_DPAD_UP, false))
    {
        center.y += VSpeed;
    }
    if ((_controller->GetLeftThumbStickY() < -0.5f) || _controller->CheckButton(XINPUT_GAMEPAD_DPAD_DOWN, false))
    {
        center.y -= VSpeed;
    }

    m_MovingCircle.Move(center);
}

void MyGame::MoveSprite(float _deltaTime, const CController* _controller)
{
    float HSpeed = 0.01f;
    float VSpeed = HSpeed * gameVp.GetRatio();

    if ((_controller->GetLeftThumbStickX() > 0.5f) || _controller->CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false))
    {
        testSprite->SetAnimation(ANIM_RIGHT);
        testSprite->position.x += HSpeed;
    }
    if ((_controller->GetLeftThumbStickX() < -0.5f) || _controller->CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false))
    {
        testSprite->SetAnimation(ANIM_LEFT);
        testSprite->position.x -= HSpeed;
    }
    if ((_controller->GetLeftThumbStickY() > 0.5f) || _controller->CheckButton(XINPUT_GAMEPAD_DPAD_UP, false))
    {
        testSprite->SetAnimation(ANIM_FORWARDS);
        testSprite->position.y += VSpeed;
    }
    if ((_controller->GetLeftThumbStickY() < -0.5f) || _controller->CheckButton(XINPUT_GAMEPAD_DPAD_DOWN, false))
    {
        testSprite->SetAnimation(ANIM_BACKWARDS);
        testSprite->position.y -= VSpeed;
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
    if (_controller->GetLeftTrigger())
    {
        testSprite->angle += 1.f * _controller->GetLeftTrigger();
    }
    if (_controller->GetRightTrigger())
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

    //Background
    backgroundSprite->Render();

    //lasers 
    greenlaser1Sprite->Render();
    bluelaser1Sprite->Render();
    darkbluelaser1Sprite->Render();
    purplelaser1Sprite->Render();
    redlaser1Sprite->Render();
    yellowlaser1Sprite->Render();
    //------------------------------------------------------------------------
    // Example Sprite Code....
    //------------------------------------------------------------------------
    testSprite->Render();

    //------------------------------------------------------------------------
    // Example Line Drawing.
    //------------------------------------------------------------------------
    /*
    static float a = 0.0f;
    float r = 1.0f;
    float g = 1.0f;
    float b = 1.0f;
    a += 0.1f;
    for (int i = 0; i < 20; i++)
    {
        float sx = 200 + sinf(a + i * 0.1f) * 60.0f;
        float sy = 200 + cosf(a + i * 0.1f) * 60.0f;
        float ex = 700 - sinf(a + i * 0.1f) * 60.0f;
        float ey = 700 - cosf(a + i * 0.1f) * 60.0f;
        g = (float)i / 20.0f;
        b = (float)i / 20.0f;
        App::DrawLine(sx, sy, ex, ey, r, g, b);
    }
    */

    //------------------------------------------------------------------------
    // Example Vectors....
    //------------------------------------------------------------------------
    {
        switch(m_MoveState)
        {
        case 0:
            App::Print(100, 100, "Move Sprite");
            break;
        case 1:
            App::Print(100, 100, "Move Green Rect");
            break;
        case 2:
            App::Print(100, 100, "Move Yellow Circle");
            break;
        }

        App::Print(100, 75, "Press A to switch moving object");
        
        bool rectOverlapRotatingRect = m_MovingRect.Overlap(m_RotatingRect);
        bool rectOverlapStaticCircle = m_MovingRect.Overlap(m_StaticCircle);

        bool circleOverlapMovingRect = m_MovingCircle.Overlap(m_MovingRect);
        bool circleOverlapRotatingRect = m_MovingCircle.Overlap(m_RotatingRect);
        bool circleOverlapStaticCircle = m_MovingCircle.Overlap(m_StaticCircle);

        (rectOverlapRotatingRect || circleOverlapRotatingRect) ? m_RotatingRect.Draw(Utils::Color_Grey) : m_RotatingRect.Draw(Utils::Color_Red);
        (rectOverlapRotatingRect || circleOverlapMovingRect || rectOverlapStaticCircle) ? m_MovingRect.Draw(Utils::Color_Grey) : m_MovingRect.Draw(Utils::Color_Green);
        (circleOverlapMovingRect || circleOverlapRotatingRect || circleOverlapStaticCircle) ? m_MovingCircle.Draw(Utils::Color_Grey) : m_MovingCircle.Draw(Utils::Color_Yellow);
        (rectOverlapStaticCircle || circleOverlapStaticCircle) ? m_StaticCircle.Draw(Utils::Color_Grey) : m_StaticCircle.Draw(Utils::Color_White);

        float centerX = gameVp.GetX(0.5f);
        float centerY = gameVp.GetY(0.5f);

        Vector2D v1 = Vector2D(0.25f, 0.f * gameVp.GetRatio());
        Utils::DrawLine(centerX, centerY, centerX + gameVp.GetWidth(v1.x), centerY + gameVp.GetHeight(v1.y), Utils::Color_Red);

        Vector2D v2 = Vector2D(0.f, 0.25f * gameVp.GetRatio());
        Utils::DrawLine(centerX, centerY, centerX + gameVp.GetWidth(v2.x), centerY + gameVp.GetHeight(v2.y), Utils::Color_Green);

        v1.Rotate(m_RotatingRect.GetRotation());
        Utils::DrawLine(centerX, centerY, centerX + gameVp.GetWidth(v1.x), centerY + gameVp.GetHeight(v1.y), Utils::Color_White);
    }
}

void MyGame::Shutdown()
{
    //Background
    delete backgroundSprite;
    //Laser
    delete greenlaser1Sprite;
    delete bluelaser1Sprite;
    delete darkbluelaser1Sprite;
    delete purplelaser1Sprite;
    delete redlaser1Sprite;
    delete yellowlaser1Sprite;

    //------------------------------------------------------------------------
    // Example Sprite Code....
    delete testSprite;
    //------------------------------------------------------------------------

    gameVp.Shutdown();
}