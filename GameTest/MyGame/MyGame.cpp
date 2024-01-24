//------------------------------------------------------------------------
#include <stdafx.h>


#include "..\app\app.h"

#include "Utils\Utils.h"
#include "Utils\GameSprite.h"
#include "Utils\GameViewport.h"
#include "Utils\Vectors.h"

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
    gameVp.AddFlag(DRAW_VIEWPORT);

    float speed = 1.0f / 15.0f;

    //Background 
    backgroundSprite1 = new CGameSprite(".\\TestData\\Backgroundmasgrande.bmp", 1, 1);
    backgroundSprite1->CreateAnimation(FALL, speed, { 0 });
    backgroundSprite2 = new CGameSprite(".\\TestData\\Backgroundmasgrande.bmp", 1, 1);
    backgroundSprite2->CreateAnimation(FALL, speed, { 0 });
    //backgroundSprite3 = new CGameSprite(".\\TestData\\Background2.bmp", 1, 1);
    //backgroundSprite3->CreateAnimation(FALL, speed, { 0 });
    
    // 
    //position background init
    backgroundSprite1->position = Vector2D(0.5f, 0.5f);
    backgroundSprite2->position = Vector2D(0.5f, 1.47f);
    //backgroundSprite3->position = Vector2D(0.5f, 2.5f);

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
    
    greenlaser1Sprite->position = Vector2D( -10.f, -10.f);
    greenlaser1Sprite->CreateAnimation(FALL, speed, { 0 });
    greenlaser1Sprite->SetAnimation(FALL);
    //blue laser
    
    bluelaser1Sprite->position = Vector2D(-10.f, -10.f);
    bluelaser1Sprite->CreateAnimation(FALL, speed, { 0 });
    bluelaser1Sprite->SetAnimation(FALL);
    //dark blue  laser
    
    darkbluelaser1Sprite->position = Vector2D(-10.f, -10.f);
    darkbluelaser1Sprite->CreateAnimation(FALL, speed, { 0 });
    darkbluelaser1Sprite->SetAnimation(FALL);
    //purple laser
    
    purplelaser1Sprite->position = Vector2D(-10.f, -10.f);
    purplelaser1Sprite->CreateAnimation(FALL, speed, { 0 });
    purplelaser1Sprite->SetAnimation(FALL);
    //red laser
    
    redlaser1Sprite->position = Vector2D(-10.f, -10.f);
    redlaser1Sprite->CreateAnimation(FALL, speed, { 0 });
    redlaser1Sprite->SetAnimation(FALL);
    //yellow laser
    
    yellowlaser1Sprite->position = Vector2D(-10.f, -10.f);
    yellowlaser1Sprite->CreateAnimation(FALL, speed, { 0 });
    yellowlaser1Sprite->SetAnimation(FALL);
    
    //------------------------------------------------------------------------
    // Example Sprite Code....
    bunnySprite = new CGameSprite(".\\TestData\\conejo.bmp", 9, 4);
    bunnySprite->position = Vector2D(0.5f, 0.25f);
    bunnySprite->scale = Vector2D(1.f, 1.f);
    
    bunnySprite->SetCollision(CollisionType::CIRCLE, Vector2D(0.75f, 1.f));

    bunnySprite->CreateAnimation(ANIM_BACKWARDS, speed, { 18,19,20,21,22,23,24,25,26 });
    bunnySprite->CreateAnimation(ANIM_LEFT, speed, { 9,10,11,12,13,14,15,16,17 });
    bunnySprite->CreateAnimation(ANIM_RIGHT, speed, { 27,28,29,30,31,32,33,34,35 });
    bunnySprite->CreateAnimation(ANIM_FORWARDS, speed, { 0,1,2,3,4,5,6,7,8 });

    App::LoadSound(".\\TestData\\music.wav");
    App::PlaySound(".\\TestData\\music.wav", true);
    App::SetVolume(".\\TestData\\music.wav", 0.75f);
    //------------------------------------------------------------------------

#ifdef _DEBUG
    //Collisions
    greenlaser1Sprite->AddFlag(DRAW_COLLISION);
    bluelaser1Sprite->AddFlag(DRAW_COLLISION);
    darkbluelaser1Sprite->AddFlag(DRAW_COLLISION);
    purplelaser1Sprite->AddFlag(DRAW_COLLISION);
    redlaser1Sprite->AddFlag(DRAW_COLLISION);
    yellowlaser1Sprite->AddFlag(DRAW_COLLISION);

    bunnySprite->AddFlag(DRAW_COLLISION);
#endif
}

void MyGame::Update(float _deltaTime)
{
    gameVp.Update(_deltaTime);

    m_TotalPlayTimeInSeconds += _deltaTime / 1000.f;
    
    float HSpeed = 0.01f;
    float VSpeed = HSpeed * gameVp.GetRatio();
    float speed = 1.0f / 15.0f;

    // Assuming the height of the background image is the same as the viewport height
    float backgroundHeight = 1.6f; // Replace with the actual height of your background image


    //Background
    backgroundSprite1->SetAnimation(FALL);
    backgroundSprite2->SetAnimation(FALL);
    //backgroundSprite3->SetAnimation(FALL);

    backgroundSprite1->Update(_deltaTime);
    backgroundSprite2->Update(_deltaTime);
    //backgroundSprite3->Update(_deltaTime);

    backgroundSprite1->position.y -= VSpeed;
    backgroundSprite2->position.y -= VSpeed;
    //backgroundSprite3->position.y -= VSpeed;

    // Reset the position of the first background image if it scrolls out of view
    if (backgroundSprite1->position.y <= -0.47f )//backgroundHeight)
    {
        backgroundSprite1->position.y = 1.49f;//backgroundHeight;
    }

    // Reset the position of the second background image if it scrolls out of view
    if (backgroundSprite2->position.y <= -0.47f) //backgroundHeight)
    {
        backgroundSprite2->position.y = 1.49f; // backgroundHeight;
    }

    /*if (backgroundSprite3->position.y <= -backgroundHeight)
    {
        backgroundSprite3->position.y = backgroundHeight;
    }*/
    
    //update lasers
    greenlaser1Sprite->Update(_deltaTime);
    bluelaser1Sprite->Update(_deltaTime);
    darkbluelaser1Sprite->Update(_deltaTime);
    purplelaser1Sprite->Update(_deltaTime);
    redlaser1Sprite->Update(_deltaTime);
    yellowlaser1Sprite->Update(_deltaTime);

    /*
    //lasers FALL
    //while (true) {
    backgroundSprite1->SetAnimation(FALL);
    //backgroundSprite1->position.y -= VSpeed;

    //backgroundSprite2->SetPosition(0.5f, 0.9f);
    backgroundSprite2->SetAnimation(FALL);
    //backgroundSprite2->position.y -= VSpeed;
    backgroundSprite1->position = Vector2D(0.5f, 1.5f);
    backgroundSprite1->CreateAnimation(FALL, speed, { 0 });
    backgroundSprite1->SetAnimation(FALL);
    //backgroundSprite1->position = Vector2D(0.5f, 1.5f);
    //}
    */

    greenlaser1Sprite->position.y -= VSpeed;
    bluelaser1Sprite->position.y -= VSpeed;
    darkbluelaser1Sprite->position.y -= VSpeed;
    purplelaser1Sprite->position.y -= VSpeed;
    redlaser1Sprite->position.y -= VSpeed;
    yellowlaser1Sprite->position.y -= VSpeed;

    if (greenlaser1Sprite->position.y <= -0.07f)
    {
        greenlaser1Sprite->position.x = Utils::Random(0.25f, 0.75f);
        greenlaser1Sprite->position.y = Utils::Random(1.1f, 2.f);
    }

    if (bluelaser1Sprite->position.y <= -0.07f)
    {
        bluelaser1Sprite->position.x = Utils::Random(0.25f, 0.75f);
        bluelaser1Sprite->position.y = Utils::Random(1.1f, 2.f);
    }

    if (darkbluelaser1Sprite->position.y <= -0.07f)
    {
        darkbluelaser1Sprite->position.x = Utils::Random(0.25f, 0.75f);
        darkbluelaser1Sprite->position.y = Utils::Random(1.1f, 2.f);
    }

    if (purplelaser1Sprite->position.y <= -0.07f)
    {
        purplelaser1Sprite->position.x = Utils::Random(0.25f, 0.75f);
        purplelaser1Sprite->position.y = Utils::Random(1.1f, 2.f);
    }

    if (redlaser1Sprite->position.y <= -0.07f)
    {
        redlaser1Sprite->position.x = Utils::Random(0.25f, 0.75f);
        redlaser1Sprite->position.y = Utils::Random(1.1f, 2.f);
    }

    if (yellowlaser1Sprite->position.y <= -0.07f)
    {
        yellowlaser1Sprite->position.x = Utils::Random(0.25f, 0.75f);
        yellowlaser1Sprite->position.y = Utils::Random(1.1f, 2.f);
    }

    // Difficulty handling
    if (m_TotalPlayTimeInSeconds <= 30.f)
    {
        bluelaser1Sprite->position.y = -10.f;
        darkbluelaser1Sprite->position.y = -10.f;
        purplelaser1Sprite->position.y = -10.f;
        redlaser1Sprite->position.y = -10.f;
        yellowlaser1Sprite->position.y = -10.f;
    }
    else if (m_TotalPlayTimeInSeconds <= 60.f)
    {
        darkbluelaser1Sprite->position.y = -10.f;
        purplelaser1Sprite->position.y = -10.f;
        redlaser1Sprite->position.y = -10.f;
        yellowlaser1Sprite->position.y = -10.f;
    }
    else if (m_TotalPlayTimeInSeconds <= 90.f)
    {
        purplelaser1Sprite->position.y = -10.f;
        redlaser1Sprite->position.y = -10.f;
        yellowlaser1Sprite->position.y = -10.f;
    }
    else if (m_TotalPlayTimeInSeconds <= 120.f)
    {
        redlaser1Sprite->position.y = -10.f;
        yellowlaser1Sprite->position.y = -10.f;
    }
    else if (m_TotalPlayTimeInSeconds <= 150.f)
    {
        yellowlaser1Sprite->position.y = -10.f;
    }
        
    //------------------------------------------------------------------------
    // Example Sprite Code....
    bunnySprite->Update(_deltaTime);

    const CController* pController = Utils::GetFirstActiveController();
    if (pController)
    {
        if ((pController->GetLeftThumbStickX() > 0.5f) || pController->CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false))
        {
            bunnySprite->SetAnimation(ANIM_RIGHT);
            bunnySprite->position.x += HSpeed;
        }
        if ((pController->GetLeftThumbStickX() < -0.5f) || pController->CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false))
        {
            bunnySprite->SetAnimation(ANIM_LEFT);
            bunnySprite->position.x -= HSpeed;
        }
        if ((pController->GetLeftThumbStickY() > 0.5f) || pController->CheckButton(XINPUT_GAMEPAD_DPAD_UP, false))
        {
            bunnySprite->SetAnimation(ANIM_FORWARDS);
            bunnySprite->position.y += VSpeed;
        }
        if ((pController->GetLeftThumbStickY() < -0.5f) || pController->CheckButton(XINPUT_GAMEPAD_DPAD_DOWN, false))
        {
            bunnySprite->SetAnimation(ANIM_BACKWARDS);
            bunnySprite->position.y -= VSpeed;
        }
        if (pController->CheckButton(XINPUT_GAMEPAD_LEFT_SHOULDER, false))
        {
            bunnySprite->scale.x += HSpeed;
        }
        if (pController->CheckButton(XINPUT_GAMEPAD_RIGHT_SHOULDER, false))
        {
            bunnySprite->scale.x -= VSpeed;
        }
        /*
        if (pController->GetLeftTrigger())
        {
            bunnySprite->angle += 0.1f * pController->GetLeftTrigger();
        }
        if (pController->GetRightTrigger())
        {
            bunnySprite->angle -= 0.1f * pController->GetRightTrigger();
        }
        */

        if (pController->CheckButton(XINPUT_GAMEPAD_A, true))
        {
            Restart();
        }
    }
    else
    {
        bunnySprite->SetAnimation(-1);
    }
    //------------------------------------------------------------------------

    //Collisions
    if (bunnySprite->Overlap(greenlaser1Sprite))
    {

    }
}

void MyGame::Render()
{
    gameVp.Render();

    //Background
    //backgroundSprite3->Render();
    backgroundSprite1->Render();
    backgroundSprite2->Render();
        
    greenlaser1Sprite->Render();
    bluelaser1Sprite->Render();
    darkbluelaser1Sprite->Render();
    purplelaser1Sprite->Render();
    redlaser1Sprite->Render();
    yellowlaser1Sprite->Render();

    //------------------------------------------------------------------------
    // Example Sprite Code....
    bunnySprite->Render();
    //------------------------------------------------------------------------

    //------------------------------------------------------------------------
    // Example Text.
    //------------------------------------------------------------------------
    App::Print(gameVp.GetX(0.5f), gameVp.GetY(0.5f), "Sample Text", 1,0,0, GLUT_BITMAP_TIMES_ROMAN_24);

    char textBuffer[64];
    sprintf(textBuffer, "PlayTime: %.0fs", m_TotalPlayTimeInSeconds);
    App::Print(gameVp.GetX(0.5f), gameVp.GetX(0.95f), textBuffer, 0.0f, 0.0f, 1.0f, GLUT_BITMAP_HELVETICA_10);

    //To be called last
    gameVp.RenderBorders();
}

void MyGame::Shutdown()
{
    //Background
    //delete backgroundSprite3;
    delete backgroundSprite1;
    delete backgroundSprite2;
    
    //Laser
    delete greenlaser1Sprite;
    delete bluelaser1Sprite;
    delete darkbluelaser1Sprite;
    delete purplelaser1Sprite;
    delete redlaser1Sprite;
    delete yellowlaser1Sprite;

    delete greenlaser2Sprite;
    delete bluelaser2Sprite;
    delete darkbluelaser2Sprite;
    delete purplelaser2Sprite;
    delete redlaser2Sprite;
    delete yellowlaser2Sprite;

    delete greenlaser3Sprite;
    delete bluelaser3Sprite;
    delete darkbluelaser3Sprite;
    delete purplelaser3Sprite;
    delete redlaser3Sprite;
    delete yellowlaser3Sprite;
    
    //------------------------------------------------------------------------
    // Example Sprite Code....
    delete bunnySprite;
    //------------------------------------------------------------------------

    App::ReleaseSound(".\\TestData\\music.wav");

    gameVp.Shutdown();
}

void MyGame::Restart()
{
    bunnySprite->position = Vector2D(0.5f, 0.25f);
    bunnySprite->scale = Vector2D(1.f, 1.f);

    backgroundSprite1->position = Vector2D(0.5f, 0.5f);
    backgroundSprite2->position = Vector2D(0.5f, 1.47f);

    greenlaser1Sprite->position = Vector2D(-10.f, -10.f);
    bluelaser1Sprite->position = Vector2D(-10.f, -10.f);
    darkbluelaser1Sprite->position = Vector2D(-10.f, -10.f);
    purplelaser1Sprite->position = Vector2D(-10.f, -10.f);
    redlaser1Sprite->position = Vector2D(-10.f, -10.f);
    yellowlaser1Sprite->position = Vector2D(-10.f, -10.f);
}


