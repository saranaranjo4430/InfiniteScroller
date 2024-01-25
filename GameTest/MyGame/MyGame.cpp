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

    canBunnyMove = true;
    collitionFlag = false; 

    //Background 
    backgroundSprite1 = new CGameSprite(".\\TestData\\Backgroundmasgrande.bmp", 1, 1);
    backgroundSprite1->CreateAnimation(FALL, speed, { 0 });
    backgroundSprite2 = new CGameSprite(".\\TestData\\Backgroundmasgrande.bmp", 1, 1);
    backgroundSprite2->CreateAnimation(FALL, speed, { 0 });

    //position background init
    backgroundSprite1->position = Vector2D(0.5f, 0.5f);
    backgroundSprite2->position = Vector2D(0.5f, 1.47f);

    //Init positions of lasers 
    
    //green laser
    greenlaser1Sprite = InitLaser(".\\TestData\\green_laser1.bmp", Vector2D(-10.f, -10.f));
    greenlaser2Sprite = InitLaser(".\\TestData\\green_laser2.bmp", Vector2D(-10.f, -10.f));
    greenlaser3Sprite = InitLaser(".\\TestData\\green_laser3.bmp", Vector2D(-10.f, -10.f));

    bluelaser1Sprite = InitLaser(".\\TestData\\blue_laser1.bmp", Vector2D(-10.f, -10.f));
    bluelaser2Sprite = InitLaser(".\\TestData\\blue_laser2.bmp", Vector2D(-10.f, -10.f));
    bluelaser3Sprite = InitLaser(".\\TestData\\blue_laser3.bmp", Vector2D(-10.f, -10.f));

    darkbluelaser1Sprite = InitLaser(".\\TestData\\darkblue_laser1.bmp", Vector2D(-10.f, -10.f));
    darkbluelaser2Sprite = InitLaser(".\\TestData\\darkblue_laser2.bmp", Vector2D(-10.f, -10.f));
    darkbluelaser3Sprite = InitLaser(".\\TestData\\darkblue_laser3.bmp", Vector2D(-10.f, -10.f));

    purplelaser1Sprite = InitLaser(".\\TestData\\purple_laser1.bmp", Vector2D(-10.f, -10.f));
    purplelaser2Sprite = InitLaser(".\\TestData\\purple_laser2.bmp", Vector2D(-10.f, -10.f));
    purplelaser3Sprite = InitLaser(".\\TestData\\purple_laser3.bmp", Vector2D(-10.f, -10.f));

    redlaser1Sprite = InitLaser(".\\TestData\\red_laser1.bmp", Vector2D(-10.f, -10.f));
    redlaser2Sprite = InitLaser(".\\TestData\\red_laser2.bmp", Vector2D(-10.f, -10.f));
    redlaser3Sprite = InitLaser(".\\TestData\\red_laser3.bmp", Vector2D(-10.f, -10.f));

    yellowlaser1Sprite = InitLaser(".\\TestData\\yellow_laser1.bmp", Vector2D(-10.f, -10.f));
    yellowlaser2Sprite = InitLaser(".\\TestData\\yellow_laser2.bmp", Vector2D(-10.f, -10.f));
    yellowlaser3Sprite = InitLaser(".\\TestData\\yellow_laser3.bmp", Vector2D(-10.f, -10.f));

    // Init Bunny 
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

#ifdef _DEBUG
    /*
    //Collisions
    greenlaser1Sprite->AddFlag(DRAW_COLLISION);
    greenlaser2Sprite->AddFlag(DRAW_COLLISION);
    greenlaser3Sprite->AddFlag(DRAW_COLLISION);

    bluelaser1Sprite->AddFlag(DRAW_COLLISION);
    bluelaser2Sprite->AddFlag(DRAW_COLLISION);
    bluelaser3Sprite->AddFlag(DRAW_COLLISION);

    darkbluelaser1Sprite->AddFlag(DRAW_COLLISION);
    darkbluelaser2Sprite->AddFlag(DRAW_COLLISION);
    darkbluelaser3Sprite->AddFlag(DRAW_COLLISION);

    purplelaser1Sprite->AddFlag(DRAW_COLLISION);
    purplelaser2Sprite->AddFlag(DRAW_COLLISION);
    purplelaser3Sprite->AddFlag(DRAW_COLLISION);

    redlaser1Sprite->AddFlag(DRAW_COLLISION);
    redlaser2Sprite->AddFlag(DRAW_COLLISION);
    redlaser3Sprite->AddFlag(DRAW_COLLISION);

    yellowlaser1Sprite->AddFlag(DRAW_COLLISION);
    yellowlaser2Sprite->AddFlag(DRAW_COLLISION);
    yellowlaser3Sprite->AddFlag(DRAW_COLLISION);

    bunnySprite->AddFlag(DRAW_COLLISION);
    */
#endif
}

CGameSprite* MyGame::InitLaser(const std::string& filePath, const Vector2D& initialPosition) {
    CGameSprite* laserSprite = new CGameSprite(filePath.c_str(), 1, 1);
    laserSprite->CreateAnimation(FALL, 1.0f / 15.0f, { 0 });
    laserSprite->SetAnimation(FALL);
    laserSprite->position = initialPosition;
    return laserSprite;
}

void MyGame::Update(float _deltaTime)
{
    gameVp.Update(_deltaTime);

    float dtInSeconds = _deltaTime / 1000.f;

    m_TotalPlayTimeInSeconds += dtInSeconds;
    
    float HSpeed = 0.f;
    float VSpeed = 0.f;
    
    if (canBunnyMove)
    {
        HSpeed = 0.75f * dtInSeconds;
        VSpeed = HSpeed * gameVp.GetRatio();
    }

    // Assuming the height of the background image is the same as the viewport height
    float backgroundHeight = 1.6f; 

    //Background
    backgroundSprite1->SetAnimation(FALL);
    backgroundSprite2->SetAnimation(FALL);

    backgroundSprite1->Update(_deltaTime);
    backgroundSprite2->Update(_deltaTime);

    backgroundSprite1->position.y -= VSpeed;
    backgroundSprite2->position.y -= VSpeed;

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

    
    //update lasers
    greenlaser1Sprite->Update(_deltaTime);
    greenlaser2Sprite->Update(_deltaTime);
    greenlaser3Sprite->Update(_deltaTime);

    bluelaser1Sprite->Update(_deltaTime);
    bluelaser2Sprite->Update(_deltaTime);
    bluelaser3Sprite->Update(_deltaTime);

    darkbluelaser1Sprite->Update(_deltaTime);
    darkbluelaser2Sprite->Update(_deltaTime);
    darkbluelaser3Sprite->Update(_deltaTime);

    purplelaser1Sprite->Update(_deltaTime);
    purplelaser2Sprite->Update(_deltaTime);
    purplelaser3Sprite->Update(_deltaTime);
        
    redlaser1Sprite->Update(_deltaTime);
    redlaser2Sprite->Update(_deltaTime);
    redlaser3Sprite->Update(_deltaTime);

    yellowlaser1Sprite->Update(_deltaTime);
    yellowlaser2Sprite->Update(_deltaTime);
    yellowlaser3Sprite->Update(_deltaTime);

    
    //lasers FALL
    greenlaser3Sprite->position.y -= VSpeed;
    greenlaser1Sprite->position.y -= VSpeed;
    greenlaser2Sprite->position.y -= VSpeed;

    bluelaser1Sprite->position.y -= VSpeed;
    bluelaser2Sprite->position.y -= VSpeed;
    bluelaser3Sprite->position.y -= VSpeed;

    darkbluelaser1Sprite->position.y -= VSpeed;
    darkbluelaser2Sprite->position.y -= VSpeed;
    darkbluelaser3Sprite->position.y -= VSpeed;

    purplelaser1Sprite->position.y -= VSpeed;
    purplelaser2Sprite->position.y -= VSpeed;
    purplelaser3Sprite->position.y -= VSpeed;

    redlaser1Sprite->position.y -= VSpeed;
    redlaser2Sprite->position.y -= VSpeed;
    redlaser3Sprite->position.y -= VSpeed;

    yellowlaser1Sprite->position.y -= VSpeed;
    yellowlaser2Sprite->position.y -= VSpeed;
    yellowlaser3Sprite->position.y -= VSpeed;

    //Reset positions 
    ResetLaserPosition(greenlaser1Sprite);
    ResetLaserPosition(greenlaser2Sprite);
    ResetLaserPosition(greenlaser3Sprite);

    ResetLaserPosition(bluelaser1Sprite);
    ResetLaserPosition(bluelaser2Sprite);
    ResetLaserPosition(bluelaser3Sprite);

    ResetLaserPosition(darkbluelaser1Sprite);
    ResetLaserPosition(darkbluelaser2Sprite);
    ResetLaserPosition(darkbluelaser3Sprite);

    ResetLaserPosition(purplelaser1Sprite);
    ResetLaserPosition(purplelaser2Sprite);
    ResetLaserPosition(purplelaser3Sprite);

    ResetLaserPosition(redlaser1Sprite);
    ResetLaserPosition(redlaser2Sprite);
    ResetLaserPosition(redlaser3Sprite);

    ResetLaserPosition(yellowlaser1Sprite);
    ResetLaserPosition(yellowlaser2Sprite);
    ResetLaserPosition(yellowlaser3Sprite);

    // Difficulty handling
    if (m_TotalPlayTimeInSeconds <= 30.f)
    {
        greenlaser3Sprite->position.y = -10.f;
        greenlaser2Sprite->position.y = -10.f;
        bluelaser3Sprite->position.y = -10.f;
        bluelaser1Sprite->position.y = -10.f;
        darkbluelaser3Sprite->position.y = -10.f;
        darkbluelaser2Sprite->position.y = -10.f;
        darkbluelaser1Sprite->position.y = -10.f;
        purplelaser3Sprite->position.y = -10.f;
        purplelaser2Sprite->position.y = -10.f;
        redlaser3Sprite->position.y = -10.f;
        redlaser1Sprite->position.y = -10.f;
        yellowlaser3Sprite->position.y = -10.f;
        yellowlaser1Sprite->position.y = -10.f;
        yellowlaser2Sprite->position.y = -10.f;
    }
    else if (m_TotalPlayTimeInSeconds <= 60.f)
    {
        darkbluelaser3Sprite->position.y = -10.f;
        greenlaser2Sprite->position.y = -10.f;
        greenlaser3Sprite->position.y = -10.f;
        darkbluelaser1Sprite->position.y = -10.f;
        purplelaser3Sprite->position.y = -10.f;
        purplelaser1Sprite->position.y = -10.f;
        purplelaser2Sprite->position.y = -10.f;
        redlaser3Sprite->position.y = -10.f;
        redlaser1Sprite->position.y = -10.f;
        yellowlaser3Sprite->position.y = -10.f;
        yellowlaser1Sprite->position.y = -10.f;
    }
    else if (m_TotalPlayTimeInSeconds <= 90.f)
    {
        bluelaser1Sprite->position.y = -10.f;
        darkbluelaser3Sprite->position.y = -10.f;
        darkbluelaser2Sprite->position.y = -10.f;
        purplelaser3Sprite->position.y = -10.f;
        redlaser3Sprite->position.y = -10.f;
        yellowlaser3Sprite->position.y = -10.f;
        yellowlaser1Sprite->position.y = -10.f;
    }
    else if (m_TotalPlayTimeInSeconds <= 120.f)
    {
        redlaser2Sprite->position.y = -10.f;
        yellowlaser3Sprite->position.y = -10.f;
        darkbluelaser1Sprite->position.y = -10.f;
        purplelaser3Sprite->position.y = -10.f;
        purplelaser1Sprite->position.y = -10.f;
        purplelaser2Sprite->position.y = -10.f;
    }
    else if (m_TotalPlayTimeInSeconds <= 150.f)
    {
        yellowlaser3Sprite->position.y = -10.f;
        darkbluelaser1Sprite->position.y = -10.f;
        purplelaser3Sprite->position.y = -10.f;
        purplelaser1Sprite->position.y = -10.f;
        purplelaser2Sprite->position.y = -10.f;
    }
    
    bunnySprite->Update(_deltaTime);

    const CController* pController = Utils::GetFirstActiveController();

    if (canBunnyMove) 
    {
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

            // Borders bunny 
            if (bunnySprite->position.x <= 0.3f) {
                bunnySprite->position.x = 0.3f;
            }
            else if (bunnySprite->position.x >= 0.7f) {
                bunnySprite->position.x = 0.7f;
            }
            if (bunnySprite->position.y >= 1.f) {
                bunnySprite->position.y = 1.f;
            }
            else if (bunnySprite->position.y <= 0.f) {
                bunnySprite->position.y = 0.f;
            }
        }
        else
        {
            bunnySprite->SetAnimation(-1);
        }

        //Collisions
        if (bunnySprite->Overlap(greenlaser1Sprite) || bunnySprite->Overlap(greenlaser2Sprite) || bunnySprite->Overlap(greenlaser3Sprite) 
            || bunnySprite->Overlap(bluelaser3Sprite) || bunnySprite->Overlap(bluelaser2Sprite) || bunnySprite->Overlap(bluelaser1Sprite)
            || bunnySprite->Overlap(darkbluelaser1Sprite) || bunnySprite->Overlap(darkbluelaser2Sprite) || bunnySprite->Overlap(darkbluelaser3Sprite)
            || bunnySprite->Overlap(purplelaser1Sprite) || bunnySprite->Overlap(purplelaser2Sprite) || bunnySprite->Overlap(purplelaser3Sprite)
            || bunnySprite->Overlap(redlaser1Sprite) || bunnySprite->Overlap(redlaser2Sprite) || bunnySprite->Overlap(redlaser3Sprite)
            || bunnySprite->Overlap(yellowlaser1Sprite) || bunnySprite->Overlap(yellowlaser2Sprite) || bunnySprite->Overlap(yellowlaser3Sprite))
        {
            collitionFlag = true;
            canBunnyMove = false;

            if (collitionFlag == true)
            {
                bunnySprite->SetAnimation(-1);
            }
        }
    }
    else
    {
        if (pController && pController->CheckButton(XINPUT_GAMEPAD_A, true))
        {
            Restart();
        }
    }   
}


void MyGame::ResetLaserPosition(CGameSprite* laserSprite) {
    if (laserSprite && laserSprite->position.y <= -0.07f) {
        laserSprite->position.x = Utils::Random(0.3f, 0.7f);
        laserSprite->position.y = Utils::Random(1.1f, 2.f);
    }
}


void MyGame::Render()
{
    gameVp.Render();

    //for rectangle 
    Vector2D centerRectangle = Vector2D(0.5f, 0.5f);
    float widthRectagle = 0.5f;
    float heightRectagle = 0.1f;
    Rect2D myRectagle(centerRectangle, widthRectagle, heightRectagle);
    

    //Background
    backgroundSprite1->Render();
    backgroundSprite2->Render();
    
    //Lasers
    greenlaser3Sprite->Render();
    greenlaser1Sprite->Render();
    greenlaser2Sprite->Render();

    bluelaser1Sprite->Render();
    bluelaser2Sprite->Render();
    bluelaser3Sprite->Render();

    darkbluelaser1Sprite->Render();
    darkbluelaser2Sprite->Render();
    darkbluelaser3Sprite->Render();

    purplelaser1Sprite->Render();
    purplelaser2Sprite->Render();
    purplelaser3Sprite->Render();

    redlaser1Sprite->Render();
    redlaser2Sprite->Render();
    redlaser3Sprite->Render();

    yellowlaser1Sprite->Render();
    yellowlaser2Sprite->Render();
    yellowlaser3Sprite->Render();

    //Bunny
    bunnySprite->Render();

    if (!canBunnyMove)
    {
        myRectagle.Draw(Utils::Color(1.0f, 1.0f, 0.0f, 1.0f));
        App::Print(gameVp.GetX(0.36f), gameVp.GetY(0.5f), "Game Over!", 1, 0, 0, GLUT_BITMAP_TIMES_ROMAN_24);
        App::Print(gameVp.GetX(0.36f), gameVp.GetY(0.47f), "To restart press Space", 1, 0, 0, GLUT_BITMAP_HELVETICA_12);
    }

    char textBuffer[64];
    sprintf(textBuffer, "PlayTime: %.0fs", m_TotalPlayTimeInSeconds);
    
    App::Print(gameVp.GetX(0.1f), gameVp.GetY(0.06f), textBuffer, 0.0f, 0.0f, 1.0f, GLUT_BITMAP_TIMES_ROMAN_10);

    //To be called last
    gameVp.RenderBorders();
}

void MyGame::Shutdown()
{
    //Background
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

    delete bunnySprite;

    App::ReleaseSound(".\\TestData\\music.wav");

    gameVp.Shutdown();
}

void MyGame::Restart()
{
    canBunnyMove = true;
    collitionFlag = false;
    m_TotalPlayTimeInSeconds = 0.f;

    bunnySprite->position = Vector2D(0.5f, 0.25f);
    bunnySprite->scale = Vector2D(1.f, 1.f);

    backgroundSprite1->position = Vector2D(0.5f, 0.5f);
    backgroundSprite2->position = Vector2D(0.5f, 1.47f);

    greenlaser1Sprite->position = Vector2D(-10.f, -10.f);
    greenlaser2Sprite->position = Vector2D(-10.f, -10.f);
    greenlaser3Sprite->position = Vector2D(-10.f, -10.f);

    bluelaser1Sprite->position = Vector2D(-10.f, -10.f);
    bluelaser2Sprite->position = Vector2D(-10.f, -10.f);
    bluelaser3Sprite->position = Vector2D(-10.f, -10.f);

    darkbluelaser1Sprite->position = Vector2D(-10.f, -10.f);
    darkbluelaser2Sprite->position = Vector2D(-10.f, -10.f);
    darkbluelaser3Sprite->position = Vector2D(-10.f, -10.f);

    purplelaser1Sprite->position = Vector2D(-10.f, -10.f);
    purplelaser2Sprite->position = Vector2D(-10.f, -10.f);
    purplelaser3Sprite->position = Vector2D(-10.f, -10.f);

    redlaser1Sprite->position = Vector2D(-10.f, -10.f);
    redlaser2Sprite->position = Vector2D(-10.f, -10.f);
    redlaser3Sprite->position = Vector2D(-10.f, -10.f);

    yellowlaser1Sprite->position = Vector2D(-10.f, -10.f);
    yellowlaser2Sprite->position = Vector2D(-10.f, -10.f);
    yellowlaser3Sprite->position = Vector2D(-10.f, -10.f);
}


