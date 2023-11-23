//------------------------------------------------------------------------
#include "stdafx.h"

#include "..\app\app.h"
#include "../stb_image/stb_image.h"
#include "../glut/include/GL/freeglut_ext.h"
#include <algorithm>

#include "Utils\Utils.h"
#include "Utils\GameSprite.h"

#include "MyGame.h"
//------------------------------------------------------------------------

MyGame::MyGame() 
  : playerSprite(nullptr),
    background(nullptr),
    isJumping(false),
    jumpSpeed(0.0f),
    gravity(0.001f),
    lane(1),
    laneWidth(200.0f),
    screenWidth(static_cast<float>(APP_INIT_WINDOW_WIDTH)),
    screenHeight(static_cast<float>(APP_INIT_WINDOW_HEIGHT)),
    gameSpeed(0.2f)
{
}

MyGame::~MyGame()
{
}

void MyGame::Init(int width, int height)
{
    
    m_GameVp.Init(width, height);
    //m_GameVp.AddFlag(DRAW_VIEWPORT);
    //m_GameVp.AddFlag(DRAW_BORDERS);

    // Load background image using stb_image
    int bgWidth, bgHeight, bgChannels;
    unsigned char* bgData = stbi_load(".\\TestData\\Background2.png", &bgWidth, &bgHeight, &bgChannels,0);
    if (bgData) {
        // Use loaded image data to create a texture for background
        GLuint bgTextureID;
        glGenTextures(1, &bgTextureID);
        glBindTexture(GL_TEXTURE_2D, bgTextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bgWidth, bgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bgData);
        //glGenerateMipmap(GL_TEXTURE_2D);

        // Set texture parameters (if needed)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Free image data after creating the texture
        stbi_image_free(bgData);

        // Now, you have bgTextureID representing your background texture
        // You can use it to render your background in the Render() function
    }

    playerSprite = new CGameSprite(m_GameVp, ".\\TestData\\Test.bmp", 8, 4);
    background = new CGameSprite(m_GameVp, ".\\TestData\\Background2.png", 1, 1);

    playerSprite->position = Vector2D(screenWidth * 0.5f, screenHeight * 0.5f);
    playerSprite->scale = Vector2D(1.f, 1.f);
    float speed = 1.0f / 15.0f;

    playerSprite->CreateAnimation(ANIM_BACKWARDS, speed, { 0,1,2,3,4,5,6,7 });
    playerSprite->CreateAnimation(ANIM_LEFT, speed, { 8,9,10,11,12,13,14,15 });
    playerSprite->CreateAnimation(ANIM_RIGHT, speed, { 16,17,18,19,20,21,22,23 });
    playerSprite->CreateAnimation(ANIM_FORWARDS, speed, { 24,25,26,27,28,29,30,31 });

    App::LoadSound(".\\TestData\\Test.wav");

    //------------------------------------------------------------------------
    // Example Sprite Code....
    /*testSprite = new CGameSprite(m_GameVp, ".\\TestData\\Test.bmp", 8, 4);
    testSprite->position = Vector2D(0.f, 0.f);
    testSprite->scale = Vector2D(1.f, 1.f);

    float speed = 1.0f / 15.0f;
    testSprite->CreateAnimation(ANIM_BACKWARDS, speed, { 0,1,2,3,4,5,6,7 });
    testSprite->CreateAnimation(ANIM_LEFT, speed, { 8,9,10,11,12,13,14,15 });
    testSprite->CreateAnimation(ANIM_RIGHT, speed, { 16,17,18,19,20,21,22,23 });
    testSprite->CreateAnimation(ANIM_FORWARDS, speed, { 24,25,26,27,28,29,30,31 });

    App::LoadSound(".\\TestData\\Test.wav");
    */
    //------------------------------------------------------------------------
}

void MyGame::Update(float _deltaTime)
{
    m_GameVp.Update(_deltaTime);

    //------------------------------------------------------------------------
    // Example Sprite Code....
    //testSprite->Update(_deltaTime);

    playerSprite->Update(_deltaTime);
    background->position.y -= gameSpeed * _deltaTime;

    // Apply gravity and handle jumping logic
    if (isJumping) {
        jumpSpeed -= gravity * _deltaTime;
        playerSprite->position.y += jumpSpeed * _deltaTime;

        if (playerSprite->position.y < screenHeight * 0.5f) {
            playerSprite->position.y = screenHeight * 0.5f;
            jumpSpeed = 0.0f;
            isJumping = false;
        }
    }

    const CController* pController = Utils::GetFirstActiveController();
    if (pController != NULL) {
        float leftThumbStickX = pController->GetLeftThumbStickX();
        if (leftThumbStickX > 0.5f) {
            lane = (lane + 1 <= 3) ? lane + 1 : 3;
        }
        else if (leftThumbStickX < -0.5f) {
            lane = (lane - 1 >= 1) ? lane - 1 : 1;
        }
        else if (pController->CheckButton(XINPUT_GAMEPAD_A, true) && !isJumping) {
            isJumping = true;
            jumpSpeed = 0.3f;
            App::PlaySound(".\\TestData\\Test.wav");
        }
    } 

/*

    const CController* pController = Utils::GetFirstActiveController();
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

        //------------------------------------------------------------------------
        // Sample Sound.
        //------------------------------------------------------------------------
        if (pController->CheckButton(XINPUT_GAMEPAD_B, true))
        {
            App::PlaySound(".\\TestData\\Test.wav");
        }
    }
    else
    {
        testSprite->SetAnimation(-1);
    }
    //------------------------------------------------------------------------
    */
    }


void MyGame::Render()
{
    m_GameVp.Render();
    
    playerSprite->Render();
    //------------------------------------------------------------------------
    // Example Sprite Code....
    //testSprite->Render();
    //------------------------------------------------------------------------

    background->Render();
    //------------------------------------------------------------------------
    // Example Text.
    //------------------------------------------------------------------------
    App::Print(100, 100, "Sample Text");

    //------------------------------------------------------------------------
    // Example Line Drawing.
    //------------------------------------------------------------------------
    //static float a = 0.0f;
    float r = 1.0f;
    float g = 1.0f;
    float b = 1.0f;
    //a += 0.1f;
    App::DrawLine(1000, 100, 300, 600, r, 0, 0);
    //for (int i = 0; i < 20; i++)
    //{

        //float sx = 200 + sinf(a + i * 0.1f) * 60.0f;
        //float sy = 200 + cosf(a + i * 0.1f) * 60.0f;
        //float ex = 700 - sinf(a + i * 0.1f) * 60.0f;
        //float ey = 700 - cosf(a + i * 0.1f) * 60.0f;
        //g = (float)i / 20.0f;
        //b = (float)i / 20.0f;
        //App::DrawLine(sx, sy, ex, ey, r, g, b);
    //}
}

void MyGame::Shutdown()
{
    //------------------------------------------------------------------------
    // Example Sprite Code....
    //delete testSprite;
    
    //------------------------------------------------------------------------

    delete playerSprite;
    delete background;
    m_GameVp.Shutdown();
}