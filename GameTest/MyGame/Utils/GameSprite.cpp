//------------------------------------------------------------------------
#include "stdafx.h"

#include "..\app\app.h"

#include "GameSprite.h"
#include "Utils.h"
//------------------------------------------------------------------------

CGameSprite::CGameSprite(const char* _fileName, int _columns, int _rows)
{
    m_BaseSprite = App::CreateSprite(_fileName, _columns, _rows);

    SetCollision(CollisionType::BOX, Vector2D(1.f, 1.f));
}
    
CGameSprite::~CGameSprite()
{
    safedelete(m_BaseSprite);
    safedelete(m_CollisionBox);
    safedelete(m_CollisionCircle);
}

void CGameSprite::Update(float _deltaTime)
{
    m_BaseSprite->Update(_deltaTime);

    m_BaseSprite->SetPosition(gameVp.GetX(position.x), gameVp.GetY(position.y));
    m_BaseSprite->SetScaleX(gameVp.GetScaleX(scale.x));
    m_BaseSprite->SetScaleY(gameVp.GetScaleY(scale.y));
    m_BaseSprite->SetAngle(Utils::DegToRad(angle));

    if (m_CollisionBox)
    {
        m_CollisionBox->Move(position);
        m_CollisionBox->Scale(scale.x, scale.y);
        m_CollisionBox->Rotate(angle);
    }
    else if (m_CollisionCircle)
    {
        m_CollisionCircle->Move(position);
        m_CollisionCircle->Scale(max(scale.x, scale.y));
    }
}

void CGameSprite::Render()
{
    if (HasFlag(GameSpriteFlags::DRAW_COLLISION))
    {
        if (m_CollisionBox)
        {
            m_CollisionBox->Draw(Utils::Color(0, 1, 0, 0.7f));
        }
        else if (m_CollisionCircle)
        {
            m_CollisionCircle->Draw(Utils::Color(0, 1, 0, 0.7f));
        }
    }

    m_BaseSprite->Draw();
}

void CGameSprite::SetAnimation(int id)
{
    m_BaseSprite->SetAnimation(id);
}

void CGameSprite::CreateAnimation(unsigned int id, float speed, const std::vector<int>& frames)
{
    m_BaseSprite->CreateAnimation(id, speed, frames);
}

void CGameSprite::SetCollision(const CollisionType& type, const Vector2D& scaleModifier)
{
    safedelete(m_CollisionBox);
    safedelete(m_CollisionCircle);

    float width = m_BaseSprite->GetWidth() / gameVp.GetWidth(1.f);
    float height = m_BaseSprite->GetHeight() / gameVp.GetHeight(1.f);

    switch(type)
    {
    case CollisionType::BOX:
        width *= scaleModifier.x;
        height *= scaleModifier.y;
        m_CollisionBox = new Rect2D(Vector2D(0, 0), width, height);
        break;

    case CollisionType::CIRCLE:
        
        float radius = max(width, height) * 0.5f * scaleModifier.x;
        m_CollisionCircle = new Circle2D(Vector2D(0, 0), radius);
        break;
    }
}

bool CGameSprite::Overlap(const CGameSprite* _other) const
{
    if (_other->m_CollisionBox)
    {
        return Overlap(*_other->m_CollisionBox);
    }
    if (_other->m_CollisionCircle)
    {
        return Overlap(*_other->m_CollisionCircle);
    }
    return false;
}

bool CGameSprite::Overlap(const Rect2D& _other) const
{
    if (m_CollisionBox)
    {
        return m_CollisionBox->Overlap(_other);
    }
    if (m_CollisionCircle)
    {
        return m_CollisionCircle->Overlap(_other);
    }
    return false;
}

bool CGameSprite::Overlap(const Circle2D& _other) const
{
    if (m_CollisionBox)
    {
        return m_CollisionBox->Overlap(_other);
    }
    if (m_CollisionCircle)
    {
        return m_CollisionCircle->Overlap(_other);
    }
    return false;
}
