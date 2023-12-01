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
    m_GameVp.Init(width, height);
    m_GameVp.AddFlag(DRAW_VIEWPORT);
    m_GameVp.AddFlag(DRAW_BORDERS);

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
    m_GameVp.Update(_deltaTime);

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
        float HSpeed = 0.01f;
        float VSpeed = HSpeed * m_GameVp.GetRatio();

        if ((pController->GetLeftThumbStickX() > 0.5f) || pController->CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false))
        {
            testSprite->SetAnimation(ANIM_RIGHT);
            testSprite->position.x += HSpeed;
        }
        if ((pController->GetLeftThumbStickX() < -0.5f) || pController->CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false))
        {
            testSprite->SetAnimation(ANIM_LEFT);
            testSprite->position.x -= HSpeed;
        }
        if ((pController->GetLeftThumbStickY() > 0.5f) || pController->CheckButton(XINPUT_GAMEPAD_DPAD_UP, false))
        {
            testSprite->SetAnimation(ANIM_FORWARDS);
            testSprite->position.y += VSpeed;
        }
        if ((pController->GetLeftThumbStickY() < -0.5f) || pController->CheckButton(XINPUT_GAMEPAD_DPAD_DOWN, false))
        {
            testSprite->SetAnimation(ANIM_BACKWARDS);
            testSprite->position.y -= VSpeed;
        }
        if (pController->CheckButton(XINPUT_GAMEPAD_LEFT_SHOULDER, false))
        {
            testSprite->scale.x += HSpeed;
        }
        if (pController->CheckButton(XINPUT_GAMEPAD_RIGHT_SHOULDER, false))
        {
            testSprite->scale.x -= VSpeed;
        }
        if (pController->GetLeftTrigger())
        {
            testSprite->angle += 0.1f * pController->GetLeftTrigger();
        }
        if (pController->GetRightTrigger())
        {
            testSprite->angle -= 0.1f * pController->GetRightTrigger();
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
    // Example Text.
    //------------------------------------------------------------------------
    //App::Print(100, 100, "Sample Text");

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
        bool rectOverlapRotatingRect = m_MovingRect.Overlap(m_RotatingRect);
        bool rectOverlapStaticCircle = m_MovingRect.Overlap(m_StaticCircle);

        bool circleOverlapMovingRect = m_MovingCircle.Overlap(m_MovingRect);
        bool circleOverlapRotatingRect = m_MovingCircle.Overlap(m_RotatingRect);
        bool circleOverlapStaticCircle = m_MovingCircle.Overlap(m_StaticCircle);

        (rectOverlapRotatingRect || circleOverlapRotatingRect)? m_RotatingRect.Draw(Utils::Color_Grey) : m_RotatingRect.Draw(Utils::Color_Red);
        (rectOverlapRotatingRect || circleOverlapMovingRect || rectOverlapStaticCircle)? m_MovingRect.Draw(Utils::Color_Grey) : m_MovingRect.Draw(Utils::Color_Green);
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