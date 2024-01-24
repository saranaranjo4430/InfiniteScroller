//------------------------------------------------------------------------
#include <stdafx.h>

#include "..\MyGame\Utils\Utils.h"
#include "..\MyGame\Utils\GameSprite.h"

#include "Constants.h"
#include "SpawnPoint.h"
#include "PaperGirl.h"
//------------------------------------------------------------------------

SpawnPoint::SpawnPoint(SpawnPointOwner* _owner)
{
    m_Owner = _owner;
}

SpawnPoint::~SpawnPoint()
{

}

void SpawnPoint::Init(const SpawnPointType& _type, float _pivotPosY)
{
    m_Type = _type;
    m_PivotPosY = _pivotPosY;

    m_ScrollingRatio = -1.f;
}

void SpawnPoint::Update(float _deltaTime)
{
    if (!m_SpawnAsked)
    {
        assert(!m_SpawnedObject);

        if (IsAttachedToBackground())
        {
            if (IsRatioInView())
            {
                const Range& scrollRatio = paperGirl->GetScrollingRatio();
                float posX = ((m_ScrollingRatio + 0.01f) - scrollRatio.min) / (scrollRatio.max - scrollRatio.min);

                if (m_Owner->RequestSpawnPointActivation(this, posX))
                {
                    SpawnObject(posX);

                    m_SpawnedObject->Update(_deltaTime);
                }

                m_SpawnAsked = true;
            }
        }
        else
        {
            float posX = Constants::Background::offscreenSafeSpawnPos.max;
            if (m_Owner->RequestSpawnPointActivation(this, posX))
            {
                SpawnObject(posX);

                m_SpawnedObject->Update(_deltaTime);
            }

            m_SpawnAsked = true;
        }
    }
    else
    {
        if (m_SpawnedObject)
        {
            m_SpawnedObject->sprite->position.x -= paperGirl->GetScrollingSpeed() * _deltaTime / 1000.f;
            m_SpawnedObject->sprite->position.y = m_PivotPosY + m_SpawnedObject->GetSizeOnScreen().y * 0.5f;

            m_SpawnedObject->Update(_deltaTime);

            if (m_SpawnedObject->sprite->position.x < Constants::Background::offscreenSafeSpawnPos.min)
            {
                if (m_Owner->RequestSpawnPointRemoval(this))
                {
                    UnspawnObject();
                }
            }
        }
        else if (!IsRatioInView())
        {
            m_SpawnAsked = false;
        }
    }
}

void SpawnPoint::Render()
{
    if (m_SpawnedObject)
    {
        m_SpawnedObject->Render();
    }
}

void SpawnPoint::Shutdown()
{
    UnspawnObject();
}

void SpawnPoint::Reset()
{
    UnspawnObject();
}

void SpawnPoint::AttachToBackground(float _scrollingRatio)
{
    m_ScrollingRatio = _scrollingRatio;
}

bool SpawnPoint::IsAttachedToBackground() const
{
    return m_ScrollingRatio >= 0.f;
}

CGameSprite* SpawnPoint::GetSprite() const
{
    assert(m_SpawnedObject);
    return m_SpawnedObject->sprite;
}

Vector2D SpawnPoint::GetCenterPos() const
{ 
    assert(m_SpawnedObject);
    return m_SpawnedObject->sprite->position;
}

Vector2D SpawnPoint::GetPivotPos() const
{
    assert(m_SpawnedObject);

    Vector2D pos;
    pos.x = m_SpawnedObject->sprite->position.x;
    pos.y = m_PivotPosY;
    return pos;
}

bool SpawnPoint::Overlap(const PropManipulator* _other) const
{
    return GetSprite()->Overlap(_other->GetSprite());
}

bool SpawnPoint::IsRatioInView() const
{
    const Range& scrollRatio = paperGirl->GetScrollingRatio();
    if (scrollRatio.min <= m_ScrollingRatio && m_ScrollingRatio <= scrollRatio.max)
    {
        return true;
    }

    return false;
}

void SpawnPoint::SpawnObject(float _posX)
{
    if (!m_SpawnedObject)
    {
        switch (m_Type)
        {
        case SpawnPointType::GRANDMA:
            m_SpawnedObject = new GrandMa();
            m_SpawnedObject->Init();
            m_SpawnedObject->sprite->scale = Vector2D(0.175f, 0.175f);
            break;

        case SpawnPointType::RECYCLEBIN:
            m_SpawnedObject = new RecycleBin();
            m_SpawnedObject->Init();
            m_SpawnedObject->sprite->scale = Vector2D(0.1f, 0.1f);
            break;

        case SpawnPointType::MAILBOX:
            m_SpawnedObject = new Mailbox();
            m_SpawnedObject->Init();
            m_SpawnedObject->sprite->scale = Vector2D(0.1f, 0.1f);
            break;
        }
    }

    assert(m_SpawnedObject);

    m_SpawnedObject->sprite->position.x = _posX;
    m_SpawnedObject->sprite->position.y = m_PivotPosY + m_SpawnedObject->GetSizeOnScreen().y * 0.5f;
}

void SpawnPoint::UnspawnObject()
{
    if (m_SpawnedObject)
    {
        m_SpawnedObject->Shutdown();
        safedelete(m_SpawnedObject);
    }

    m_SpawnAsked = false;
}
