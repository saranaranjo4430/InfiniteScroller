//------------------------------------------------------------------------
#include <stdafx.h>

#include "..\MyGame\Utils\Utils.h"
#include "..\MyGame\Utils\GameSprite.h"

#include "Constants.h"
#include "MainCharacter.h"
#include "PaperGirl.h"
//------------------------------------------------------------------------

MainCharacter::MainCharacter()
{

}

MainCharacter::~MainCharacter()
{

}

void MainCharacter::Init()
{
    //Init Prop
    m_Prop = new TheGirl();
    m_Prop->Init();
}

void MainCharacter::Update(float _deltaTime)
{
    float FastDisp = Constants::TheGirl::fastSpeed * _deltaTime / 1000.f;
    float SlowDisp = Constants::TheGirl::slowSpeed * _deltaTime / 1000.f;
    float VDisp = Constants::TheGirl::verticalSpeed * _deltaTime / 1000.f;

    float XFactor = paperGirl->GetXInput();
    float YFactor = paperGirl->GetYInput();

    //Determine animation to play
    if (!m_Prop->sprite->IsPlaying(TheGirl::ANIM_ACTION_THROW) && !m_Prop->sprite->IsPlaying(TheGirl::ANIM_ACTION_FALL))
    {
        //Default anim
        (paperGirl->IsGameStarted()) ? m_NextAnimation = TheGirl::ANIM_MOVE_NORMAL : m_NextAnimation = TheGirl::ANIM_IDLE;

        //Process inputs
        if (Utils::GetFirstActiveController())
        {
            if (XFactor > 0.f)
            {
                if (!paperGirl->IsGameStarted())
                {
                    paperGirl->StartGame();
                }

                if (paperGirl->GetBoostLevel() > 0.f)
                {
                    m_NextAnimation = TheGirl::ANIM_MOVE_FAST;
                }
            }
            else if (paperGirl->IsGameStarted() && XFactor < 0.f)
            {
                m_NextAnimation = TheGirl::ANIM_MOVE_SLOW;
            }
        }
    }

    m_Prop->SetAnimation(m_NextAnimation);

    //Update Position
    if (paperGirl->IsGameStarted())
    {
        if (m_Prop->sprite->IsPlaying(TheGirl::ANIM_MOVE_FAST) || m_Prop->sprite->IsPlaying(TheGirl::ANIM_TRANSITION_BOOST))
        {
            m_Prop->sprite->position.x += FastDisp * XFactor;
        }
        else if (m_Prop->sprite->IsPlaying(TheGirl::ANIM_MOVE_SLOW))
        {
            m_Prop->sprite->position.x += SlowDisp * XFactor;
        }

        m_Prop->sprite->position.y += VDisp * YFactor;

        Vector2D pivotPos = GetPivotPos();
        float clampedPosY = Utils::Clampf(pivotPos.y, Constants::Background::roadZone.min, Constants::Background::roadZone.max);
        m_Prop->sprite->position.y += (clampedPosY - pivotPos.y);
    }

    //Update sprite
    m_Prop->Update(_deltaTime);
}

void MainCharacter::Render()
{
    m_Prop->Render();
}

void MainCharacter::Shutdown()
{
    m_Prop->Shutdown();
    safedelete(m_Prop);
}

void MainCharacter::Reset()
{
    m_Prop->Reset();
    m_NextAnimation = TheGirl::ANIM_IDLE;
}

bool MainCharacter::IsBoosting() const
{
    return m_Prop->sprite->IsPlaying(TheGirl::ANIM_MOVE_FAST);
}

Vector2D MainCharacter::GetCenterPos() const
{
    return m_Prop->sprite->position;
}

Vector2D MainCharacter::GetPivotPos() const
{
    Vector2D pos;
    pos.x = m_Prop->sprite->position.x;
    pos.y = m_Prop->sprite->position.y - m_Prop->GetSizeOnScreen().y * 0.5f;
    return pos;
}

