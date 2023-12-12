//------------------------------------------------------------------------
#include "stdafx.h"

#include "..\app\app.h"

#include "..\MyGame\Utils\Utils.h"
#include "..\MyGame\Utils\GameSprite.h"
#include "..\MyGame\Utils\ScrollingBackground.h"

#include "PaperGirl.h"
//------------------------------------------------------------------------

PaperGirl::PaperGirl()
{
}

PaperGirl::~PaperGirl()
{
}

void PaperGirl::Init(int width, int height)
{
    gameVp.Init(width, height);
    gameVp.AddFlag(ViewportFlags::DRAW_VIEWPORT);
    //m_GameVp.AddFlag(ViewportFlags::DRAW_BORDERS);

    InitBackground();
    InitMainCharacter();

    ResetGame();
}

void PaperGirl::Update(float _deltaTime)
{
    m_ActiveController = Utils::GetFirstActiveController();

    gameVp.Update(_deltaTime);

    m_Background->Update(_deltaTime);

    UpdateMainCharacter(_deltaTime);

    if (m_ActiveController && m_ActiveController->CheckButton(XINPUT_GAMEPAD_START, false))
    {
        ResetGame();
    }
}

void PaperGirl::Render()
{
    gameVp.Render();

    m_Background->Render();
    m_MainCharacter->Render();

    gameVp.RenderBorders();
}

void PaperGirl::Shutdown()
{
    delete m_MainCharacter;

    m_Background->Shutdown();
    delete m_Background;

    gameVp.Shutdown();
}

void PaperGirl::InitMainCharacter()
{
    //Init Sprite
    m_MainCharacter = new CGameSprite(".\\PaperGirl\\Data\\PaperGirl.bmp", 6, 5);

    //Init Animations
    m_MainCharacter->CreateAnimation(ANIM_IDLE, 0.f, { 0 });

    m_MainCharacter->CreateAnimation(ANIM_MOVE_SLOW, 0.4f, { 6,7,8,9,10,11 });
    m_MainCharacter->CreateAnimation(ANIM_MOVE_NORMAL, 0.75f, { 6,7,8,9,10,11 });
    m_MainCharacter->CreateAnimation(ANIM_MOVE_FAST, 0.25f, { 6,7,8,9,10,11 });

    m_MainCharacter->CreateAnimation(ANIM_TRANSITION_START, 0.4f, { 0,1,2 });
    m_MainCharacter->CreateAnimation(ANIM_TRANSITION_BOOST, 0.2f, { 24,25,26 });

    m_MainCharacter->CreateAnimation(ANIM_ACTION_THROW, 1.5f, { 12,13,14,15,16,17 });
    m_MainCharacter->CreateAnimation(ANIM_ACTION_FALL, 1.5f, { 18,19 });

    m_MainCharacter->SetAnimation(ANIM_IDLE);

    //Init Collisions
    m_MainCharacter->SetCollision(CollisionType::CIRCLE, Vector2D(1.f, 1.f), Vector2D(-0.005f, -0.02f));
    m_MainCharacter->AddFlag(GameSpriteFlags::DRAW_COLLISION);
}

static float fastSpeed = 0.1f;
static float slowSpeed = 0.15f;
static float verticalSpeed = 0.1f;

static Vector2D roadRange = Vector2D(0.095f, 0.32f);

static float scrollAccSpeed = 0.005f;
static float scrollMaxSpeed = 0.04f;

void PaperGirl::UpdateMainCharacter(float _deltaTime)
{
    float FastDisp = fastSpeed * _deltaTime / 1000.f;
    float SlowDisp = slowSpeed * _deltaTime / 1000.f;
    float VDisp = verticalSpeed * _deltaTime / 1000.f;

    float XFactor = GetXInput();
    float YFactor = GetYInput();

    //Update sprite
    m_MainCharacter->Update(_deltaTime);

    //Determine animation to play
    if (!m_MainCharacter->IsPlaying(ANIM_ACTION_THROW) && !m_MainCharacter->IsPlaying(ANIM_ACTION_FALL))
    {
        //Default anim
        (m_RideStarted) ? m_MainCharacterWantedAnimState = ANIM_MOVE_NORMAL : m_MainCharacterWantedAnimState = ANIM_IDLE;

        //Process inputs
        if (m_ActiveController)
        {
            if (XFactor > 0.f)
            {
                if (!m_RideStarted)
                {
                    StartRide();
                }

                if (GetBoostLevel() > 0.f)
                {
                    m_MainCharacterWantedAnimState = ANIM_MOVE_FAST;
                }
            }
            else if (m_RideStarted && XFactor < 0.f)
            {
                m_MainCharacterWantedAnimState = ANIM_MOVE_SLOW;
            }
        }
    }

    //Update Animation
    UpdateMainCharacterAnim();

    //Update Position
    if (m_RideStarted)
    {
        m_Background->scrollSpeed += scrollAccSpeed;
        m_Background->scrollSpeed = Utils::Clampf(m_Background->scrollSpeed, 0.f, scrollMaxSpeed);

        if (m_MainCharacter->IsPlaying(ANIM_MOVE_FAST) || m_MainCharacter->IsPlaying(ANIM_TRANSITION_BOOST))
        {
            m_MainCharacter->position.x += FastDisp * XFactor;
        }
        else if (m_MainCharacter->IsPlaying(ANIM_MOVE_SLOW))
        {
            m_MainCharacter->position.x += SlowDisp * XFactor;
        }

        m_MainCharacter->position.y += VDisp * YFactor;
        m_MainCharacter->position.y = Utils::Clampf(m_MainCharacter->position.y, roadRange.x, roadRange.y);
    }
}

void PaperGirl::UpdateMainCharacterAnim()
{
    if (m_MainCharacter->IsPlaying(m_MainCharacterWantedAnimState))
    {
        return;
    }

    bool loop = true;

    switch (m_MainCharacterWantedAnimState)
    {
    case ANIM_IDLE:
        //Don't interrupt transitions
        if (m_MainCharacter->IsFinished(ANIM_TRANSITION_START) && m_MainCharacter->IsFinished(ANIM_TRANSITION_BOOST))
        {
            m_MainCharacter->SetAnimation(m_MainCharacterWantedAnimState, false);
        }
        break;

    case ANIM_MOVE_SLOW:
    case ANIM_MOVE_NORMAL:
    case ANIM_MOVE_FAST:
        //If PaperGirl is idle, need to wait for transition to be over
        if (m_MainCharacter->IsPlaying(ANIM_IDLE))
        {
            m_MainCharacter->SetAnimation(ANIM_TRANSITION_START, false);
            break;
        }

        if (m_MainCharacter->IsPlaying(ANIM_TRANSITION_START) && !m_MainCharacter->IsFinished(ANIM_TRANSITION_START))
        {
            break;
        }

        //If PaperGirl is moving fast, there's another specific transition
        if (m_MainCharacterWantedAnimState == ANIM_MOVE_FAST)
        {
            if (!m_MainCharacter->IsPlaying(ANIM_TRANSITION_BOOST))
            {
                m_MainCharacter->SetAnimation(ANIM_TRANSITION_BOOST, false);
                break;
            }

            if (m_MainCharacter->IsFinished(ANIM_TRANSITION_BOOST))
            {
                m_MainCharacter->SetAnimation(m_MainCharacterWantedAnimState);
            }
        }
        else
        {
            m_MainCharacter->SetAnimation(m_MainCharacterWantedAnimState);
        }
        break;

    default:
        m_MainCharacter->SetAnimation(m_MainCharacterWantedAnimState);
        break;
    }    
}

void PaperGirl::InitBackground()
{
    std::vector<const char*> bitmaps;
    bitmaps.push_back(".\\PaperGirl\\Data\\Background01.bmp");

    m_Background = new ScrollingBackground();
    m_Background->Init(bitmaps, SCROLL_HORIZONTAL);
    m_Background->scale = Vector2D(1.25f, 1.25f);
    m_Background->position.y = 0.625f;
    m_Background->scrollSpeed = 0.0f;
}

void PaperGirl::StartRide()
{
    m_RideStarted = true;
}

void PaperGirl::ResetGame()
{
    //Init PaperGirl TRS
    m_MainCharacter->position = Vector2D(0.25f, 0.25f);
    m_MainCharacter->scale = Vector2D(1.f, 1.f);
    m_MainCharacterWantedAnimState = ANIM_IDLE;

    //Init Background
    m_Background->Reset();
    m_Background->scrollSpeed = 0.f;

    //Reset gameplay variables
    m_RideStarted = false;
}

float PaperGirl::GetBoostLevel() const
{
    return m_BoostLevel;
}

float PaperGirl::GetXInput() const
{
    if (m_ActiveController)
    {
        if (m_ActiveController->GetLeftThumbStickX() > STICK_THRESHOLD || m_ActiveController->GetLeftThumbStickX() < -STICK_THRESHOLD)
        {
            return m_ActiveController->GetLeftThumbStickX();
        }
        if (m_ActiveController->CheckButton(XINPUT_GAMEPAD_DPAD_RIGHT, false))
        {
            return 1.f;
        }
        if (m_ActiveController->CheckButton(XINPUT_GAMEPAD_DPAD_LEFT, false))
        {
            return -1.f;
        }
    }
    return 0.f;
}

float PaperGirl::GetYInput() const
{
    if (m_ActiveController)
    {
        if (m_ActiveController->GetLeftThumbStickY() > STICK_THRESHOLD || m_ActiveController->GetLeftThumbStickY() < -STICK_THRESHOLD)
        {
            return m_ActiveController->GetLeftThumbStickY();
        }
        if (m_ActiveController->CheckButton(XINPUT_GAMEPAD_DPAD_UP, false))
        {
            return 1.f;
        }
        if (m_ActiveController->CheckButton(XINPUT_GAMEPAD_DPAD_DOWN, false))
        {
            return -1.f;
        }
    }
    return 0.f;
}