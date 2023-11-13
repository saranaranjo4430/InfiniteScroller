//------------------------------------------------------------------------
#include "stdafx.h"

#include "..\app\app.h"

#include "GameSprite.h"
//------------------------------------------------------------------------

CGameSprite::CGameSprite(const GameViewport& _vp, const char* _fileName, int _columns, int _rows) : m_Vp(_vp)
{
    m_BaseSprite = App::CreateSprite(_fileName, _columns, _rows);
}
    
CGameSprite::~CGameSprite()
{
    delete m_BaseSprite;
}

void CGameSprite::Update(float _deltaTime)
{
    m_BaseSprite->Update(_deltaTime);

    m_BaseSprite->SetPosition(m_Vp.GetX(position.x), m_Vp.GetY(position.y));
    m_BaseSprite->SetScaleX(m_Vp.GetScaleX(scale.x));
    m_BaseSprite->SetScaleY(m_Vp.GetScaleY(scale.y));
    m_BaseSprite->SetAngle(angle);
}

void CGameSprite::Render()
{
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

