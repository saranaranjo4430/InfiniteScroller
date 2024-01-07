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

    //Sprite TRS
    m_BaseSprite->SetPosition(gameVp.GetX(position.x), gameVp.GetY(position.y));
    m_BaseSprite->SetScaleX(gameVp.GetScaleX(scale.x));
    m_BaseSprite->SetScaleY(gameVp.GetScaleY(scale.y));
    m_BaseSprite->SetAngle(Utils::DegToRad(angle));

    //Collision TRS
    float scalex = scale.x * m_CollisionScale.x;
    float scaley = scale.y * m_CollisionScale.y;

    if (m_CollisionBox)
    {
        m_CollisionBox->Move(position + m_CollisionPos);
        m_CollisionBox->Scale(scalex, scaley);
        m_CollisionBox->Rotate(angle);
    }
    else if (m_CollisionCircle)
    {
        m_CollisionCircle->Move(position + m_CollisionPos);
        m_CollisionCircle->Scale(scalex, scaley);
        m_CollisionCircle->Rotate(angle);
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

void CGameSprite::CreateAnimation(unsigned int id, float duration, const std::vector<int>& frames)
{
    float speed = duration / frames.size();
    m_BaseSprite->CreateAnimation(id, speed, frames);
}

void CGameSprite::SetAnimation(int id, bool loop)
{
    m_BaseSprite->SetAnimation(id, loop);
}

bool CGameSprite::IsFinished(int id) const 
{ 
    return (!IsPlaying(id) || m_BaseSprite->IsAnimationFinished());
}

bool CGameSprite::IsPlaying(int id) const
{
    return (m_BaseSprite->GetAnimationId() == id);
}

void CGameSprite::SetCollision(const CollisionType& type, const Vector2D& scale, const Vector2D& pos)
{
    safedelete(m_CollisionBox);
    safedelete(m_CollisionCircle);

    float width = m_BaseSprite->GetWidth();
    float height = m_BaseSprite->GetHeight();

    switch (type)
    {
    case CollisionType::BOX:
        width /= gameVp.GetWidth();
        height /= gameVp.GetHeight();

        m_CollisionBox = new Rect2D(Vector2D(0, 0), width, height);
        break;

    case CollisionType::CIRCLE:
        float radius = max(width, height) * 0.5f;
        radius /= gameVp.GetWidth();

        m_CollisionCircle = new Circle2D(Vector2D(0, 0), radius);
        break;
    }

    m_CollisionPos = pos;
    m_CollisionScale = scale;
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

bool CGameSprite::Overlap(const Ellipse2D& _other) const
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

Vector2D CGameSprite::GetTextureSize() const
{
    return Vector2D(m_BaseSprite->GetWidth(), m_BaseSprite->GetHeight());
}

Vector2D CGameSprite::GetSizeOnScreen() const
{
    Vector2D size = GetTextureSize();

    size.x *= scale.x / gameVp.GetWidth();
    size.y *= scale.y / gameVp.GetHeight();

    return size;
}